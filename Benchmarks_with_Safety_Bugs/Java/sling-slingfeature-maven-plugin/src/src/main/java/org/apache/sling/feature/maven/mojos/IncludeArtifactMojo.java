/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. The ASF
 * licenses this file to You under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package org.apache.sling.feature.maven.mojos;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.List;
import java.util.Map;

import org.apache.maven.model.Dependency;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.Component;
import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.plugins.annotations.ResolutionScope;
import org.apache.maven.shared.transfer.artifact.install.ArtifactInstaller;
import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Artifacts;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.ExtensionState;
import org.apache.sling.feature.ExtensionType;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.io.json.FeatureJSONWriter;
import org.apache.sling.feature.maven.FeatureConstants;
import org.apache.sling.feature.maven.ProjectHelper;

/**
 * This goal creates a Feature Model file that includes the Module Artifact as
 * bundle (or extension) so that the Artifact can be added through a FM into a
 * FM project. The FM file can be found in the 'build directory'/slingfeature-tmp
 * folder.
 * After a FM file is created successfully this file will be installed
 * in the local Maven Repository as 'slingosgifeature' file under the Module's Maven
 * Id location (group, artifact, version). This file can then later be used inside
 * the 'aggregate-features' goal with:
 * {@code
 * <includeArtifact>
 *     <groupId>org.apache.sling</groupId>
 *     <artifactId>org.apache.sling.test.feature</artifactId>
 *     <version>1.0.0</version>
 *     <classifier>my-test-classifier</classifier>
 *     <type>slingosgifeature</type>
 * </includeArtifact>
 * }
 * It also can add dependencies to the FM file if its scope is provided (normally
 * that would be 'compile'). In addition a bundle start order can be set for these
 * included dependency bundles.
 * Finally any FM files inside the Source FM folder are embedded into the FM file. This
 * allows to add extension files like 'repoinit' etc to be added to provide them with
 * the module.
 */
@Mojo(name = "include-artifact", defaultPhase = LifecyclePhase.PREPARE_PACKAGE, requiresDependencyResolution = ResolutionScope.COMPILE,
      threadSafe = true
    )
public class IncludeArtifactMojo extends AbstractIncludingFeatureMojo {

    public static final String CFG_CLASSIFIER = "includeArtifactClassifier";
    public static final String CFG_START_ORDER = "bundleStartOrder";
    public static final String CFG_INCLUDE_DEPENDENCIES_WITH_SCOPE = "includeDependenciesWithScope";

    /**
     * Classifier of the feature the current artifact is included in.
     * For simple projects a artifact classifier is not needed but in multi
     * feature projects the classifier is used to find the appropriate feature.
     */
    @Parameter(property = CFG_CLASSIFIER, required = false)
    private String includeArtifactClassifier;

    /**
     * Start Order of all included Dependencies.
     */
    @Parameter(property = CFG_START_ORDER, required = false, defaultValue = "-1")
    private int bundlesStartOrder;

    /**
     * All listed dependency's scopes will be added to the descriptor.
     */
    @Parameter(property = CFG_INCLUDE_DEPENDENCIES_WITH_SCOPE, required = false)
    private String[] includeDependenciesWithScope;

    /**
     * Name of the extension to include the artifact in. If not specified the
     * artifact is included as a bundle.
     */
    @Parameter
    private String includeArtifactExtension;

    /**
     * Name of the classifier for the artifact to be included. By default
     * the main artifact (no classifier) will be included.
     * @since 1.3.0
     */
    @Parameter
    private String includeClassifier;

    /**
     * Name of the type for the artifact to be included. By default
     * the type of the main artifact is used.
     * @since 1.3.0
     */
    @Parameter
    private String includeType;

    /**
     * Metadata for the included artifact.
     */
    @Parameter
    private Map<String, String> metadata;

    @Component
    protected ArtifactInstaller installer;

    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {

        checkPreconditions();

        final Map<String, Feature> featuresMap = ProjectHelper.getFeatures(this.project);
        Feature found = null;
        String key = null;
        for (final Map.Entry<String, Feature> entry : featuresMap.entrySet()) {
            if (includeArtifactClassifier != null && includeArtifactClassifier.equals(entry.getValue().getId().getClassifier())) {
                key = entry.getKey();
                found = entry.getValue();
                break;
            }
        }
        File file = null;
        if (found == null) {
            found = new Feature(new ArtifactId(this.project.getGroupId(), this.project.getArtifactId(),
                    this.project.getVersion(), includeArtifactClassifier, FeatureConstants.PACKAGING_FEATURE));

            file = new File(
                this.getTmpDir(),
                "feature" + (includeArtifactClassifier == null ? "" : "-" + includeArtifactClassifier) + ".json");
            key = file.getAbsolutePath();
            ProjectHelper.getFeatures(this.project).put(key, found);
            ProjectHelper.getAssembledFeatures(this.project).put(key, found);
        }

        final Artifact art = new Artifact(new ArtifactId(this.project.getGroupId(), this.project.getArtifactId(),
                this.project.getVersion(), includeClassifier, includeType != null ? includeType : this.project.getArtifact().getType()));
        if (metadata != null && metadata.size() > 0) {
            art.getMetadata().putAll(metadata);
        }
        includeArtifact(found, includeArtifactExtension, art);

        includeArtifact(ProjectHelper.getAssembledFeatures(this.project).get(key), includeArtifactExtension,
                art.copy(art.getId()));

        addDependencies(found);

        if (file != null) {
            try ( final Writer writer = new FileWriter(file)) {
                FeatureJSONWriter.write(writer, found);
            } catch (final IOException ioe) {
                throw new MojoExecutionException("Unable to write feature", ioe);
            }
        }
    }

    private void includeArtifact(final Feature f, final String extensionName, final Artifact art)
            throws MojoExecutionException {
        Artifacts container = f.getBundles();
        if (extensionName != null) {
            Extension ext = f.getExtensions().getByName(extensionName);
            if (ext == null) {
                ext = new Extension(ExtensionType.ARTIFACTS, extensionName, ExtensionState.REQUIRED);
                f.getExtensions().add(ext);
            }
            if (ext.getType() != ExtensionType.ARTIFACTS) {
                throw new MojoExecutionException(
                        "Wrong extension type for extension " + extensionName + " : " + ext.getType());
            }
            container = ext.getArtifacts();
        }
        container.add(art);
    }

    private void addDependencies(Feature feature) {
        // Add Dependencies if configured so
        for(String includeDependencyScope: includeDependenciesWithScope) {
            List<Dependency> dependencies = project.getDependencies();
            for(Dependency dependency: dependencies) {
                if(includeDependencyScope.equals(dependency.getScope())) {
                    ArtifactId id = new ArtifactId(
                        dependency.getGroupId(), dependency.getArtifactId(), dependency.getVersion(), dependency.getClassifier(), dependency.getType()
                    );
                    Artifact artifact = new Artifact(id);
                    if(bundlesStartOrder >= 0) {
                        artifact.setStartOrder(bundlesStartOrder);
                    }
                    feature.getBundles().add(artifact);
                }
            }
        }
    }
}
