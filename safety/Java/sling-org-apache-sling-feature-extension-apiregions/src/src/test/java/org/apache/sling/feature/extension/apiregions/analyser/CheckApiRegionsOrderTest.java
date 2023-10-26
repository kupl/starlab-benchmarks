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
package org.apache.sling.feature.extension.apiregions.analyser;

import java.util.HashMap;
import java.util.Map;

import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.ExtensionState;
import org.apache.sling.feature.ExtensionType;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.analyser.task.AnalyserTaskContext;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.junit.Test;
import org.mockito.Mockito;

public class CheckApiRegionsOrderTest  {
    @Test
    public void testValidOrder() throws Exception {
        CheckApiRegionsOrder caro = new CheckApiRegionsOrder();

        Extension e = new Extension(ExtensionType.JSON, ApiRegions.EXTENSION_NAME, ExtensionState.OPTIONAL);
        e.setJSON("[{\"name\":\"deprecated\","
                + "\"exports\": [\"a.b.c\"]},"
                + "{\"name\":\"internal\","
                + "\"exports\": [\"g.h.i\"]}]");

        Feature f = new Feature(ArtifactId.fromMvnId("a:b:1"));
        f.getExtensions().add(e);

        Map<String, String> cfgMap = new HashMap<>();
        cfgMap.put("order", "deprecated, internal ");

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getConfiguration()).thenReturn(cfgMap);
        Mockito.when(ctx.getFeature()).thenReturn(f);

        caro.execute(ctx);
        Mockito.verify(ctx, Mockito.never()).reportError(Mockito.anyString());
    }

    @Test
    public void testInvalidOrder() throws Exception {
        CheckApiRegionsOrder caro = new CheckApiRegionsOrder();

        Extension e = new Extension(ExtensionType.JSON, ApiRegions.EXTENSION_NAME, ExtensionState.OPTIONAL);
        e.setJSON("[{\"name\":\"deprecated\","
                + "\"exports\": [\"a.b.c\"]},"
                + "{\"name\":\"internal\","
                + "\"exports\": [\"d.e.f\"]},"
                + "{\"name\":\"deprecated\","
                + "\"exports\": [\"g.h.i\"]}]");

        Feature f = new Feature(ArtifactId.fromMvnId("a:b:1"));
        f.getExtensions().add(e);

        Map<String, String> cfgMap = new HashMap<>();
        cfgMap.put("order", "deprecated, internal ");

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getConfiguration()).thenReturn(cfgMap);
        Mockito.when(ctx.getFeature()).thenReturn(f);

        caro.execute(ctx);
        Mockito.verify(ctx).reportError(Mockito.contains("Invalid api regions"));
    }

    @Test
    public void testInvalidRegion() throws Exception {
        CheckApiRegionsOrder caro = new CheckApiRegionsOrder();

        Extension e = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        e.setJSON("[{\"name\":\"foo\"}]");

        Feature f = new Feature(ArtifactId.fromMvnId("a:b:1"));
        f.getExtensions().add(e);

        Map<String, String> cfgMap = new HashMap<>();
        cfgMap.put("order", "bar");

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getConfiguration()).thenReturn(cfgMap);
        Mockito.when(ctx.getFeature()).thenReturn(f);

        caro.execute(ctx);
        Mockito.verify(ctx).reportError(Mockito.contains("undeclared"));
    }

    @Test
    public void testNoExtensionsIsValid() throws Exception {
        CheckApiRegionsOrder caro = new CheckApiRegionsOrder();

        Map<String, String> cfgMap = new HashMap<>();
        cfgMap.put("order", "deprecated, internal ");

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getConfiguration()).thenReturn(cfgMap);
        Mockito.when(ctx.getFeature()).thenReturn(new Feature(ArtifactId.fromMvnId("a:b:1")));

        caro.execute(ctx);
        Mockito.verify(ctx, Mockito.never()).reportError(Mockito.anyString());
    }

    @Test
    public void testEmptyOrder() throws Exception {
        CheckApiRegionsOrder caro = new CheckApiRegionsOrder();

        Map<String, String> cfgMap = new HashMap<>();
        cfgMap.put("order", " ");

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getConfiguration()).thenReturn(cfgMap);
        Mockito.when(ctx.getFeature()).thenReturn(new Feature(ArtifactId.fromMvnId("a:b:1")));

        caro.execute(ctx);
        Mockito.verify(ctx).reportError(Mockito.contains("No regions"));
    }

    @Test
    public void testNoFeature() throws Exception {
        CheckApiRegionsOrder caro = new CheckApiRegionsOrder();

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getConfiguration()).thenReturn(new HashMap<>());

        caro.execute(ctx);
        Mockito.verify(ctx).reportError(Mockito.contains("No feature"));
    }

    @Test
    public void testNoOrderConfig() throws Exception {
        CheckApiRegionsOrder caro = new CheckApiRegionsOrder();

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getConfiguration()).thenReturn(new HashMap<>());
        Mockito.when(ctx.getFeature()).thenReturn(new Feature(ArtifactId.fromMvnId("a:b:1")));

        caro.execute(ctx);
        Mockito.verify(ctx).reportError(Mockito.contains("'order'"));
    }
}
