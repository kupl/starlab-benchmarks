/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.sling.feature.maven.mojos;

import java.io.File;
import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import org.apache.maven.artifact.handler.manager.ArtifactHandlerManager;
import org.apache.maven.artifact.resolver.ArtifactResolver;
import org.apache.maven.execution.MavenSession;
import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugins.annotations.Component;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.project.MavenProject;
import org.apache.maven.project.MavenProjectHelper;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.builder.ArtifactProvider;
import org.apache.sling.feature.builder.BuilderContext;
import org.apache.sling.feature.builder.FeatureBuilder;
import org.apache.sling.feature.builder.FeatureProvider;
import org.apache.sling.feature.io.json.FeatureJSONReader;
import org.apache.sling.feature.maven.FeatureConstants;
import org.apache.sling.feature.maven.FeatureProjectConfig;
import org.apache.sling.feature.maven.ProjectHelper;

/**
 * Base class for all mojos.
 */
public abstract class AbstractFeatureMojo extends AbstractMojo {

    private static final String PROPERTY_HANDLED_GENERATED_FEATURES = Feature.class.getName() + "/generated";

    /**
     * All of the below configurations are handled by the Preprocessor. Mojos should
     * only use them for informational purposes but not for processing! The read
     * features and test features are available through the ProjectHelper.
     */

    /**
     * Directory containing feature files
     */
    @Parameter(name = FeatureProjectConfig.CFG_FEATURES,
            required = true,
            defaultValue = FeatureProjectConfig.DEFAULT_FEATURE_DIR)
    protected File features;

    /**
     * Comma separated list of includes for the feature files in
     * the configured directory. Only feature files specified by
     * this include are processed.
     */
    @Parameter(name = FeatureProjectConfig.CFG_FEATURES_INCLUDES,
            defaultValue = FeatureProjectConfig.DEFAULT_FEATURE_INCLUDES)
    private String featuresIncludes;

    /**
     * Comma separated list of excludes for the feature files.
     * Feature files excluded by this configuration are not processed
     * at all.
     */
    @Parameter(name = FeatureProjectConfig.CFG_FEATURES_EXCLUDES)
    private String featuresExcludes;

    /**
     * Directory containing test feature files.
     */
    @Parameter(name = FeatureProjectConfig.CFG_TEST_FEATURES,
            required = true,
            defaultValue = FeatureProjectConfig.DEFAULT_TEST_FEATURE_DIR)
    private File testFeatures;

    /**
     * Comma separated list of includes for the test features.
     */
    @Parameter(name = FeatureProjectConfig.CFG_TEST_FEATURES_INCLUDES,
            defaultValue = FeatureProjectConfig.DEFAULT_FEATURE_INCLUDES)
    private String testFeaturesIncludes;

    /**
     * Comma separated list of excludes for the test features.
     */
    @Parameter(name = FeatureProjectConfig.CFG_TEST_FEATURES_EXCLUDES)
    private String testFeaturesExcludes;

    /**
     * If set to {@code true} the features are validated against the JSON schema.
     */
    @Parameter(name = FeatureProjectConfig.CFG_VALIDATE_FEATURES, defaultValue = "true")
    private boolean validateFeatures;

    /**
     * If set to {@code true} the artifacts from the feature are not as dependencies
     * to the project.
     */
    @Parameter(name=FeatureProjectConfig.CFG_SKIP_ADD_FEATURE_DEPENDENCIES,
            defaultValue="false")
    private boolean skipAddFeatureDependencies;

    /**
     * If set to {@code true} the artifacts from the test feature are not as dependencies to the project.
     */
    @Parameter(name=FeatureProjectConfig.CFG_SKIP_ADD_TEST_FEATURE_DEPENDENCIES,
            defaultValue="true")
    private boolean skipAddTestFeatureDependencies;

    /**
     * If set to {@code true} the main jar artifact is not added to the feature.
     */
    @Parameter(name=FeatureProjectConfig.CFG_SKIP_ADD_JAR_TO_FEATURE,
            defaultValue="false")
    private boolean skipAddJarToFeature;

    /**
     * If set to {@code true} the main jar artifact is not added to the test feature.
     */
    @Parameter(name=FeatureProjectConfig.CFG_SKIP_ADD_JAR_TO_TEST_FEATURE,
            defaultValue="false")
    private boolean skipAddJarToTestFeature;

    /**
     * Directory containing generated feature files
     */
    @Parameter
    private File generatedFeatures;

    /**
     * Comma separated list of includes for the generated feature files in the
     * configured directory. Only feature files specified by this include are
     * processed.
     */
    @Parameter(defaultValue = FeatureProjectConfig.DEFAULT_FEATURE_INCLUDES)
    private String generatedFeaturesIncludes;

    /**
     * Comma separated list of excludes for the generated feature files. Feature
     * files excluded by this configuration are not processed at all.
     */
    @Parameter
    private String generatedFeaturesExcludes;

    /**
     * The start level for the attached jar/bundle.
     */
    @Parameter(name=FeatureProjectConfig.CFG_JAR_START_ORDER)
    private int jarStartOrder;

    /**
     * Enable the replacement of variables when reading a feature model. The supported
     * variables are "project.groupId", "project.artifactId", "project.version" and
     * "project.osgiVersion".
     * @since 1.3.6
     */
    @Parameter(defaultValue = "true")
    private boolean enableProjectVariableReplacement;

    /**
     * A comma separated list of variables which are replaced when a feature model
     * is read. The value of these variables is fetched from the project properties.
     * @since 1.3.6
     */
    @Parameter
    private String replacePropertyVariables;

    /**
     * Enable old variable replacement in feature model based on the full maven
     * project including system variables.
     * If this is enabled, enableProjectVariableReplacement and
     * replacePropertyVariables have no effect.
     * @since 1.3.6
     */
    @Parameter(defaultValue = "false")
    private boolean enableLegacyVariableReplacement;

    @Parameter(property = "project", readonly = true, required = true)
    protected MavenProject project;

    @Parameter(property = "session", readonly = true, required = true)
    protected MavenSession mavenSession;

    @Component
    protected MavenProjectHelper projectHelper;

    @Component
    ArtifactHandlerManager artifactHandlerManager;

    @Component
    ArtifactResolver artifactResolver;

    protected File getTmpDir() {
        final File dir = new File(this.project.getBuild().getDirectory(), "slingfeature-tmp");
        dir.mkdirs();
        return dir;
    }

    /**
     * This method needs to be invoked by each mojo that deals with features
     *
     * @throws MojoExecutionException
     */
    protected void checkPreconditions() throws MojoExecutionException {
        final String errorMessage = ProjectHelper.checkPreprocessorRun(this.project);
        if (errorMessage != null) {
            throw new MojoExecutionException(errorMessage);
        }

        // make sure to check for generated features only once
        if (this.project.getContextValue(PROPERTY_HANDLED_GENERATED_FEATURES) == null) {

            this.handleGeneratedFeatures();

            this.project.setContextValue(PROPERTY_HANDLED_GENERATED_FEATURES, Boolean.TRUE);
        }
        if (FeatureConstants.PACKAGING_FEATURE.equals(project.getPackaging())
                && ProjectHelper.getFeatures(project).isEmpty()) {
            throw new MojoExecutionException("Feature project has no features defined");
        }
    }

    private void handleGeneratedFeatures() throws MojoExecutionException {
        final File dir;
        if (this.generatedFeatures == null) {
            final File targetDir = new File(this.project.getBasedir(), this.project.getBuild().getDirectory());
            final File genDir = new File(targetDir, "generated-features");
            if (genDir.exists()) {
                dir = genDir;
            } else {
                dir = null;
            }
        } else {
            dir = this.generatedFeatures;
        }
        if (dir != null) {
            if (!dir.exists()) {
                throw new MojoExecutionException("Directory does not exists: " + dir);
            }
            if (!dir.isDirectory()) {
                throw new MojoExecutionException(
                        "Generated features configuration is not a directory: " + dir);
            }

            final List<File> files = new ArrayList<>();
            ProjectHelper.scan(files, dir, this.generatedFeaturesIncludes,
                    this.generatedFeaturesExcludes);

            for (final File file : files) {
                getLog().debug("Reading feature file " + file);
                try {
                    final String json = ProjectHelper.readFeatureFile(project, file, null,
                            this.enableLegacyVariableReplacement,
                            this.enableProjectVariableReplacement,
                            this.replacePropertyVariables != null ? this.replacePropertyVariables.split(",") : null);

                    try (final Reader reader = new StringReader(json)) {
                        final Feature feature = FeatureJSONReader.read(reader, file.getAbsolutePath());

                        ProjectHelper.checkFeatureId(project, feature);

                        ProjectHelper.setFeatureInfo(project, feature);

                        // Add feature to map of features
                        final String key = file.toPath().normalize().toFile().getAbsolutePath();
                        ProjectHelper.getFeatures(this.project).put(key, feature);

                        // assemble feature and add
                        final Feature assembledFeature = FeatureBuilder.assemble(feature, getBuilderContext());
                        ProjectHelper.getAssembledFeatures(project).put(key, assembledFeature);

                        // finally validate classifier
                        ProjectHelper.validateFeatureClassifiers(project);
                    } catch (final IOException io) {
                        throw new MojoExecutionException("Unable to read feature " + file.getAbsolutePath(), io);
                    }
                } catch (final RuntimeException re) {
                    // this is a bit unusual, but as ProjectHelper can only throw RuntimeException
                    // it's
                    // more user friendly to catch it and rethrow a mojo friendly exception
                    throw new MojoExecutionException(re.getMessage(), re.getCause() != null ? re.getCause() : re);
                }
            }
        }
    }

    private BuilderContext getBuilderContext() {
        final BuilderContext builderContext = new BuilderContext(new BaseFeatureProvider())
            .setArtifactProvider(new BaseArtifactProvider());

        return builderContext;
    }

    protected class BaseFeatureProvider implements FeatureProvider {
        @Override
        public Feature provide(ArtifactId id) {
            // Check for the feature in the local context
            for (final Feature feat : ProjectHelper.getAssembledFeatures(project).values()) {
                if (feat.getId().equals(id)) {
                    return feat;
                }
            }

            if (ProjectHelper.isLocalProjectArtifact(project, id)) {
                throw new RuntimeException("Unable to resolve local artifact " + id.toMvnId());
            }

            // Finally, look the feature up via Maven's dependency mechanism
            return ProjectHelper.getOrResolveFeature(project, mavenSession, artifactHandlerManager,
                artifactResolver, id);
        }
    }

    protected class BaseArtifactProvider implements ArtifactProvider {
        @Override
        public URL provide(final ArtifactId id) {
            try {
                return ProjectHelper
                    .getOrResolveArtifact(project, mavenSession, artifactHandlerManager, artifactResolver, id)
                    .getFile().toURI().toURL();
            } catch (Exception e) {
                getLog().error(e);
                return null;
            }
        }
    }
}
