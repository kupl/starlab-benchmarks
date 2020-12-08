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

import org.apache.felix.ipojo.Factory;
import org.apache.felix.ipojo.runtime.core.services.FooService;
import org.junit.Before;
import org.junit.Test;
import org.osgi.framework.ServiceReference;
import org.osgi.service.cm.Configuration;
import org.osgi.service.cm.ConfigurationAdmin;

import java.io.IOException;
import java.util.Dictionary;
import java.util.Hashtable;
import java.util.Properties;

import static junit.framework.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;

public class TestStaticPropsReconfiguration extends Common {

    @Before
    public void setUp() {
        String type = "PS-FooProviderType-2";
        ipojoHelper.createComponentInstance(type, "FooProvider-1");

        Properties p2 = new Properties();
        p2.put("instance.name", "FooProvider-2");
        p2.put("int", 4);
        p2.put("long", (long) 42);
        p2.put("string", "bar");
        p2.put("strAProp", new String[]{"bar", "foo"});
        p2.put("intAProp", new int[]{1, 2, 3});
        ipojoHelper.createComponentInstance(type, p2);
    }

    @Test
    public void testReconfFactory1() {
        ServiceReference sr = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), "FooProvider-1");
        assertNotNull("Check the availability of the FS service", sr);

        // Check service properties
        Integer intProp = (Integer) sr.getProperty("int");
        Long longProp = (Long) sr.getProperty("long");
        String strProp = (String) sr.getProperty("string");
        String[] strAProp = (String[]) sr.getProperty("strAProp");
        int[] intAProp = (int[]) sr.getProperty("intAProp");

        assertEquals("Check intProp equality", intProp, new Integer(2));
        assertEquals("Check longProp equality", longProp, new Long(40));
        assertEquals("Check strProp equality", strProp, new String("foo"));
        assertNotNull("Check strAProp not nullity", strAProp);
        String[] v = new String[]{"foo", "bar"};
        for (int i = 0; i < strAProp.length; i++) {
            if (!strAProp[i].equals(v[i])) {
                fail("Check the strAProp Equality");
            }
        }
        assertNotNull("Check intAProp not nullity", intAProp);
        int[] v2 = new int[]{1, 2, 3};
        for (int i = 0; i < intAProp.length; i++) {
            if (intAProp[i] != v2[i]) {
                fail("Check the intAProp Equality");
            }
        }

        // Reconfiguration
        ServiceReference fact_ref = ipojoHelper.getServiceReferenceByName(Factory.class.getName(), "PS-FooProviderType-2");
        Dictionary reconf = new Properties();
        reconf.put("instance.name", "FooProvider-1");
        reconf.put("int", new Integer(5));
        reconf.put("long", new Long(43));
        reconf.put("string", new String("toto"));
        reconf.put("strAProp", new String[]{"foo", "baz"});
        reconf.put("intAProp", new int[]{3, 2, 1});
        Factory fact = (Factory) osgiHelper.getRawServiceObject(fact_ref);
        try {
            fact.reconfigure(reconf);
        } catch (Exception e) {
            fail("Configuration non acceptable : " + reconf);
        }

        sr = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), "FooProvider-1");
        assertNotNull("Check the availability of the FS service", sr);

        // Check service properties after the reconfiguration
        intProp = (Integer) sr.getProperty("int");
        longProp = (Long) sr.getProperty("long");
        strProp = (String) sr.getProperty("string");
        strAProp = (String[]) sr.getProperty("strAProp");
        intAProp = (int[]) sr.getProperty("intAProp");

        assertEquals("Check intProp equality after reconfiguration", intProp, new Integer(5));
        assertEquals("Check longProp equality after reconfiguration", longProp, new Long(43));
        assertEquals("Check strProp equality after reconfiguration", strProp, new String("toto"));
        assertNotNull("Check strAProp not nullity after reconfiguration", strAProp);
        v = new String[]{"foo", "baz"};
        for (int i = 0; i < strAProp.length; i++) {
            if (!strAProp[i].equals(v[i])) {
                fail("Check the strAProp Equality");
            }
        }
        assertNotNull("Check intAProp not nullity", intAProp);
        v2 = new int[]{3, 2, 1};
        for (int i = 0; i < intAProp.length; i++) {
            if (intAProp[i] != v2[i]) {
                fail("Check the intAProp Equality");
            }
        }

    }

    @Test
    public void testReconfFactory2() {
        ServiceReference sr = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), "FooProvider-2");
        assertNotNull("Check the availability of the FS service", sr);

        // Check service properties
        Integer intProp = (Integer) sr.getProperty("int");
        Long longProp = (Long) sr.getProperty("long");
        String strProp = (String) sr.getProperty("string");
        String[] strAProp = (String[]) sr.getProperty("strAProp");
        int[] intAProp = (int[]) sr.getProperty("intAProp");

        assertEquals("Check intProp equality", intProp, new Integer(4));
        assertEquals("Check longProp equality", longProp, new Long(42));
        assertEquals("Check strProp equality", strProp, "bar");

        assertNotNull("Check strAProp not nullity", strAProp);
        String[] v = new String[]{"bar", "foo"};
        for (int i = 0; i < strAProp.length; i++) {
            if (!strAProp[i].equals(v[i])) {
                fail("Check the strAProp Equality");
            }
        }
        assertNotNull("Check intAProp not nullity", intAProp);
        int[] v2 = new int[]{1, 2, 3};
        for (int i = 0; i < intAProp.length; i++) {
            if (intAProp[i] != v2[i]) {
                fail("Check the intAProp Equality");
            }
        }

        // Reconfiguration
        ServiceReference fact_ref = ipojoHelper.getServiceReferenceByName(Factory.class.getName(), "PS-FooProviderType-2");
        Properties reconf = new Properties();
        reconf.put("instance.name", "FooProvider-2");
        reconf.put("int", 5);
        reconf.put("long", (long) 43);
        reconf.put("string", "toto");
        reconf.put("strAProp", new String[]{"foo", "baz"});
        reconf.put("intAProp", new int[]{3, 2, 1});
        Factory fact = (Factory) osgiHelper.getRawServiceObject(fact_ref);
        try {
            fact.reconfigure(reconf);
        } catch (Exception e) {
            fail("Configuration non acceptable : " + reconf);
        }

        // Check service properties after the reconfiguration
        intProp = (Integer) sr.getProperty("int");
        longProp = (Long) sr.getProperty("long");
        strProp = (String) sr.getProperty("string");
        strAProp = (String[]) sr.getProperty("strAProp");
        intAProp = (int[]) sr.getProperty("intAProp");

        assertEquals("Check intProp equality after reconfiguration", intProp, new Integer(5));
        assertEquals("Check longProp equality after reconfiguration", longProp, new Long(43));
        assertEquals("Check strProp equality after reconfiguration", strProp, "toto");
        assertNotNull("Check strAProp not nullity after reconfiguration", strAProp);
        v = new String[]{"foo", "baz"};
        for (int i = 0; i < strAProp.length; i++) {
            if (!strAProp[i].equals(v[i])) {
                fail("Check the strAProp Equality");
            }
        }
        assertNotNull("Check intAProp not nullity", intAProp);
        v2 = new int[]{3, 2, 1};
        for (int i = 0; i < intAProp.length; i++) {
            if (intAProp[i] != v2[i]) {
                fail("Check the intAProp Equality");
            }
        }
    }

    @Test
    public void testMSFFactory1() throws IOException, InterruptedException {
        ServiceReference sr = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), "FooProvider-1");
        assertNotNull("Check the availability of the FS service", sr);

        // Check service properties
        Integer intProp = (Integer) sr.getProperty("int");
        Long longProp = (Long) sr.getProperty("long");
        String strProp = (String) sr.getProperty("string");
        String[] strAProp = (String[]) sr.getProperty("strAProp");
        int[] intAProp = (int[]) sr.getProperty("intAProp");

        assertEquals("Check intProp equality", intProp, new Integer(2));
        assertEquals("Check longProp equality", longProp, new Long(40));
        assertEquals("Check strProp equality", strProp, "foo");
        assertNotNull("Check strAProp not nullity", strAProp);
        String[] v = new String[]{"foo", "bar"};
        for (int i = 0; i < strAProp.length; i++) {
            if (!strAProp[i].equals(v[i])) {
                fail("Check the strAProp Equality");
            }
        }
        assertNotNull("Check intAProp not nullity", intAProp);
        int[] v2 = new int[]{1, 2, 3};
        for (int i = 0; i < intAProp.length; i++) {
            if (intAProp[i] != v2[i]) {
                fail("Check the intAProp Equality");
            }
        }

        // Reconfiguration
        ConfigurationAdmin admin = osgiHelper.getServiceObject(ConfigurationAdmin.class);
        Configuration configuration = admin.getConfiguration("FooProvider-1", "?");

        Dictionary<String, Object> reconf = new Hashtable<String, Object>();
        reconf.put("int", 5);
        reconf.put("long", (long) 43);
        reconf.put("string", "toto");
        reconf.put("strAProp", new String[]{"foo", "baz"});
        reconf.put("intAProp", new int[]{3, 2, 1});

        configuration.update(reconf);
        Thread.sleep(200);

        sr = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), "FooProvider-1");
        assertNotNull("Check the availability of the FS service", sr);

        // Check service properties after the reconfiguration
        intProp = (Integer) sr.getProperty("int");
        longProp = (Long) sr.getProperty("long");
        strProp = (String) sr.getProperty("string");
        strAProp = (String[]) sr.getProperty("strAProp");
        intAProp = (int[]) sr.getProperty("intAProp");

        assertEquals("Check intProp equality after reconfiguration", intProp, new Integer(5));
        assertEquals("Check longProp equality after reconfiguration", longProp, new Long(43));
        assertEquals("Check strProp equality after reconfiguration", strProp, "toto");
        assertNotNull("Check strAProp not nullity after reconfiguration", strAProp);
        v = new String[]{"foo", "baz"};
        for (int i = 0; i < strAProp.length; i++) {
            if (!strAProp[i].equals(v[i])) {
                fail("Check the strAProp Equality");
            }
        }
        assertNotNull("Check intAProp not nullity", intAProp);
        v2 = new int[]{3, 2, 1};
        for (int i = 0; i < intAProp.length; i++) {
            if (intAProp[i] != v2[i]) {
                fail("Check the intAProp Equality");
            }
        }
    }

    @Test
    public void testReconfMSF2() throws IOException, InterruptedException {
        ServiceReference sr = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), "FooProvider-2");
        assertNotNull("Check the availability of the FS service", sr);

        // Check service properties
        Integer intProp = (Integer) sr.getProperty("int");
        Long longProp = (Long) sr.getProperty("long");
        String strProp = (String) sr.getProperty("string");
        String[] strAProp = (String[]) sr.getProperty("strAProp");
        int[] intAProp = (int[]) sr.getProperty("intAProp");

        assertEquals("Check intProp equality", intProp, new Integer(4));
        assertEquals("Check longProp equality", longProp, new Long(42));
        assertEquals("Check strProp equality", strProp, "bar");

        assertNotNull("Check strAProp not nullity", strAProp);
        String[] v = new String[]{"bar", "foo"};
        for (int i = 0; i < strAProp.length; i++) {
            if (!strAProp[i].equals(v[i])) {
                fail("Check the strAProp Equality");
            }
        }
        assertNotNull("Check intAProp not nullity", intAProp);
        int[] v2 = new int[]{1, 2, 3};
        for (int i = 0; i < intAProp.length; i++) {
            if (intAProp[i] != v2[i]) {
                fail("Check the intAProp Equality");
            }
        }

        ConfigurationAdmin admin = osgiHelper.getServiceObject(ConfigurationAdmin.class);
        Configuration configuration = admin.getConfiguration("FooProvider-2", "?");

        // Reconfiguration
        Dictionary<String, Object> reconf = new Hashtable<String, Object>();
        reconf.put("int", 5);
        reconf.put("long", (long) 43);
        reconf.put("string", "toto");
        reconf.put("strAProp", new String[]{"foo", "baz"});
        reconf.put("intAProp", new int[]{3, 2, 1});

        configuration.update(reconf);
        Thread.sleep(200);

        // Check service properties after the reconfiguration
        intProp = (Integer) sr.getProperty("int");
        longProp = (Long) sr.getProperty("long");
        strProp = (String) sr.getProperty("string");
        strAProp = (String[]) sr.getProperty("strAProp");
        intAProp = (int[]) sr.getProperty("intAProp");

        assertEquals("Check intProp equality after reconfiguration", intProp, new Integer(5));
        assertEquals("Check longProp equality after reconfiguration", longProp, new Long(43));
        assertEquals("Check strProp equality after reconfiguration", strProp, "toto");
        assertNotNull("Check strAProp not nullity after reconfiguration", strAProp);
        v = new String[]{"foo", "baz"};
        for (int i = 0; i < strAProp.length; i++) {
            if (!strAProp[i].equals(v[i])) {
                fail("Check the strAProp Equality");
            }
        }
        assertNotNull("Check intAProp not nullity", intAProp);
        v2 = new int[]{3, 2, 1};
        for (int i = 0; i < intAProp.length; i++) {
            if (intAProp[i] != v2[i]) {
                fail("Check the intAProp Equality");
            }
        }
    }


}
