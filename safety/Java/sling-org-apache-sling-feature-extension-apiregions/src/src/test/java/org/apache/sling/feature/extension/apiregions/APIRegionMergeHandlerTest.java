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
package org.apache.sling.feature.extension.apiregions;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonReader;

import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.ExtensionState;
import org.apache.sling.feature.ExtensionType;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.builder.HandlerContext;
import org.apache.sling.feature.extension.apiregions.api.ApiExport;
import org.apache.sling.feature.extension.apiregions.api.ApiRegion;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class APIRegionMergeHandlerTest {
    private Path tempDir;

    @Before
    public void setUp() throws IOException {
        tempDir = Files.createTempDirectory(getClass().getSimpleName());
    }

    @After
    public void tearDown() throws IOException {
        // Delete the temp dir again
        Files.walk(tempDir)
            .sorted(Comparator.reverseOrder())
            .map(Path::toFile)
            .forEach(File::delete);
    }

    @Test
    public void testCanMerge() {
        APIRegionMergeHandler armh = new APIRegionMergeHandler();

        Extension ex = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        assertTrue(armh.canMerge(ex));
        assertFalse(armh.canMerge(new Extension(ExtensionType.JSON, "foo", ExtensionState.OPTIONAL)));
    }

    @Test
    public void testAPIRegionMerging() throws Exception {
        APIRegionMergeHandler armh = new APIRegionMergeHandler();

        Feature tf = new Feature(ArtifactId.fromMvnId("x:t:1"));
        Feature sf = new Feature(ArtifactId.fromMvnId("y:s:2"));

        Extension tgEx = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        tgEx.setJSON("[{\"name\":\"global\","
                + "\"exports\": [\"a.b.c\",\"d.e.f\"]},"
                + "{\"name\":\"internal\","
                + "\"exports\":[\"xyz\"],"
                + "\"some-key\":\"some-val\"}]");

        Extension srEx = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        srEx.setJSON("[{\"name\":\"global\","
                + "\"exports\": [\"test\"]},"
                + "{\"name\":\"something\","
                + "\"exports\": [\"a.ha\"],"
                + "\"my-key\": \"my-val\"}]");

        HandlerContext hc = Mockito.mock(HandlerContext.class);
        armh.merge(hc, tf, sf, tgEx, srEx);

        ApiRegions expected = new ApiRegions();
        ApiRegion global = new ApiRegion("global");
        global.add(new ApiExport("a.b.c"));
        global.add(new ApiExport("d.e.f"));
        global.add(new ApiExport("test"));
        global.setFeatureOrigins(sf.getId());
        expected.add(global);

        ApiRegion internal = new ApiRegion("internal");
        internal.add(new ApiExport("xyz"));
        internal.getProperties().put("some-key", "some-val");
        expected.add(internal);

        ApiRegion something = new ApiRegion("something");
        something.add(new ApiExport("a.ha"));
        something.getProperties().put("my-key", "my-val");
        something.setFeatureOrigins(sf.getId());
        expected.add(something);

        ApiRegions created = ApiRegions.parse((JsonArray) tgEx.getJSONStructure());

        assertEquals(expected, created);
    }


    @Test
    public void testRegionExportsNoInheritance() throws Exception {
        APIRegionMergeHandler armh = new APIRegionMergeHandler();

        Feature tf = new Feature(ArtifactId.fromMvnId("x:t:1"));
        Feature sf = new Feature(ArtifactId.fromMvnId("y:s:2"));

        Extension srEx = new Extension(ExtensionType.JSON, "api-regions", ExtensionState.OPTIONAL);
        srEx.setJSON("[{\"name\":\"global\","
                + "\"exports\": [\"a.b.c\",\"d.e.f\"]},"
                + "{\"name\":\"deprecated\","
                + "\"exports\":[\"klm\",\"#ignored\",\"qrs\"]},"
                + "{\"name\":\"internal\","
                + "\"exports\":[\"xyz\"]},"
                + "{\"name\":\"forbidden\","
                + "\"exports\":[\"abc\",\"klm\"]}]");

        HandlerContext hc = Mockito.mock(HandlerContext.class);
        armh.merge(hc, tf, sf, null, srEx);

        Extension tgEx = tf.getExtensions().iterator().next();

        String expectedJSON = "[{\"name\":\"global\",\"exports\":[\"a.b.c\",\"d.e.f\"],\"feature-origins\":[\"y:s:2\"]},"
                + "{\"name\":\"deprecated\",\"exports\":[\"klm\",\"qrs\"],\"feature-origins\":[\"y:s:2\"]},"
                + "{\"name\":\"internal\",\"exports\":[\"xyz\"],\"feature-origins\":[\"y:s:2\"]},"
                + "{\"name\":\"forbidden\",\"exports\":[\"abc\",\"klm\"],\"feature-origins\":[\"y:s:2\"]}]";
        JsonReader er = Json.createReader(new StringReader(expectedJSON));
        JsonReader ar = Json.createReader(new StringReader(tgEx.getJSON()));
        JsonArray ea = er.readArray();
        JsonArray aa = ar.readArray();

        assertEquals(ea, aa);
    }
}
