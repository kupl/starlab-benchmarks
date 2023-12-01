/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
package org.apache.sling.feature.extension.apiregions.analyser;

import java.io.File;
import java.io.IOException;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.ExtensionState;
import org.apache.sling.feature.ExtensionType;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.analyser.task.AnalyserTaskContext;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.apache.sling.feature.scanner.BundleDescriptor;
import org.apache.sling.feature.scanner.FeatureDescriptor;
import org.apache.sling.feature.scanner.impl.BundleDescriptorImpl;
import org.apache.sling.feature.scanner.impl.FeatureDescriptorImpl;
import org.junit.BeforeClass;
import org.junit.Test;
import org.mockito.Mockito;
import static org.junit.Assert.assertEquals;

public class CheckApiRegionsBundleExportsImportsTest {
    private static File resourceRoot;

    @BeforeClass
    public static void setupClass() {
        resourceRoot =
                new File(CheckApiRegionsBundleExportsImportsTest.class.
                        getResource("/test-framework.jar").getFile()).getParentFile();
    }

    @Test
    public void testId() {
        assertEquals(ApiRegions.EXTENSION_NAME + "-exportsimports", new CheckApiRegionsBundleExportsImports().getId());
    }

    @Test
    /*
     * Bundle b3 imports org.foo.e, but no bundle exports it. The feature is marked
     * as complete which it isn't
     */
    public void testImportExportNoRegionsMarkedAsComplete() throws Exception {
        CheckApiRegionsBundleExportsImports t = new CheckApiRegionsBundleExportsImports();

        Feature f = new Feature(ArtifactId.fromMvnId("f:f:1"));
        f.setComplete(true);
        FeatureDescriptor fd = new FeatureDescriptorImpl(f);

        fdAddBundle(fd, "g:b1:1", "test-bundle1.jar");
        fdAddBundle(fd, "g:b3:1", "test-bundle3.jar");

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getFeature()).thenReturn(f);
        Mockito.when(ctx.getFeatureDescriptor()).thenReturn(fd);
        t.execute(ctx);

        Mockito.verify(ctx, Mockito.times(2)).reportError(Mockito.anyString());
        Mockito.verify(ctx).reportError(Mockito.contains("org.foo.e"));
        Mockito.verify(ctx).reportError(Mockito.contains("marked as 'complete'"));
        Mockito.verify(ctx, Mockito.never()).reportWarning(Mockito.anyString());
    }

    @Test
    public void testImportExportNoRegionsAllOk() throws Exception {
        CheckApiRegionsBundleExportsImports t = new CheckApiRegionsBundleExportsImports();

        Feature f = new Feature(ArtifactId.fromMvnId("f:f:1"));
        FeatureDescriptor fd = new FeatureDescriptorImpl(f);

        fdAddBundle(fd, "g:b1:1", "test-bundle1.jar");
        fdAddBundle(fd, "g:b2:1", "test-bundle2.jar");

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getFeature()).thenReturn(f);
        Mockito.when(ctx.getFeatureDescriptor()).thenReturn(fd);
        t.execute(ctx);

        Mockito.verify(ctx, Mockito.never()).reportError(Mockito.anyString());
        Mockito.verify(ctx, Mockito.never()).reportWarning(Mockito.anyString());
    }

    @Test
    /*
     * Bundle b3 imports org.foo.e, but no bundle exports it
     */
    public void testImportExportNoRegionsMissing() throws Exception {
        CheckApiRegionsBundleExportsImports t = new CheckApiRegionsBundleExportsImports();

        Feature f = new Feature(ArtifactId.fromMvnId("f:f:1"));
        FeatureDescriptor fd = new FeatureDescriptorImpl(f);

        fdAddBundle(fd, "g:b1:1", "test-bundle1.jar");
        fdAddBundle(fd, "g:b3:1", "test-bundle3.jar");

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getFeature()).thenReturn(f);
        Mockito.when(ctx.getFeatureDescriptor()).thenReturn(fd);
        t.execute(ctx);

        Mockito.verify(ctx).reportError(Mockito.contains("org.foo.e"));
        Mockito.verify(ctx, Mockito.times(1)).reportError(Mockito.anyString());
        Mockito.verify(ctx, Mockito.never()).reportWarning(Mockito.anyString());
    }

    @Test
    /*
     * Bundle 2 imports org.foo.b from bundle 1, but bundle 1 exports it in a
     * different region, bundle 2 is in no region.
     */
    public void testImportExportWithRegionsMissing() throws Exception {
        String exJson = "[{\"name\": \"something\", \"exports\": [\"org.foo.b\"],\"feature-origins\":[\"f:f:1\"]}]";

        CheckApiRegionsBundleExportsImports t = new CheckApiRegionsBundleExportsImports();

        Feature f = new Feature(ArtifactId.fromMvnId("f:f:1"));
        Extension ex = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        ex.setJSON(exJson);
        f.getExtensions().add(ex);

        FeatureDescriptor fd = new FeatureDescriptorImpl(f);

        fdAddBundle(fd, "g:b1:1", "test-bundle1.jar", f.getId());
        fdAddBundle(fd, "g:b2:1", "test-bundle2.jar", ArtifactId.fromMvnId("f:f2:1"));

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getFeature()).thenReturn(f);
        Mockito.when(ctx.getFeatureDescriptor()).thenReturn(fd);
        t.execute(ctx);

        Mockito.verify(ctx).reportError(Mockito.contains("org.foo.b"));
        Mockito.verify(ctx, Mockito.times(1)).reportError(Mockito.anyString());
        Mockito.verify(ctx, Mockito.never()).reportWarning(Mockito.anyString());
    }

    @Test
    /*
     * Bundle 2 imports org.foo.b from bundle 1, but bundle 1 exports it in a different
     * region, bundle 1 is in something region, and bundle 2 is in somethingelse region.
     */
    public void testImportExportWithRegionMismatch() throws Exception {
        String exJson =
            "["
                + "{\"name\": \"something\", \"exports\": [\"org.foo.b\"],\"feature-origins\":[\"f:f:1\"]},"
                + "{\"name\": \"someotherthing\", \"exports\": [],\"feature-origins\":[\"f:f:1\"]},"
                + "{\"name\": \"somethingelse\", \"exports\": [],\"feature-origins\":[\"f:f2:1\"]}"
            + "]";

        CheckApiRegionsBundleExportsImports t = new CheckApiRegionsBundleExportsImports();

        Feature f = new Feature(ArtifactId.fromMvnId("f:f:1"));
        Extension ex = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        ex.setJSON(exJson);
        f.getExtensions().add(ex);

        FeatureDescriptor fd = new FeatureDescriptorImpl(f);

        fdAddBundle(fd, "g:b1:1", "test-bundle1.jar", fd.getFeature().getId());
        fdAddBundle(fd, "g:b2:1", "test-bundle2.jar", ArtifactId.fromMvnId("f:f2:1"));

        Map<String, String> cfgMap = new HashMap<String, String>();
        cfgMap.put("ignoreAPIRegions", "false");

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getFeature()).thenReturn(f);
        Mockito.when(ctx.getFeatureDescriptor()).thenReturn(fd);
        Mockito.when(ctx.getConfiguration()).thenReturn(cfgMap);
        t.execute(ctx);

        Mockito.verify(ctx).reportError(Mockito.contains("org.foo.b"));
        Mockito.verify(ctx).reportError(Mockito.contains("something"));
        Mockito.verify(ctx).reportError(Mockito.contains("somethingelse"));
        Mockito.verify(ctx, Mockito.times(1)).reportError(Mockito.anyString());
        Mockito.verify(ctx, Mockito.never()).reportWarning(Mockito.anyString());
    }

    @Test
    /*
     * Bundle 2 imports org.foo.b from bundle 1, but bundle 1 exports it in a different
     * region, bundle 1 is in something region, and bundle 2 is in somethingelse region.
     * However this should still pass as the analyzer is configured to ignore regions.
     */
    public void testImportExportWithRegionMismatchIgnoreRegions() throws Exception {
        String exJson =
            "["
                + "{\"name\": \"something\", \"exports\": [\"org.foo.b\"],\"feature-origins\":[\"f:f:1\"]},"
                + "{\"name\": \"someotherthing\", \"exports\": [],\"feature-origins\":[\"f:f:1\"]},"
                + "{\"name\": \"somethingelse\", \"exports\": [],\"feature-origins\":[\"f:f2:1\"]}"
                + "]";

        CheckApiRegionsBundleExportsImports t = new CheckApiRegionsBundleExportsImports();

        Feature f = new Feature(ArtifactId.fromMvnId("f:f:1"));
        Extension ex = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        ex.setJSON(exJson);
        f.getExtensions().add(ex);

        FeatureDescriptor fd = new FeatureDescriptorImpl(f);

        fdAddBundle(fd, "g:b1:1", "test-bundle1.jar", fd.getFeature().getId());
        fdAddBundle(fd, "g:b2:1", "test-bundle2.jar", ArtifactId.fromMvnId("f:f2:1"));

        Map<String, String> cfgMap = new HashMap<String, String>();
        cfgMap.put("ignoreAPIRegions", "true");

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getFeature()).thenReturn(f);
        Mockito.when(ctx.getFeatureDescriptor()).thenReturn(fd);
        Mockito.when(ctx.getConfiguration()).thenReturn(cfgMap);
        t.execute(ctx);

        Mockito.verify(ctx, Mockito.never()).reportError(Mockito.anyString());
        Mockito.verify(ctx, Mockito.never()).reportWarning(Mockito.anyString());
    }

    @Test
    /*
     * Bundle 3 imports org.foo.a from Bundle 1 and org.foo.e from Bundle 4.
     * The Feature is in a region called 'blah' which exports nothing, but because
     * all these bundles are in the same feature they can all see each other.
     */
    public void testImportFromOtherBundleInSameFeature() throws Exception {
        String exJson = "[{\"name\": \"blah\",\"feature-origins\":[\"f:f:2:1\",\"f:f3:1\"]}" +
            ",{\"name\": \"something\",\"feature-origins\":[\"f:f:1\"]}" +
            ",{\"name\": \"someotherthing\",\"feature-origins\":[\"f:f:1\"]}" +
            ",{\"name\": \"abc\",\"feature-origins\":[\"f:2:1\"]}" +
            ",{\"name\": \"xyz\",\"feature-origins\":[\"f:f2:1\"]}" +
            "]"; // no exports

        CheckApiRegionsBundleExportsImports t = new CheckApiRegionsBundleExportsImports();

        Feature f = new Feature(ArtifactId.fromMvnId("f:f:2"));
        Extension ex = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        ex.setJSON(exJson);
        f.getExtensions().add(ex);

        FeatureDescriptor fd = new FeatureDescriptorImpl(f);

        fdAddBundle(fd, "g:b1:1", "test-bundle1.jar", ArtifactId.fromMvnId("f:f3:1"));
        fdAddBundle(fd, "g:b3:1", "test-bundle3.jar", ArtifactId.fromMvnId("f:f3:1"));
        fdAddBundle(fd, "g:b4:1", "test-bundle4.jar", ArtifactId.fromMvnId("f:f3:1"));

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getFeature()).thenReturn(f);
        Mockito.when(ctx.getFeatureDescriptor()).thenReturn(fd);
        t.execute(ctx);

        Mockito.verify(ctx, Mockito.never()).reportError(Mockito.anyString());
        Mockito.verify(ctx, Mockito.never()).reportWarning(Mockito.anyString());
    }

    @Test
    /*
     * Bundle 2 imports org.foo.b from bundle 1. Bundle 1 exports it in the something region
     * and bundle 2 imports it in the something region, so this succeeds.
     */
    public void testImportExportWithMatchingRegion() throws Exception {
        String exJson = "[{\"name\": \"something\", \"exports\": [\"org.foo.b\"],\"feature-origins\":[\"f:f:1\",\"f:f2:1\"]}" +
            ",{\"name\": \"someotherthing\", \"exports\": [\"org.foo.b\"],\"feature-origins\":[\"f:f:1\"]}]";

        CheckApiRegionsBundleExportsImports t = new CheckApiRegionsBundleExportsImports();

        Feature f = new Feature(ArtifactId.fromMvnId("f:f:1"));
        Extension ex = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        ex.setJSON(exJson);
        f.getExtensions().add(ex);

        FeatureDescriptor fd = new FeatureDescriptorImpl(f);

        fdAddBundle(fd, "g:b1:1", "test-bundle1.jar", f.getId());
        fdAddBundle(fd, "g:b2:1", "test-bundle2.jar", ArtifactId.fromMvnId("f:f2:1"));

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getFeature()).thenReturn(f);
        Mockito.when(ctx.getFeatureDescriptor()).thenReturn(fd);
        t.execute(ctx);

        Mockito.verify(ctx, Mockito.never()).reportError(Mockito.anyString());
        Mockito.verify(ctx, Mockito.never()).reportWarning(Mockito.anyString());
    }

    @Test
    /*
     * Bundle 2 imports org.foo.b from bundle 1. Bundle 1 exports it in the global region.
     * Bundle 2 is not explicitly part of the global region, but can still see it
     */
    public void testImportFromGlobalAlwaysSucceeds() throws Exception {
        String exJson = "[{\"name\": \"global\", \"exports\": [\"org.foo.b\"],\"feature-origins\":[\"f:f:1\"]}" +
            ",{\"name\": \"something\", \"exports\": [],\"feature-origins\":[\"f:f2:1\"]}" +
            ",{\"name\": \"lalala\", \"exports\": [],\"feature-origins\":[\"f:f:1\"]}" +
            ",{\"name\": \"someotherthing\", \"exports\": [],\"feature-origins\":[\"f:f:1\"]}]"
            ;

        CheckApiRegionsBundleExportsImports t = new CheckApiRegionsBundleExportsImports();

        Feature f = new Feature(ArtifactId.fromMvnId("f:f:1"));
        Extension ex = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        ex.setJSON(exJson);
        f.getExtensions().add(ex);

        FeatureDescriptor fd = new FeatureDescriptorImpl(f);

        fdAddBundle(fd, "g:b1:1", "test-bundle1.jar",f.getId());
        fdAddBundle(fd, "g:b2:1", "test-bundle2.jar",ArtifactId.fromMvnId("f:f2:1"));

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getFeature()).thenReturn(f);
        Mockito.when(ctx.getFeatureDescriptor()).thenReturn(fd);
        Mockito.when(ctx.getConfiguration()).thenReturn(
                Collections.singletonMap("fileStorage",
                        resourceRoot + "/origins/testImportFromGlobalAlwaysSucceeds"));
        t.execute(ctx);

        Mockito.verify(ctx, Mockito.never()).reportError(Mockito.anyString());
        Mockito.verify(ctx, Mockito.never()).reportWarning(Mockito.anyString());
    }

    @Test
    /*
     * Bundle 2 imports org.foo.b from bundle 1. Bundle 1 exports it in region1.
     * Regions
     */
    public void testImportFromInheritedRegionSucceeds() throws Exception {
        String exJson = "[" +
            "{\"name\": \"region1\", \"exports\": [\"org.foo.b\"],\"feature-origins\":[\"f:f1:1\"]}," +
            "{\"name\": \"region2\", \"exports\": [\"org.foo.c\"],\"feature-origins\":[\"f:f1:1\",\"f:f2:1\"]}," +
            "{\"name\": \"region3\", \"exports\": [],\"feature-origins\":[\"f:f2:1\"]}" +
            "]";

        CheckApiRegionsBundleExportsImports t = new CheckApiRegionsBundleExportsImports();

        Feature f = new Feature(ArtifactId.fromMvnId("f:f:1"));
        Extension ex = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        ex.setJSON(exJson);
        f.getExtensions().add(ex);


        FeatureDescriptor fd = new FeatureDescriptorImpl(f);

        fdAddBundle(fd, "g:b1:1", "test-bundle1.jar", f.getId());
        fdAddBundle(fd, "g:b2:1", "test-bundle2.jar", ArtifactId.fromMvnId("f:f2:1"));

        AnalyserTaskContext ctx = Mockito.mock(AnalyserTaskContext.class);
        Mockito.when(ctx.getFeature()).thenReturn(f);
        Mockito.when(ctx.getFeatureDescriptor()).thenReturn(fd);
        t.execute(ctx);

        Mockito.verify(ctx, Mockito.never()).reportError(Mockito.anyString());
        Mockito.verify(ctx, Mockito.never()).reportWarning(Mockito.anyString());

    }

    private void fdAddBundle(FeatureDescriptor fd, String id, String file, ArtifactId... origins) throws IOException {
        Artifact artifact = new Artifact(ArtifactId.fromMvnId(id));
        artifact.setFeatureOrigins(origins);
        BundleDescriptor bd1 = new BundleDescriptorImpl(
                artifact, new File(resourceRoot, file).toURI().toURL(), 0);
        fd.getBundleDescriptors().add(bd1);
    }
}
