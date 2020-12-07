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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.File;
import java.io.FileWriter;
import java.io.Writer;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.maven.artifact.Artifact;
import org.apache.maven.artifact.DefaultArtifact;
import org.apache.maven.artifact.handler.DefaultArtifactHandler;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.project.MavenProject;
import org.apache.maven.shared.utils.io.FileUtils;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.io.json.FeatureJSONWriter;
import org.apache.sling.feature.maven.ProjectHelper;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;

public class AbstractIncludingFeatureMojoTest {

    private AbstractIncludingFeatureMojo mojo;

    @Before
    public void setup() {
        mojo = new AbstractIncludingFeatureMojo() {

            @Override
            public void execute() throws MojoExecutionException, MojoFailureException {
                // nothing to do
            }
        };

        mojo.project = Mockito.mock(MavenProject.class);
        Mockito.when(mojo.project.getGroupId()).thenReturn("g");
        Mockito.when(mojo.project.getArtifactId()).thenReturn("a");
        Mockito.when(mojo.project.getVersion()).thenReturn("1");
    }

    @Test
    public void testSelectAggregateClassifierNotFound() throws Exception {
        final FeatureSelectionConfig fsc = new FeatureSelectionConfig();
        fsc.setIncludeClassifier("agg");

        try {
            mojo.getSelectedFeatures(fsc);
            fail();
        } catch ( MojoExecutionException mee) {
            assertTrue(mee.getMessage().contains(" agg not found"));
        }
    }

    @Test
    public void testSelectAggregateClassifier() throws Exception {
        setupProjectFeatures(mojo.project, "agg", "foo");

        final FeatureSelectionConfig fsc = new FeatureSelectionConfig();
        fsc.setIncludeClassifier("agg");

        final Map<String, Feature> selection = mojo.getSelectedFeatures(fsc);
        assertEquals(1, selection.size());
        assertEquals("agg", selection.values().iterator().next().getId().getClassifier());
    }


    @Test
    public void testSelectAggregateClassifierFromFiles() throws Exception {
        setupProjectFeatures(mojo.project, Collections.singletonMap("/foo/file", new Feature(ArtifactId.parse("g:a:slingosgifeature:foo:1"))));

        final FeatureSelectionConfig fsc = new FeatureSelectionConfig();
        fsc.setIncludeClassifier("foo");

        final Map<String, Feature> selection = mojo.getSelectedFeatures(fsc);
        assertEquals(1, selection.size());
        assertEquals("foo", selection.values().iterator().next().getId().getClassifier());
    }

    @Test
    public void testSelectRefFiles() throws Exception {
        final File baseDirectory = Files.createTempDirectory("ref-files").toFile();
        try {
            Mockito.when(mojo.project.getBasedir()).thenReturn(baseDirectory);

            final ArtifactId a1 = ArtifactId.parse("g1:a1:1.0");
            final ArtifactId a2 = ArtifactId.parse("g2:a2:2.0");

            final Feature f1 = new Feature(a1);
            final Feature f2 = new Feature(a2);

            try ( final Writer writer = new FileWriter(new File(baseDirectory, "f1.json"))) {
                FeatureJSONWriter.write(writer, f1);
            }
            try ( final Writer writer = new FileWriter(new File(baseDirectory, "f2.json"))) {
                FeatureJSONWriter.write(writer, f2);
            }

            final Set<Artifact> depArtifacts = new HashSet<>();
            final Artifact art1 = new DefaultArtifact(a1.getGroupId(), a1.getArtifactId(), a1.getVersion(),
                    Artifact.SCOPE_PROVIDED, a1.getType(), a1.getClassifier(), new DefaultArtifactHandler());
            art1.setFile(new File(baseDirectory, "f1.json"));
            depArtifacts.add(art1);

            final Artifact art2 = new DefaultArtifact(a2.getGroupId(), a2.getArtifactId(), a2.getVersion(),
                    Artifact.SCOPE_PROVIDED, a2.getType(), a2.getClassifier(), new DefaultArtifactHandler());
            art2.setFile(new File(baseDirectory, "f2.json"));
            depArtifacts.add(art2);

            Mockito.when(mojo.project.getDependencyArtifacts()).thenReturn(depArtifacts);

            final File refsDir = new File(baseDirectory, "src" + File.separatorChar + "main" + File.separatorChar + "references");
            refsDir.mkdirs();

            final List<String> refs = new ArrayList<>();
            refs.add("# Some comment");
            refs.add(a1.toMvnUrl());
            refs.add(a2.toMvnUrl());
            Files.write(new File(refsDir, "artifacts.refs").toPath(), refs);
            final List<String> garbage = Collections.singletonList("foo");
            Files.write(new File(refsDir, "artifacts.txt").toPath(), garbage);

            final FeatureSelectionConfig fsc = new FeatureSelectionConfig();
            fsc.setRefsInclude("src/main/references/*.refs");

            final Map<String, Feature> selection = mojo.getSelectedFeatures(fsc);
            assertEquals(2, selection.size());
            final List<ArtifactId> ids = new ArrayList<>();
            selection.values().stream().forEach(feature -> ids.add(feature.getId()));
            assertTrue(ids.contains(a1));
            assertTrue(ids.contains(a2));

        } finally {
            FileUtils.deleteDirectory(baseDirectory);
        }
    }

    private void setupProjectFeatures(final MavenProject project, final Map<String, Feature> features) {
        Mockito.when(project.getContextValue(Mockito.anyString())).thenReturn(features);
    }

    private void setupProjectFeatures(final MavenProject project, String... aggregates) {
        final Map<String, Feature> map = new HashMap<>();
        for(final String agg : aggregates) {
            map.put(ProjectHelper.generateAggregateFeatureKey(agg, true), new Feature(ArtifactId.parse("g:a:slingosgifeature:" + agg + ":1")));
        }
        setupProjectFeatures(project, map);
    }
}
