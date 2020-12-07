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
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.apache.maven.artifact.Artifact;
import org.apache.maven.model.Dependency;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.ExtensionType;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.io.json.FeatureJSONWriter;
import org.apache.sling.feature.maven.ProjectHelper;

public abstract class AbstractRepositoryMojo extends AbstractIncludingFeatureMojo {

    /**
     * The directory to store the artifacts into.
     */
    @Parameter(defaultValue = "artifacts", property = "repositoryDir")
    String repositoryDir;

    boolean decompress;

    @Override
    public abstract void execute() throws MojoExecutionException, MojoFailureException;

    protected void doExecute(final File artifactDir, final Map<String, Feature> features, final List<Dependency> embed)
            throws MojoExecutionException, MojoFailureException {
        this.getLog().info("Creating repository in '" + artifactDir.getPath() + "'...");

        for (final Feature feature : features.values()) {
            processFeature(artifactDir, feature);
        }

        if (embed != null) {
            for (Dependency include : embed) {
                copyArtifactToRepository(ProjectHelper.toArtifactId(include), artifactDir);
            }
        }
    }
    protected Feature getLocalFeature(final ArtifactId id) {
        final Map<String, org.apache.sling.feature.Feature> features = ProjectHelper.getAssembledFeatures(this.project);
        for (Feature f : features.values()) {
            if (f.getId().equals(id)) {
                return f;
            }
        }
        return null;
    }

    protected void processFeature(final File artifactDir, final Feature f) throws MojoExecutionException {
        for(final org.apache.sling.feature.Artifact artifact : f.getBundles()) {
            copyArtifactToRepository(artifact.getId(), artifactDir);
        }
        for(final Extension ext : f.getExtensions()) {
            if ( ext.getType() == ExtensionType.ARTIFACTS ) {
                for(final org.apache.sling.feature.Artifact artifact : ext.getArtifacts()) {
                    copyArtifactToRepository(artifact.getId(), artifactDir);
                }
            }
        }

        final File featureFile = getRepositoryFile(artifactDir, f.getId());
        featureFile.getParentFile().mkdirs();
        try ( final Writer writer = new FileWriter(featureFile)) {
            FeatureJSONWriter.write(writer, f);
        } catch (final IOException e) {
            throw new MojoExecutionException("Unable to write feature file  :" + f.getId().toMvnId(), e);
        }
        if ( f.getPrototype() != null ) {
            if (ProjectHelper.isLocalProjectArtifact(this.project, f.getPrototype().getId())) {
                final Feature prototype = this.getLocalFeature(f.getPrototype().getId());
                if (prototype == null) {
                    throw new MojoExecutionException(
                            "Unable to find project feature " + f.getPrototype().getId().toMvnId());
                }
                processFeature(artifactDir, prototype);
            } else {
                final Feature prototype = ProjectHelper.getOrResolveFeature(project, mavenSession, artifactHandlerManager,
                        artifactResolver, f.getPrototype().getId());
                processFeature(artifactDir, prototype);
            }
        }
    }

    /**
     * Get the file in the repository directory
     * @param artifactDir The base artifact directory
     * @param artifact The artifact
     * @return The file
     */
    private File getRepositoryFile(final File artifactDir, final org.apache.sling.feature.ArtifactId artifact) {
        final StringBuilder artifactNameBuilder = new StringBuilder();
        artifactNameBuilder.append(artifact.getArtifactId());
        artifactNameBuilder.append('-');
        artifactNameBuilder.append(artifact.getVersion());
        if ( artifact.getClassifier() != null ) {
            artifactNameBuilder.append('-');
            artifactNameBuilder.append(artifact.getClassifier());
        }
        artifactNameBuilder.append('.');
        artifactNameBuilder.append(artifact.getType());
        final String artifactName = artifactNameBuilder.toString();

        final StringBuilder sb = new StringBuilder();
        sb.append(artifact.getGroupId().replace('.', File.separatorChar));
        sb.append(File.separatorChar);
        sb.append(artifact.getArtifactId());
        sb.append(File.separatorChar);
        sb.append(artifact.getVersion());
        sb.append(File.separatorChar);
        sb.append(artifactName);
        final String destPath = sb.toString();

        final File artifactFile = new File(artifactDir, destPath);
        artifactFile.getParentFile().mkdirs();

        return artifactFile;
    }

    /**
     * Copy a single artifact to the repository
     * @throws MojoExecutionException
     */
    private void copyArtifactToRepository(final ArtifactId artifactId,
            final File artifactDir)
    throws MojoExecutionException {
        final File artifactFile = getRepositoryFile(artifactDir, artifactId);
        // TODO - we could overwrite snapshots?
        if ( artifactFile.exists() ) {
            return;
        }
        final Artifact source = ProjectHelper.getOrResolveArtifact(this.project,
                this.mavenSession,
                this.artifactHandlerManager,
                this.artifactResolver,
                artifactId);

        try {
            if (decompress) {
                copyAndDecompressArtifact(source.getFile(), artifactFile);
            } else {
                copyArtifact(source.getFile(), artifactFile);
            }
        } catch (IOException e) {
            throw new MojoExecutionException("Unable to copy artifact from " + source.getFile(), e);
        }
    }

    void copyAndDecompressArtifact(final File sourceFile, final File artifactFile) throws IOException {
        getLog().info("Decompressing " + artifactFile);
        JarDecompressor.copyDecompress(sourceFile, artifactFile);
    }

    void copyArtifact(final File sourceFile, final File artifactFile) throws IOException {
        FileUtils.copyFile(sourceFile, artifactFile);
    }
}
