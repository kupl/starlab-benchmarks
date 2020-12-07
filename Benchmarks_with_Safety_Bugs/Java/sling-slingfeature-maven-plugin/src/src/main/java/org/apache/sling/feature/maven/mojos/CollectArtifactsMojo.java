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
import java.util.Map;
import java.util.StringTokenizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.maven.model.Dependency;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.sling.feature.Feature;

@Mojo(
    name = "collect-artifacts",
    requiresProject = false,
    threadSafe = true
)
public final class CollectArtifactsMojo extends AbstractRepositoryMojo {

    private final Pattern gavPattern = Pattern.compile("([^: ]+):([^: ]+)(:([^: ]*)(:([^: ]+))?)?:([^: ]+)");

    /**
     * A CSV list of Feature GAV.
     * Specifying this property, <code>includes</code> parameter will be overridden
     */
    @Parameter(property = "features")
    private String csvFeaturesGAV;

    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        final File artifactDir ;
        // this strange looking line checks whether this is outside of a maven project
        if ( this.project.getBuild().getDirectory().contains( "${project.basedir}" ) ) {
            artifactDir = new File(repositoryDir);
        } else {
            artifactDir = new File(this.project.getBuild().getDirectory(), repositoryDir);
        }
        this.doExecute(artifactDir, getFeatures(), null);
    }

    protected Map<String, Feature> getFeatures() throws MojoExecutionException {
        final FeatureSelectionConfig config = new FeatureSelectionConfig();

        if (csvFeaturesGAV != null && !csvFeaturesGAV.isEmpty()) {

            StringTokenizer tokenizer = new StringTokenizer(csvFeaturesGAV, ",");
            while (tokenizer.hasMoreTokens()) {
                String gav = tokenizer.nextToken();
                Matcher gavMatcher = gavPattern.matcher(gav);

                if (!gavMatcher.matches()) {
                    getLog().warn("Wrong GAV coordinates "
                            + gav
                            + " specified on 'features' property, expected format is groupId:artifactId[:packaging[:classifier]]:version");
                }

                final Dependency dep = new Dependency();
                dep.setGroupId(gavMatcher.group(1));
                dep.setArtifactId(gavMatcher.group(2));
                dep.setVersion(gavMatcher.group(7));
                dep.setType(gavMatcher.group(4));
                dep.setClassifier(gavMatcher.group(6));

                config.setIncludeArtifact(dep);
            }
        }

        return this.getSelectedFeatures(config);
    }
}
