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

import org.apache.maven.project.MavenProject;
import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.ExtensionState;
import org.apache.sling.feature.ExtensionType;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.maven.Preprocessor;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;

import static org.junit.Assert.assertEquals;

public class ExtractExtensionsMojoTest {
    private Path tempDir;

    @Before
    public void setup() throws Exception {
        tempDir = Files.createTempDirectory(getClass().getSimpleName());
    }

    @After
    public void tearDown() throws Exception {
        // Delete the temp dir again
        Files.walk(tempDir)
            .sorted(Comparator.reverseOrder())
            .map(Path::toFile)
            .forEach(File::delete);
    }

    @Test
    public void testExtractJSONExtensionFromAggregate() throws Exception {
        ExtractExtensionMojo mojo = new ExtractExtensionMojo();
        setupMavenProject(mojo);

        mojo.selection = new FeatureSelectionConfig();
        mojo.selection.setIncludeClassifier("myagg");
        mojo.extension = "my-json-ext";
        mojo.outputDir = tempDir + "/extracted1";

        mojo.execute();

        byte[] bytes = Files.readAllBytes(new File(mojo.outputDir + "/myagg-my-json-ext.json").toPath());
        assertEquals("[{\"x\": 1234}]", new String(bytes));
    }

    @Test
    public void testExtractTxtExtensionFromFeatureFile() throws Exception {
        ExtractExtensionMojo mojo = new ExtractExtensionMojo() {{this.features = tempDir.toFile();}};
        setupMavenProject(mojo);

        mojo.selection = new FeatureSelectionConfig();
        mojo.selection.setFilesInclude("myfeat.json");
        mojo.extension = "my-text-ext";
        mojo.outputDir = tempDir + "/extracted2";

        mojo.execute();

        byte[] bytes = Files.readAllBytes(new File(mojo.outputDir + "/my-text-ext.txt").toPath());
        assertEquals("hi there", new String(bytes));
    }

    @Test
    public void testExtractArtifactsFromAggregagte() throws Exception {
        ExtractExtensionMojo mojo = new ExtractExtensionMojo();
        setupMavenProject(mojo);

        mojo.selection = new FeatureSelectionConfig();
        mojo.selection.setIncludeClassifier("myagg");
        mojo.extension = "my-artifact-ext";
        mojo.outputDir = tempDir + "/extracted3";

        mojo.execute();

        byte[] bytes = Files.readAllBytes(new File(mojo.outputDir + "/myagg-my-artifact-ext.txt").toPath());
        assertEquals("a:b:1" + System.lineSeparator() + "c:d:2" + System.lineSeparator(),
                new String(bytes));
    }

    private void setupMavenProject(ExtractExtensionMojo mojo) {
        mojo.project = new MavenProject();
        mojo.project.setGroupId("g");
        mojo.project.setArtifactId("a");
        mojo.project.setVersion("9");
        mojo.project.setContextValue(Preprocessor.class.getName(), "yes!");
        mojo.project.setContextValue(Feature.class.getName() + "/generated", Boolean.TRUE);
        mojo.project.setContextValue(Feature.class.getName() + "/assembledmain.json-cache", getFeatureMap());
    }

    private Map<String, Feature> getFeatureMap() {
        Map<String, Feature> featureMap = new HashMap<>();
        Feature f1 = new Feature(ArtifactId.fromMvnId("g:a:slingosgifeature:myagg:9"));
        Extension e1 = new Extension(ExtensionType.JSON, "my-json-ext", ExtensionState.OPTIONAL);
        e1.setJSON("[{\"x\": 1234}]");
        f1.getExtensions().add(e1);
        Extension e2 = new Extension(ExtensionType.TEXT, "my-text-ext", ExtensionState.REQUIRED);
        e2.setText("blah blah");
        f1.getExtensions().add(e2);
        Extension e3 = new Extension(ExtensionType.ARTIFACTS, "my-artifact-ext", ExtensionState.OPTIONAL);
        e3.getArtifacts().add(new Artifact(ArtifactId.fromMvnId("a:b:1")));
        e3.getArtifacts().add(new Artifact(ArtifactId.fromMvnId("c:d:2")));
        f1.getExtensions().add(e3);
        featureMap.put(tempDir.toString() + "/myagg.json", f1);

        Feature f2 = new Feature(ArtifactId.fromMvnId(("g:a:1")));
        Extension e4 = new Extension(ExtensionType.TEXT, "my-text-ext1", ExtensionState.REQUIRED);
        e4.setText("lala");
        f2.getExtensions().add(e4);
        Extension e5 = new Extension(ExtensionType.TEXT, "my-text-ext2", ExtensionState.REQUIRED);
        e5.setText("yeah yeah");
        f2.getExtensions().add(e5);
        Extension e6 = new Extension(ExtensionType.TEXT, "my-text-ext", ExtensionState.REQUIRED);
        e6.setText("hi there");
        f2.getExtensions().add(e6);
        featureMap.put(tempDir.toString() + "/myfeat.json", f2);
        return featureMap;
    }
}
