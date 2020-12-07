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

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.ExtensionType;
import org.apache.sling.feature.Feature;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.Map;

@Mojo(name = "extract-extension",
    defaultPhase = LifecyclePhase.PACKAGE,
    threadSafe = true)
public class ExtractExtensionMojo extends AbstractIncludingFeatureMojo {
    @Parameter(name = "selection", required = true)
    FeatureSelectionConfig selection;

    @Parameter(name = "extension", required = true)
    String extension;

    @Parameter(name = "outputDir", required = true)
    String outputDir;

    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        checkPreconditions();
        File dir = new File(outputDir);
        dir.mkdirs();

        getLog().info("Feature Selection: " + selection);

        Map<String, Feature> selFeat = getSelectedFeatures(selection);
        for (Map.Entry<String, Feature> entry : selFeat.entrySet()) {
            Feature f = entry.getValue();
            Extension ext = f.getExtensions().getByName(extension);
            if (ext == null) {
                getLog().info("Feature " + f.getId() + " does not contain extension " + extension);
                continue;
            }

            String fileName = f.getId().getClassifier();
            if (fileName == null) {
                fileName = extension;
            } else {
                fileName += "-" + extension;
            }
            fileName += ext.getType() == ExtensionType.JSON ? ".json" : ".txt";

            try (Writer wr = new FileWriter(new File(dir, fileName))) {
                switch (ext.getType()) {
                case ARTIFACTS:
                    // List the artifacts as text
                    for (Artifact a : ext.getArtifacts()) {
                        wr.write(a.getId().toMvnId());
                        wr.write(System.lineSeparator());
                    }
                    break;
                case JSON:
                    wr.write(ext.getJSON());
                    break;
                case TEXT:
                    wr.write(ext.getText());
                    break;
                }
            } catch (IOException ex) {
                throw new MojoExecutionException("Problem writing feature" + outputDir, ex);
            }
        }
    }
}
