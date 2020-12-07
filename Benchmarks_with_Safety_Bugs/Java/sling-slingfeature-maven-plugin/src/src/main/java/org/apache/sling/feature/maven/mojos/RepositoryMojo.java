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
import java.util.List;
import java.util.Map;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.plugins.annotations.ResolutionScope;
import org.apache.sling.feature.Feature;

/**
 * Create a Maven repository structure from the referenced artifacts in the features.
 */
@Mojo(
        name = "repository",
        defaultPhase = LifecyclePhase.PACKAGE,
        requiresDependencyResolution = ResolutionScope.TEST,
        threadSafe = true
    )
public class RepositoryMojo extends AbstractRepositoryMojo {

    @Parameter
    List<Repository> repositories;

    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        checkPreconditions();
        if (repositories == null || repositories.isEmpty()) {
            final File artifactDir = new File(this.project.getBuild().getDirectory(), this.repositoryDir);
            this.doExecute(artifactDir, this.selectAllFeatureFilesAndAggregates(), null);
        } else {
            for (final Repository rep : this.repositories) {
                final Map<String, Feature> selection = this.getSelectedFeatures(rep);
                final File artifactDir = new File(this.project.getBuild().getDirectory(), rep.repositoryDir);
                this.decompress = rep.decompress;
                this.doExecute(artifactDir, selection, rep.getEmbedArtifacts());
            }
        }
    }
}
