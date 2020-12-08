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

package org.apache.felix.ipojo.runtime.core.test.annotations;

import org.apache.felix.ipojo.metadata.Element;
import org.junit.Test;

import static junit.framework.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;

public class TestFactory extends Common {


    @Test
    public void testArchDeprecated() {
        Element meta = ipojoHelper.getMetadata(getTestBundle(),  "org.apache.felix.ipojo.runtime.core.test.components.FactoryDeprecated");
        String fact = meta.getAttribute("public");
        String name = meta.getAttribute("name");
        assertNotNull("Factory exists ", fact);
        assertEquals("Factory value", "true", fact);
        assertNotNull("Name exists ", name);
        assertEquals("Name value", "org.apache.felix.ipojo.runtime.core.test.components.FactoryDeprecated", name);
    }

    @Test
    public void testArch() {
        Element meta = ipojoHelper.getMetadata(getTestBundle(),  "org.apache.felix.ipojo.runtime.core.test.components.Factory");
        String fact = meta.getAttribute("public");
        String name = meta.getAttribute("name");
        assertNotNull("Factory exists ", fact);
        assertEquals("Factory value", "true", fact);
        assertNotNull("Name exists ", name);
        assertEquals("Name value", "factory", name);
    }

    @Test
    public void testNoArch() {
        Element meta = ipojoHelper.getMetadata(getTestBundle(),  "org.apache.felix.ipojo.runtime.core.test.components.NoFactory");
        String fact = meta.getAttribute("public");
        String name = meta.getAttribute("name");
        assertNotNull("Factory exists ", fact);
        assertEquals("Factory value", "false", fact);
        assertNotNull("Name exists ", name);
        assertEquals("Name value", "nofactory", name);
    }

    @Test
    public void testFactoryMethod() {
        Element meta = ipojoHelper.getMetadata(getTestBundle(),  "org.apache.felix.ipojo.runtime.core.test.components.FactoryMethod");
        String method = meta.getAttribute("factory-method");
        assertNotNull("Method exists ", method);
        assertEquals("Method value", "create", method);
    }

    @Test
    public void testFactoryMethodDeprecated() {
        Element meta = ipojoHelper.getMetadata(getTestBundle(),  "org.apache.felix.ipojo.runtime.core.test.components.FactoryMethodDeprecated");
        String method = meta.getAttribute("factory-method");
        assertNotNull("Method exists ", method);
        assertEquals("Method value", "create", method);
    }

    @Test
    public void testVersion() {
        Element meta = ipojoHelper.getMetadata(getTestBundle(),  "org.apache.felix.ipojo.runtime.core.test.components.ComponentTypeVersion");
        String version = meta.getAttribute("version");
        assertNotNull("Version exist", version);
        assertEquals("Version value", "1.0.0", version);
    }

    @Test
    public void testNoVersion() {
        Element meta = ipojoHelper.getMetadata(getTestBundle(),  "org.apache.felix.ipojo.runtime.core.test.components.FactoryMethod");
        String version = meta.getAttribute("version");
        assertNull("No Version", version);
    }


}

