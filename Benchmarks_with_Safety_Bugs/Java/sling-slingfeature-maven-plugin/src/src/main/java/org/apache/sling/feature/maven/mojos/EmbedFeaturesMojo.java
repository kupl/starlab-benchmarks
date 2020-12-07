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
import java.util.Map;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.plugins.annotations.ResolutionScope;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.io.json.FeatureJSONWriter;

/**
 * Embed the features in the resources
 */
@Mojo(name = "embed-features", defaultPhase = LifecyclePhase.PREPARE_PACKAGE,
      requiresDependencyResolution = ResolutionScope.TEST,
      threadSafe = true
    )
public class EmbedFeaturesMojo extends AbstractIncludingFeatureMojo {

    /**
     * Configuration to define the features to be embedded, by default all features
     * are embedded.
     */
    @Parameter
    private FeatureSelectionConfig embed;

    /** Path where the features are embedded in the target/classes directory. */
    @Parameter(defaultValue = "META-INF/features")
    private String resourcesPath;

    /**
     * The project's build output directory (i.e. target/classes).
     */
    @Parameter(defaultValue = "${project.build.outputDirectory}", readonly = true)
    private File buildOutputDirectory;

    private void include(final File directory, final Feature feature)
    throws MojoExecutionException {
        // write the feature
        final String classifier = feature.getId().getClassifier();
        final File outputFile = new File(directory,
                classifier == null ? "feature.json" : "feature-" + classifier + ".json");
        outputFile.getParentFile().mkdirs();

        try ( final Writer writer = new FileWriter(outputFile)) {
            FeatureJSONWriter.write(writer, feature);
        } catch (final IOException e) {
            throw new MojoExecutionException("Unable to write feature " + feature.getId().toMvnId() + " to " + outputFile, e);
        }
    }

    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        checkPreconditions();

        final Map<String, Feature> features = embed == null ? this.selectAllFeatureFilesAndAggregates()
                : this.getSelectedFeatures(embed);
        if (features.isEmpty()) {
            getLog().info("No features to embed");
        } else {
            getLog().info("Embedding " + features.size() + " features at " + this.resourcesPath);

            final File directory = new File(buildOutputDirectory, this.resourcesPath.replace('/', File.separatorChar));
            for (final Feature f : features.values()) {
                this.include(directory, f);
            }
        }
    }
}
