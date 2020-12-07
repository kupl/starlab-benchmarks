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

import org.apache.maven.model.Build;
import org.apache.maven.project.MavenProject;
import org.junit.Test;
import org.mockito.Mockito;

import java.util.HashMap;
import java.util.Map;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class AnalyseFeaturesMojoTest {
    @Test
    public void testTaskConfiguration() {
        Build mockBuild = Mockito.mock(Build.class);
        Mockito.when(mockBuild.getDirectory()).thenReturn("/some-build-dir");

        MavenProject mockProject = Mockito.mock(MavenProject.class);
        Mockito.when(mockProject.getBuild()).thenReturn(mockBuild);

        AnalyseFeaturesMojo afm = new AnalyseFeaturesMojo();
        afm.project = mockProject;

        Map<String, Map<String, String>> cfg = new HashMap<String, Map<String,String>>();
        afm.addTaskConfigurationDefaults(cfg);

        Map<String, String> allCfg = cfg.get("all");
        assertTrue(allCfg.get("fileStorage").startsWith("/some-build-dir/"));
    }

    @Test
    public void testTaskConfigurationOverridden() {
        Build mockBuild = Mockito.mock(Build.class);
        Mockito.when(mockBuild.getDirectory()).thenReturn("/some-build-dir");

        MavenProject mockProject = Mockito.mock(MavenProject.class);
        Mockito.when(mockProject.getBuild()).thenReturn(mockBuild);

        AnalyseFeaturesMojo afm = new AnalyseFeaturesMojo();
        afm.project = mockProject;

        Map<String, Map<String, String>> cfg = new HashMap<String, Map<String,String>>();

        Map<String,String> allMap = new HashMap<>();
        allMap.put("fileStorage", "/my-location");
        allMap.put("x", "y");
        cfg.put("all", allMap);

        afm.addTaskConfigurationDefaults(cfg);

        Map<String, String> allCfg = cfg.get("all");
        assertEquals("/my-location", allCfg.get("fileStorage"));
        assertEquals("y", allCfg.get("x"));
    }

    @Test
    public void testTaskConfigurationSupplied() {
        Build mockBuild = Mockito.mock(Build.class);
        Mockito.when(mockBuild.getDirectory()).thenReturn("/some-build-dir");

        MavenProject mockProject = Mockito.mock(MavenProject.class);
        Mockito.when(mockProject.getBuild()).thenReturn(mockBuild);

        AnalyseFeaturesMojo afm = new AnalyseFeaturesMojo();
        afm.project = mockProject;

        Map<String, Map<String, String>> cfg = new HashMap<String, Map<String,String>>();

        Map<String,String> allMap = new HashMap<>();
        allMap.put("x", "y");
        cfg.put("all", allMap);

        afm.addTaskConfigurationDefaults(cfg);

        Map<String, String> allCfg = cfg.get("all");
        assertTrue(allCfg.get("fileStorage").startsWith("/some-build-dir/"));
        assertEquals("y", allCfg.get("x"));
    }
}
