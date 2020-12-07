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

import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;

public class ApisJarMojoTest {
    @Test
    public void testGuessClassifier() {
        ApisJarMojo ajm = new ApisJarMojo();

        assertNull(ajm.inferClassifier(null, null, null));
        assertNull(ajm.inferClassifier("foo-123.jar", "foo", "123"));
        assertNull(ajm.inferClassifier("foo-1234.jar", "foo", "123"));
        assertNull(ajm.inferClassifier("foo-12345.jar", "foo", "123"));
        assertEquals("abc", ajm.inferClassifier("foo-123-abc", "foo", "123"));
        assertNull(ajm.inferClassifier("blah/blah/foo-123.jar", "foo", "123"));
        assertEquals("bar", ajm.inferClassifier("foo-123-bar.jar", "foo", "123"));
        assertEquals("bar", ajm.inferClassifier("blah/blah/foo-123-bar.jar", "foo", "123"));
        assertNull(ajm.inferClassifier("oof-123-bar.jar", "foo", "123"));
        assertNull(ajm.inferClassifier("foo-345-bar.jar", "foo", "123"));
    }
}
