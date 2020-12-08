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

package org.apache.felix.ipojo.runtime.core;

import org.apache.felix.ipojo.ComponentInstance;
import org.apache.felix.ipojo.Factory;
import org.apache.felix.ipojo.Pojo;
import org.apache.felix.ipojo.PrimitiveInstanceDescription;
import org.apache.felix.ipojo.runtime.core.services.FooService;
import org.junit.Test;
import org.osgi.framework.ServiceReference;
import org.ow2.chameleon.testing.helpers.BaseTest;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.List;
import java.util.Properties;

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertNotNull;
import static org.junit.Assert.*;

/**
 * Check the getComponentInstance method on a POJO
 */
public class TestGetComponentInstance extends BaseTest {

    /**
     * Check if a pojo can correctly be cast in POJO.
     * Check the getComponentInstance method.
     */
    @Test
    public void testGetComponentInstance() throws NoSuchFieldException, IllegalAccessException, NoSuchMethodException {
        String factName = "Manipulation-FooProviderType-1";
        String compName = "FooProvider-1";
        ServiceReference ref = null;

        // Get the factory to create a component instance
        Factory fact = ipojoHelper.getFactory(factName);
        assertNotNull("Cannot find the factory FooProvider-1", fact);

        Properties props = new Properties();
        props.put("instance.name", compName);
        ComponentInstance ci = null;
        try {
            ci = fact.createComponentInstance(props);
        } catch (Exception e1) {
            fail(e1.getMessage());
        }

        assertEquals("Check instance name", compName, ci.getInstanceName());

        // Get a FooService provider
        ref = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), compName);

        assertNotNull("FS not available", ref);

        // Get foo object
        FooService fs = (FooService) osgiHelper.getServiceObject(ref);

        // Cast to POJO
        Pojo pojo = (Pojo) fs;
        Field im = fs.getClass().getDeclaredField("__IM");
        assertNotNull(im);
        im.setAccessible(true);
        assertNotNull(im.get(fs));

        Method method = fs.getClass().getMethod("getComponentInstance");
        assertNotNull(method);

        // GetComponentInstance
        ComponentInstance instance = pojo.getComponentInstance();
        assertNotNull(instance);
        assertEquals("Check component instance name", instance.getInstanceName(), compName);
        assertEquals("Check component factory name", instance.getFactory().getName(), factName);
        assertNotNull("Instance description not null", instance.getInstanceDescription());
        PrimitiveInstanceDescription id = (PrimitiveInstanceDescription) instance.getInstanceDescription();
        assertTrue("Check instance state", id.getState() == ComponentInstance.VALID);
        assertEquals("Check created pojo count", id.getCreatedObjects().length, 1);
        assertEquals("Check instance description name", id.getName(), compName);

        ci.dispose();

        // Check that there is no more FooService
        ref = osgiHelper.getServiceReference(FooService.class.getName());
        assertNull("FS available, but component instance stopped", ref);
    }

    @Override
    protected List<String> getExtraExports() {
        return Arrays.asList("org.apache.felix.ipojo.runtime.core.components");
    }

}
