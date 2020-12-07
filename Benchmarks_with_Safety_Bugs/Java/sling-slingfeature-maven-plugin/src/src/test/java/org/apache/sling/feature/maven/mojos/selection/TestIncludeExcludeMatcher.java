/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. The ASF
 * licenses this file to You under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package org.apache.sling.feature.maven.mojos.selection;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.fail;

import java.util.Arrays;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.sling.feature.ArtifactId;
import org.junit.Test;

public class TestIncludeExcludeMatcher {

    @Test public void testLicenseDefaultsMatcher() throws MojoExecutionException {
        IncludeExcludeMatcher m = new IncludeExcludeMatcher(Arrays.asList("org.apache.sling=ASL2", "org.apache="),
                null, "=", true);
        assertEquals("ASL2", m.matches(ArtifactId.parse("org.apache.sling:artifact:1.0")));
        assertEquals("", m.matches(ArtifactId.parse("org.apache:artifact:1.0")));
        assertNull(m.matches(ArtifactId.parse("org.apache.felix:artifact:1.0")));

        try {
            new IncludeExcludeMatcher(Arrays.asList("org.apache.sling"),
                    null, "=", true);
            fail();
        } catch ( MojoExecutionException e) {
            // expected
        }
    }

    @Test public void testInfoMatcher() throws MojoExecutionException {
        IncludeExcludeMatcher m = new IncludeExcludeMatcher(Arrays.asList("org.apache.sling"),
                Arrays.asList("org.apache.sling:api"), null, false);
        assertEquals("", m.matches(ArtifactId.parse("org.apache.sling:artifact:1.0")));
        assertNull(m.matches(ArtifactId.parse("org.apache.sling:api:1.0")));
    }
}
