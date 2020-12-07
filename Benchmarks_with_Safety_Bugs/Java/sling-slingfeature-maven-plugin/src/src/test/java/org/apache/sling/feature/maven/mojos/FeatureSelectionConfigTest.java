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

import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.maven.ProjectHelper;
import org.junit.Test;

public class FeatureSelectionConfigTest {

    @Test
    public void testOrdering() {
        final FeatureSelectionConfig cfg = new FeatureSelectionConfig();
        cfg.setIncludeClassifier("classifier");
        cfg.setFilesExclude("EXCLUDE");
        cfg.setIncludeArtifact(ProjectHelper.toDependency(ArtifactId.parse("gr:ar:1.1"), null));
        cfg.setFilesInclude("INC1");
        cfg.setIncludeClassifier("another");
        cfg.setFilesInclude("INC2");
        cfg.setRefsInclude("src/main/references/**.ref");
        cfg.setFilesInclude("INC3");

        // we should have five items
        assertEquals(7, cfg.getSelections().size());

        // check ordering
        assertEquals(FeatureSelectionConfig.SelectionType.CLASSIFIER, cfg.getSelections().get(0).type);
        assertEquals("classifier", cfg.getSelections().get(0).instruction);

        assertEquals(FeatureSelectionConfig.SelectionType.ARTIFACT, cfg.getSelections().get(1).type);
        assertEquals("gr:ar:1.1", cfg.getSelections().get(1).instruction);

        assertEquals(FeatureSelectionConfig.SelectionType.FILES_INCLUDE, cfg.getSelections().get(2).type);
        assertEquals("INC1", cfg.getSelections().get(2).instruction);

        assertEquals(FeatureSelectionConfig.SelectionType.CLASSIFIER, cfg.getSelections().get(3).type);
        assertEquals("another", cfg.getSelections().get(3).instruction);

        assertEquals(FeatureSelectionConfig.SelectionType.FILES_INCLUDE, cfg.getSelections().get(4).type);
        assertEquals("INC2", cfg.getSelections().get(4).instruction);

        assertEquals(FeatureSelectionConfig.SelectionType.REFS_INCLUDE, cfg.getSelections().get(5).type);
        assertEquals("src/main/references/**.ref", cfg.getSelections().get(5).instruction);

        assertEquals(FeatureSelectionConfig.SelectionType.FILES_INCLUDE, cfg.getSelections().get(4).type);
        assertEquals("INC3", cfg.getSelections().get(6).instruction);

        // check excludes
        assertEquals(1, cfg.getFilesExcludes().size());
        assertEquals("EXCLUDE", cfg.getFilesExcludes().get(0));
    }
}
