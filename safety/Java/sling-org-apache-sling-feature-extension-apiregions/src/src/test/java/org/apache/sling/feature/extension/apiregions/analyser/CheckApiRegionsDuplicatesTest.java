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

import java.util.List;

import org.junit.Test;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class CheckApiRegionsDuplicatesTest extends AbstractApiRegionsAnalyserTaskTest<CheckApiRegionsDuplicates> {

    @Override
    protected CheckApiRegionsDuplicates newTask() {
        return new CheckApiRegionsDuplicates();
    }

    @Test
    public void testDetectDuplicates() throws Exception {
        List<String> errors = execute("[{\"name\": \"global\",\"exports\": [\"org.osgi.util.function\"]},{\"name\": \"deprecated\",\"exports\": [\"org.osgi.util.function\"]}]");

        assertFalse(errors.isEmpty());
        assertEquals(1, errors.size());
        System.out.println(errors);
        assertTrue(errors.iterator()
                .next()
                .startsWith("Regions 'global' and 'deprecated' defined in feature 'org.apache.sling.testing:org.apache.sling.testing.apiregions:1.0.0' declare both 1 package(s)"));
    }

}
