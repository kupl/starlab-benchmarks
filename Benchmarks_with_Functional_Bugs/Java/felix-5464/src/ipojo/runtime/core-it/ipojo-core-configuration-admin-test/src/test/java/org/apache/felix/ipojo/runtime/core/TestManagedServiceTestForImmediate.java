/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.apache.felix.ipojo.runtime.core;

import org.apache.felix.ipojo.ComponentFactory;
import org.apache.felix.ipojo.ComponentInstance;
import org.apache.felix.ipojo.PrimitiveInstanceDescription;
import org.apache.felix.ipojo.architecture.Architecture;
import org.apache.felix.ipojo.runtime.core.services.FooService;
import org.junit.Before;
import org.junit.Test;
import org.osgi.framework.ServiceReference;
import org.osgi.service.cm.Configuration;
import org.osgi.service.cm.ConfigurationAdmin;

import java.io.IOException;
import java.util.Dictionary;
import java.util.Properties;

import static junit.framework.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;

public class TestManagedServiceTestForImmediate extends Common {

    private String factNameImm = "CA-ImmConfigurableProvider";
    private String msp = "foo";

    private ComponentFactory factImm;

    @Before
    public void setUp() throws InterruptedException {
        factImm = (ComponentFactory) ipojoHelper.getFactory(factNameImm);
    }

    @Test
    public void testFactoryCreationAndReconfigurationUsingManagedService() {
        Properties props = new Properties();
        props.put("managed.service.pid", msp); // Exposes a ManagedService.
        props.put("message", "message");
        ComponentInstance instance = null;
        try {
            instance = factImm.createComponentInstance(props);
        } catch (Exception e) {
            e.printStackTrace();
            fail(e.getMessage());
        }

        ServiceReference ref = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), instance.getInstanceName());
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertNotNull("FS availability", ref);

        FooService fs = (FooService) bc.getService(ref);
        Properties p = fs.fooProps();
        String mes = p.getProperty("message");
        int count = (Integer) p.get("count");
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertEquals("Check message", "message", mes);
        assertEquals("Check count", 1, count);

        //Update
        Configuration configuration;
        try {
            configuration = admin.getConfiguration(msp, getTestBundle().getLocation());
            Dictionary prc = configuration.getProperties();
            if (prc == null) {
                prc = new Properties();
            }
            prc.put("message", "message2");
            configuration.update(prc);
            System.err.println("updated ? ");
            grace();
        } catch (Exception e) {
            fail(e.getMessage());
        }

        ref = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), instance.getInstanceName());
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertNotNull("FS availability", ref);

        fs = (FooService) bc.getService(ref);
        p = fs.fooProps();
        mes = p.getProperty("message");
        count = (Integer) p.get("count");
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertEquals("Check message", "message2", mes);
        assertEquals("Check count", 2, count);

        instance.dispose();
    }

    /**
     * Creates an instance using a factory configuration.
     * The configuration specifies the managed.service.pid property
     * Reconfiguration is applied using the exposed managed service.
     */
    @Test
    public void testCreationUsingFactoryConfigurationSettingTheManagedServicePid() {
        Configuration conf = null;
        try {
            conf = admin.createFactoryConfiguration(factNameImm, getTestBundle().getLocation());
            Dictionary props = conf.getProperties();
            if (props == null) {
                props = new Properties();
            }
            props.put("managed.service.pid", msp);
            props.put("message", "message");
            conf.update(props);
            grace();
        } catch (Exception e) {
            fail(e.getMessage());
        }


        Architecture arch = (Architecture) osgiHelper.getServiceObject(org.apache.felix.ipojo.architecture.Architecture.class.getName(), "(architecture.instance=" + conf.getPid() + ")");

        ServiceReference ref = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), conf.getPid());
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) arch.getInstanceDescription()).getCreatedObjects().length);
        assertNotNull("FS availability", ref);

        // arch = (Architecture) osgiHelper.getServiceObject( org.apache.felix.ipojo.architecture.Architecture.class.getName(), "(architecture.instance=" + conf.getPid() + ")");
        FooService fs = (FooService) bc.getService(ref);
        Properties p = fs.fooProps();
        String mes = p.getProperty("message");
        int count = ((Integer) p.get("count")).intValue();
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) arch.getInstanceDescription()).getCreatedObjects().length);
        assertEquals("Check message", "message", mes);
        assertEquals("Check count", 1, count);

        //Update
        Configuration configuration;
        try {
            configuration = admin.getConfiguration(msp, getTestBundle().getLocation());
            Dictionary prc = configuration.getProperties();
            if (prc == null) {
                prc = new Properties();
            }
            prc.put("message", "message2");
            configuration.update(prc);
            grace();
        } catch (Exception e) {
            fail(e.getMessage());
        }

        //  arch = (Architecture) osgiHelper.getServiceObject( org.apache.felix.ipojo.architecture.Architecture.class.getName(), "(architecture.instance=" + conf.getPid() + ")");
        ref = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), conf.getPid());
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) arch.getInstanceDescription()).getCreatedObjects().length);
        assertNotNull("FS availability", ref);

        // arch = (Architecture) osgiHelper.getServiceObject( org.apache.felix.ipojo.architecture.Architecture.class.getName(), "(architecture.instance=" + conf.getPid() + ")");
        fs = (FooService) bc.getService(ref);
        p = fs.fooProps();
        mes = p.getProperty("message");
        count = ((Integer) p.get("count")).intValue();
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) arch.getInstanceDescription()).getCreatedObjects().length);
        if (mes.equals("message")) {
            System.out.println("Warning, configuration not yet applied");
            assertEquals("Check count - W", 1, count);
        } else {
            assertEquals("Check message", "message2", mes);
            assertEquals("Check count", 2, count);
        }

        try {
            conf.delete();
            grace();
        } catch (IOException e) {
            fail(e.getMessage());
        }

    }

    @Test
    public void testCreationAndReconfiguration2() {
        // The configuration exists before the instance creation.

        //Update
        Configuration configuration = null;
        try {
            configuration = admin.getConfiguration(msp, getTestBundle().getLocation());
            Dictionary prc = configuration.getProperties();
            if (prc == null) {
                prc = new Properties();
            }
            prc.put("message", "message2");
            configuration.update(prc);
            grace();
        } catch (Exception e) {
            fail(e.getMessage());
        }

        Properties props = new Properties();
        props.put("managed.service.pid", msp);
        props.put("message", "message");
        ComponentInstance instance = null;
        try {
            instance = factImm.createComponentInstance(props);
            grace();
        } catch (Exception e) {
            fail(e.getMessage());
        }

        ServiceReference ref = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), instance.getInstanceName());
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertNotNull("FS availability", ref);

        FooService fs = (FooService) bc.getService(ref);
        Properties p = fs.fooProps();
        String mes = p.getProperty("message");
        //  int count1 = ((Integer) p.get("count")).intValue();
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertEquals("Check message - 1 (" + mes + ")", "message2", mes); // Already reconfigured.
        // assertEquals("Check count", 2, count); // Two : 1) "message" on immediate, "message2" on the reconfiguration,
        // not necessary as the property can be set before the immediate instance creation

        instance.dispose();

        //Reconfiguration
        try {
            configuration = admin.getConfiguration(msp, getTestBundle().getLocation());
            Dictionary prc = configuration.getProperties();
            if (prc == null) {
                prc = new Properties();
            }
            prc.put("message", "message3");
            configuration.update(prc);
            grace();
        } catch (Exception e) {
            fail(e.getMessage());
        }

        // Recreation of the instance.
        props = new Properties();
        props.put("managed.service.pid", msp);
        props.put("message", "message");
        instance = null;
        try {
            instance = factImm.createComponentInstance(props);
            grace();
        } catch (Exception e) {
            fail(e.getMessage());
        }
        ref = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), instance.getInstanceName());
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertNotNull("FS availability", ref);

        fs = (FooService) bc.getService(ref);
        p = fs.fooProps();
        mes = p.getProperty("message");
        // int count = ((Integer) p.get("count")).intValue();
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertEquals("Check message already reconfigured", "message3", mes); // Already reconfigured.
        //assertEquals("Check count", count1 + 1, count); // message before the reconfiguration, message3 after the reconfiguration

        instance.dispose();


    }

    @Test
    public void testCreationAndReconfiguration3() {
        // The configuration exists before the instance creation.

        //Update
        Configuration configuration;
        try {
            configuration = admin.getConfiguration(msp, getTestBundle().getLocation());
            Dictionary prc = configuration.getProperties();
            if (prc == null) {
                prc = new Properties();
            }
            prc.put("message", "message2");
            configuration.update(prc);
            grace();
        } catch (Exception e) {
            e.printStackTrace();
            fail(e.getMessage());
        }

        Properties props = new Properties();
        props.put("managed.service.pid", msp);
        props.put("message", "message");
        ComponentInstance instance = null;
        try {
            instance = factImm.createComponentInstance(props);
            grace();
        } catch (Exception e) {
            fail(e.getMessage());
        }

        ServiceReference ref = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), instance.getInstanceName());
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertNotNull("FS availability", ref);

        FooService fs = (FooService) bc.getService(ref);
        Properties p = fs.fooProps();
        String mes = p.getProperty("message");
        // int count = ((Integer) p.get("count")).intValue();
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertEquals("Check message", "message2", mes); // Already reconfigured.
        //assertEquals("Check count", 1, count);

        //Reconfiguration
        try {
            configuration = admin.getConfiguration(msp);
            Dictionary prc = configuration.getProperties();
            if (prc == null) {
                prc = new Properties();
            }
            prc.put("message", "message3");
            configuration.update(prc);
            grace();
        } catch (Exception e) {
            fail(e.getMessage());
        }

        instance.dispose();

        // Recreation of the instance.
        props = new Properties();
        props.put("managed.service.pid", msp);
        props.put("message", "message");
        instance = null;
        try {
            instance = factImm.createComponentInstance(props);
        } catch (Exception e) {
            fail(e.getMessage());
        }

        // Wait until the configuration is delivered.
        grace();

        ref = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), instance.getInstanceName());
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertNotNull("FS availability", ref);

        fs = (FooService) bc.getService(ref);
        p = fs.fooProps();
        mes = p.getProperty("message");
        //  count = ((Integer) p.get("count")).intValue();
        assertEquals("Check 1 object", 1, ((PrimitiveInstanceDescription) instance.getInstanceDescription()).getCreatedObjects().length);
        assertEquals("Check message", "message3", mes); // Already reconfigured.
        // assertEquals("Check count", 1, count);

        instance.dispose();


    }


}
