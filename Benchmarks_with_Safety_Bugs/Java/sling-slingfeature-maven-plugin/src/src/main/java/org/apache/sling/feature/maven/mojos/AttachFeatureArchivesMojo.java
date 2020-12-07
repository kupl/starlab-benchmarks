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
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.jar.JarEntry;
import java.util.jar.JarOutputStream;
import java.util.jar.Manifest;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.plugins.annotations.ResolutionScope;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.io.archive.ArchiveWriter;
import org.apache.sling.feature.maven.ProjectHelper;

/**
 * Create a feature model archive and attach it. An archive is created for each
 * feature of this project
 */
@Mojo(
        name = "attach-featurearchives",
        defaultPhase = LifecyclePhase.PACKAGE,
        requiresDependencyResolution = ResolutionScope.TEST,
        threadSafe = true
    )
public class AttachFeatureArchivesMojo extends AbstractIncludingFeatureMojo {

    private static final String DEFAULT_CLASSIFIER = "far";

    public static final String ATTR_BUILT_BY = "Built-By";

    public static final String ATTR_CREATED_BY = "Created-By";

    public static final String ATTR_IMPLEMENTATION_VERSION = "Implementation-Version";

    public static final String ATTR_IMPLEMENTATION_VENDOR = "Implementation-Vendor";

    public static final String ATTR_IMPLEMENTATION_BUILD = "Implementation-Build";

    public static final String ATTR_IMPLEMENTATION_VENDOR_ID = "Implementation-Vendor-Id";

    public static final String ATTR_IMPLEMENTATION_TITLE = "Implementation-Title";

    public static final String ATTR_SPECIFICATION_TITLE = "Specification-Title";

    public static final String ATTR_SPECIFICATION_VENDOR = "Specification-Vendor";

    public static final String ATTR_SPECIFICATION_VERSION = "Specification-Version";

    @Parameter
    private List<Archive> archives;

    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        checkPreconditions();
        if (archives == null || archives.size() == 0) {
            // by default create an archive of each feature individually
            for (final Map.Entry<String, Feature> entry : ProjectHelper.getFeatures(this.project).entrySet()) {
                final boolean add;
                if (ProjectHelper.isAggregate(entry.getKey())) {
                    add = ProjectHelper.isAttachAggregate(entry.getKey());
                } else {
                    add = true;
                }

                if (add) {
                    final String classifier = entry.getValue().getId().getClassifier() == null ? DEFAULT_CLASSIFIER
                            : entry.getValue().getId().getClassifier().concat(DEFAULT_CLASSIFIER);
                    createArchive(Collections.singletonList(entry.getValue()), classifier, Archive.DEFAULT_EXTENSION, true);
                }
            }
        } else {
            for (final Archive archive : archives) {
                if (archive.getClassifier() == null) {
                    throw new MojoExecutionException("Classifier is missing for archive.");
                }
                if (archive.getType() == null) {
                    throw new MojoExecutionException("Type is missing for archive.");
                }
                final List<Feature> features = new ArrayList<>();
                features.addAll(this.getSelectedFeatures(archive).values());
                this.createArchive(features, archive.getClassifier(), archive.getType(), archive.attach);
            }
        }
    }

    private Manifest createBaseManifest(final Feature feature) {
        final Manifest mf = new Manifest();
        mf.getMainAttributes().putValue("Implementation-Build", project.getVersion());
        mf.getMainAttributes().putValue("Implementation-Version", project.getVersion());
        mf.getMainAttributes().putValue("Specification-Version", project.getVersion());

        if (feature.getVendor() != null) {
            mf.getMainAttributes().putValue("Implementation-Vendor", feature.getVendor());
            mf.getMainAttributes().putValue("Created-By", feature.getVendor());
            mf.getMainAttributes().putValue("Built-By", feature.getVendor());
            mf.getMainAttributes().putValue("Specification-Vendor", feature.getVendor());
        }

        mf.getMainAttributes().putValue("Implementation-Vendor-Id", project.getGroupId());
        if (feature != null && feature.getTitle() != null) {
            mf.getMainAttributes().putValue("Implementation-Title", feature.getTitle());
            mf.getMainAttributes().putValue("Specification-Title", feature.getTitle());
        } else if (project.getName() != null) {
            mf.getMainAttributes().putValue("Implementation-Title", project.getName());
            mf.getMainAttributes().putValue("Specification-Title", project.getName());
        }

        return mf;
    }

    /**
     * Create feature archive
     * @param features The features contained in the archive
     * @param classifier The classifier to use for the archive
     * @param type The type to use for the archive
     * @param attach Whether the archive should be attached to the project
     */
    private void createArchive(final List<Feature> features, final String classifier, final String type, final boolean attach) throws MojoExecutionException {
        final ArtifactId archiveId = features.get(0).getId().changeClassifier(classifier).changeType(type);

        // write the feature model archive
        final File outputFile = new File(
                this.project.getBuild().getDirectory().concat(File.separator).concat(archiveId.toMvnName()));
        outputFile.getParentFile().mkdirs();

        getLog().info("Creating feature archive " + outputFile.getName());
        try ( final FileOutputStream fos = new FileOutputStream(outputFile);
              final JarOutputStream jos = ArchiveWriter.write(fos,
                        createBaseManifest(features.size() == 1 ? features.get(0) : null), id -> {

                        try {
                            return ProjectHelper.getOrResolveArtifact(project, mavenSession, artifactHandlerManager,
                                    artifactResolver, id).getFile().toURI().toURL();
                        } catch (final MalformedURLException e) {
                            getLog().debug("Malformed url " + e.getMessage(), e);
                            // ignore
                            return null;
                        }
                    }
                        , features.toArray(new Feature[features.size()]))) {

            // handle license etc.
            final File classesDir = new File(this.project.getBuild().getOutputDirectory());
            if ( classesDir.exists() ) {
                final File metaInfDir = new File(classesDir, "META-INF");
                for(final String name : new String[] {"LICENSE", "NOTICE", "DEPENDENCIES"}) {
                    final File f = new File(metaInfDir, name);
                    if ( f.exists() ) {
                        final JarEntry artifactEntry = new JarEntry("META-INF/" + name);
                        jos.putNextEntry(artifactEntry);

                        final byte[] buffer = new byte[8192];
                        try (final InputStream is = new FileInputStream(f)) {
                            int l = 0;
                            while ( (l = is.read(buffer)) > 0 ) {
                                jos.write(buffer, 0, l);
                            }
                        }
                        jos.closeEntry();

                    }
                }
            }
            jos.finish();
        } catch (final IOException e) {
            throw new MojoExecutionException(
                    "Unable to write feature model archive to " + outputFile + " : " + e.getMessage(), e);
        }

        if ( attach ) {
            // attach it as an additional artifact
            projectHelper.attachArtifact(project, archiveId.getType(), archiveId.getClassifier(), outputFile);
        }
    }
}
