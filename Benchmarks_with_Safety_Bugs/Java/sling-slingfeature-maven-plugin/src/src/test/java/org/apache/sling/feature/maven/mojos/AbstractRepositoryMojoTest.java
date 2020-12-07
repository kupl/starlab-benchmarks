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

import org.apache.maven.artifact.handler.ArtifactHandler;
import org.apache.maven.artifact.handler.manager.ArtifactHandlerManager;
import org.apache.maven.artifact.resolver.ArtifactResolver;
import org.apache.maven.execution.MavenSession;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.project.MavenProject;
import org.apache.sling.feature.ArtifactId;
import org.junit.Test;
import org.mockito.Mockito;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Method;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;

import static org.junit.Assert.assertEquals;

public class AbstractRepositoryMojoTest {
    @Test
    public void testCopyArtifactToRepository() throws Exception {
        StringBuilder recordedOp = new StringBuilder();

        AbstractRepositoryMojo arm = new AbstractRepositoryMojo() {
            @Override
            public void execute() throws MojoExecutionException, MojoFailureException {
                // not used here
            }

            @Override
            void copyAndDecompressArtifact(File sourceFile, File artifactFile) throws IOException {
                recordedOp.append("copy_decompress");
            }

            @Override
            void copyArtifact(File sourceFile, File artifactFile) throws IOException {
                recordedOp.append("copy");
            }
        };
        arm.project = Mockito.mock(MavenProject.class);
        arm.mavenSession = Mockito.mock(MavenSession.class);
        arm.artifactResolver = Mockito.mock(ArtifactResolver.class);
        arm.artifactHandlerManager = Mockito.mock(ArtifactHandlerManager.class);
        Mockito.when(arm.artifactHandlerManager.getArtifactHandler(Mockito.anyString()))
                    .thenReturn(Mockito.mock(ArtifactHandler.class));

        Method m = AbstractRepositoryMojo.class.getDeclaredMethod("copyArtifactToRepository", ArtifactId.class, File.class);
        m.setAccessible(true);

        File td = Files.createTempDirectory(getClass().getSimpleName()).toFile();
        ArtifactId aid = ArtifactId.fromMvnId("foo:bar:123");
        try {
            m.invoke(arm, aid, td);
        } finally {
            deleteDirTree(td);
        }

        assertEquals("copy", recordedOp.toString());
    }

    @Test
    public void testCopyDecompressArtifactToRepository() throws Exception {
        StringBuilder recordedOp = new StringBuilder();

        AbstractRepositoryMojo arm = new AbstractRepositoryMojo() {
            @Override
            public void execute() throws MojoExecutionException, MojoFailureException {
                // not used here
            }

            @Override
            void copyAndDecompressArtifact(File sourceFile, File artifactFile) throws IOException {
                recordedOp.append("copy_decompress");
            }

            @Override
            void copyArtifact(File sourceFile, File artifactFile) throws IOException {
                recordedOp.append("copy");
            }
        };
        arm.project = Mockito.mock(MavenProject.class);
        arm.mavenSession = Mockito.mock(MavenSession.class);
        arm.artifactResolver = Mockito.mock(ArtifactResolver.class);
        arm.artifactHandlerManager = Mockito.mock(ArtifactHandlerManager.class);
        Mockito.when(arm.artifactHandlerManager.getArtifactHandler(Mockito.anyString()))
                    .thenReturn(Mockito.mock(ArtifactHandler.class));
        arm.decompress = true;

        Method m = AbstractRepositoryMojo.class.getDeclaredMethod("copyArtifactToRepository", ArtifactId.class, File.class);
        m.setAccessible(true);

        File td = Files.createTempDirectory(getClass().getSimpleName()).toFile();
        ArtifactId aid = ArtifactId.fromMvnId("foo:bar:123");
        try {
            m.invoke(arm, aid, td);
        } finally {
            deleteDirTree(td);
        }

        assertEquals("copy_decompress", recordedOp.toString());
    }


    private void deleteDirTree(File dir) throws IOException {
        Path tempDir = dir.toPath();

        Files.walk(tempDir)
            .sorted(Comparator.reverseOrder())
            .map(Path::toFile)
            .forEach(File::delete);
    }
}
