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
package org.apache.sling.feature.extension.apiregions.api;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.StringWriter;
import java.io.Writer;

import org.apache.sling.feature.ArtifactId;
import org.junit.Test;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

public class TestApiRegions {

    private String readJSON(final String name) throws IOException {
        try (final Reader reader = new InputStreamReader(
                TestApiRegions.class.getResourceAsStream("/json/" + name + ".json"),
                "UTF-8"); final Writer writer = new StringWriter()) {
            int l;
            char[] buf = new char[2048];
            while ((l = reader.read(buf)) > -1) {
                writer.write(buf, 0, l);
            }

            return writer.toString();
        }
    }

    @Test
    public void testParsing() throws Exception {
        final String json = readJSON("apis");

        final ApiRegions regions = ApiRegions.parse(json);
        assertNotNull(regions);

        assertEquals(2, regions.listRegions().size());

        final ApiRegion global = regions.listRegions().get(0);
        assertEquals("global", global.getName());

        assertEquals(2, global.listExports().size());

        final ApiRegion internal = regions.listRegions().get(1);
        assertEquals("internal", internal.getName());

        assertEquals(1, internal.listExports().size());
    }

    @Test
    public void testOrdering() throws Exception {
        final ApiRegions regions = new ApiRegions();
        final ApiRegion one = new ApiRegion("one");
        one.setFeatureOrigins(ArtifactId.fromMvnId("f:f1:1"));
        final ApiRegion two = new ApiRegion("two");
        two.setFeatureOrigins(ArtifactId.fromMvnId("f:f1:1"));
        final ApiRegion three = new ApiRegion("three");
        three.setFeatureOrigins(ArtifactId.fromMvnId("f:f1:1"));

        final ApiRegion duplicate = new ApiRegion("two");
        duplicate.setFeatureOrigins(ArtifactId.fromMvnId("f:f1:1"));
        final ApiRegion other = new ApiRegion("other");
        other.setFeatureOrigins(ArtifactId.fromMvnId("f:f1:1"));

        assertTrue(regions.add(one));
        assertTrue(regions.add(two));
        assertTrue(regions.add(three));

        assertFalse(regions.add(duplicate));

        assertEquals(3, regions.listRegions().size());

        assertNull(one.getParent());
        assertEquals(one, two.getParent());
        assertEquals(two, three.getParent());
    }

    @Test
    public void testExports() throws Exception {
        final ApiRegions regions = new ApiRegions();

        final ApiRegion one = new ApiRegion("one");
        one.setFeatureOrigins(ArtifactId.fromMvnId("f:f1:1"));
        one.add(new ApiExport("a"));

        final ApiRegion two = new ApiRegion("two");
        two.setFeatureOrigins(ArtifactId.fromMvnId("f:f1:1"));
        two.add(new ApiExport("b"));

        final ApiRegion three = new ApiRegion("three");
        three.setFeatureOrigins(ArtifactId.fromMvnId("f:f1:1"));
        three.add(new ApiExport("c"));

        assertTrue(regions.add(one));
        assertTrue(regions.add(two));
        assertTrue(regions.add(three));

        assertEquals(1, one.listAllExports().size());
        assertTrue(one.listAllExports().contains(new ApiExport("a")));
        assertEquals(1, one.listExports().size());
        assertTrue(one.listExports().contains(new ApiExport("a")));

        assertEquals(2, two.listAllExports().size());
        assertTrue(two.listAllExports().contains(new ApiExport("a")));
        assertTrue(two.listAllExports().contains(new ApiExport("b")));
        assertEquals(1, two.listExports().size());
        assertTrue(two.listExports().contains(new ApiExport("b")));

        assertEquals(3, three.listAllExports().size());
        assertTrue(three.listAllExports().contains(new ApiExport("a")));
        assertTrue(three.listAllExports().contains(new ApiExport("b")));
        assertTrue(three.listAllExports().contains(new ApiExport("c")));
        assertEquals(1, three.listExports().size());
        assertTrue(three.listExports().contains(new ApiExport("c")));
    }
}
