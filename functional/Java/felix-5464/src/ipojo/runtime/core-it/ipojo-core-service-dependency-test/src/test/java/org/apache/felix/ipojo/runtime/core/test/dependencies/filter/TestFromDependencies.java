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
package org.apache.felix.ipojo.runtime.core.test.dependencies.filter;

import org.apache.felix.ipojo.ComponentInstance;
import org.apache.felix.ipojo.architecture.Architecture;
import org.apache.felix.ipojo.architecture.InstanceDescription;
import org.apache.felix.ipojo.runtime.core.test.dependencies.Common;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.osgi.framework.ServiceReference;

import java.util.Properties;

import static org.junit.Assert.*;

public class TestFromDependencies extends Common {

    ComponentInstance instance1, instance2, instance3, instance4;
    ComponentInstance providerA, providerB, providerC, providerD;

    @Before
    public void setUp() {
        try {
            Properties prov = new Properties();
            prov.put("instance.name", "A");
            providerA = ipojoHelper.getFactory("SimpleFilterCheckServiceProvider").createComponentInstance(prov);
            providerA.stop();

            Properties prov2 = new Properties();
            prov2.put("instance.name", "B");
            providerB = ipojoHelper.getFactory("SimpleFilterCheckServiceProvider").createComponentInstance(prov2);
            providerB.stop();

            Properties prov3 = new Properties();
            prov3.put("service.pid", "A");
            providerC = ipojoHelper.getFactory("SimplePIDCheckServiceProvider").createComponentInstance(prov3);
            providerC.stop();

            Properties prov4 = new Properties();
            prov4.put("service.pid", "D");
            prov4.put("instance.name", "D");
            providerD = ipojoHelper.getFactory("SimplePIDCheckServiceProvider").createComponentInstance(prov4);
            providerD.stop();

            // Uses the component type from value
            Properties i1 = new Properties();
            i1.put("instance.name", "Subscriber1");
            instance1 = ipojoHelper.getFactory("SimpleFromCheckServiceSubscriber").createComponentInstance(i1);

            // Uses the instance configuration from value
            Properties i2 = new Properties();
            i2.put("instance.name", "Subscriber2");
            Properties ii2 = new Properties();
            ii2.put("id1", "B");
            i2.put("requires.from", ii2);
            instance2 = ipojoHelper.getFactory("SimpleFromCheckServiceSubscriber").createComponentInstance(i2);

            // Uses the instance configuration from value (*)
            Properties i3 = new Properties();
            i3.put("instance.name", "Subscriber3");
            Properties ii3 = new Properties();
            ii3.put("id1", "*");
            i3.put("requires.from", ii3);
            instance3 = ipojoHelper.getFactory("SimpleFromCheckServiceSubscriber").createComponentInstance(i3);

            // Uses the instance configuration from value, merge filter and from
            Properties i4 = new Properties();
            i4.put("instance.name", "Subscriber4");
            Properties ii4 = new Properties();
            ii4.put("id1", "D");
            i4.put("requires.from", ii4);
            Properties iii4 = new Properties();
            iii4.put("id1", "(service.pid=D)");
            i4.put("requires.filters", iii4);
            instance4 = ipojoHelper.getFactory("SimpleFromCheckServiceSubscriber").createComponentInstance(i4);

        } catch (Exception e) {
            e.printStackTrace();
            fail(e.getMessage());
        }

    }

    @After
    public void tearDown() {
        instance1.dispose();
        instance2.dispose();
        instance3.dispose();
        instance4.dispose();
        providerA.dispose();
        providerB.dispose();
        providerC.dispose();
        providerD.dispose();
        instance1 = null;
        instance2 = null;
        instance3 = null;
        instance4 = null;
        providerA = null;
        providerB = null;
        providerC = null;
        providerD = null;
    }


    @Test
    public void testFromInstanceName() {
        instance1.start();

        ServiceReference arch_ref = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), instance1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ref);
        InstanceDescription id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id.getState() == ComponentInstance.INVALID);

        providerB.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id.getState() == ComponentInstance.INVALID);

        providerA.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 1", id.getState() == ComponentInstance.VALID);

        providerA.stop();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 4", id.getState() == ComponentInstance.INVALID);

        providerA.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 3", id.getState() == ComponentInstance.VALID);


        providerA.stop();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 5", id.getState() == ComponentInstance.INVALID);

        id = null;
        providerB.stop();
        getContext().ungetService(arch_ref);
    }

    @Test
    public void testFromPID() {
        instance1.start();

        ServiceReference arch_ref = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), instance1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ref);
        InstanceDescription id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id.getState() == ComponentInstance.INVALID);

        providerB.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id.getState() == ComponentInstance.INVALID);

        providerC.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 1", id.getState() == ComponentInstance.VALID);

        providerC.stop();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 4", id.getState() == ComponentInstance.INVALID);

        providerC.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 3", id.getState() == ComponentInstance.VALID);

        providerC.stop();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 5", id.getState() == ComponentInstance.INVALID);

        id = null;
        providerB.stop();
        getContext().ungetService(arch_ref);
    }

    @Test
    public void testFromInstanceNameInstanceConfiguration() {
        instance2.start();

        ServiceReference arch_ref = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), instance2.getInstanceName());
        assertNotNull("Check architecture availability", arch_ref);
        InstanceDescription id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id.getState() == ComponentInstance.INVALID);

        providerA.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id.getState() == ComponentInstance.INVALID);

        providerB.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 1", id.getState() == ComponentInstance.VALID);

        providerB.stop();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 4", id.getState() == ComponentInstance.INVALID);

        providerB.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 3", id.getState() == ComponentInstance.VALID);

        providerB.stop();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 5", id.getState() == ComponentInstance.INVALID);

        id = null;
        providerA.stop();
        getContext().ungetService(arch_ref);
    }

    @Test
    public void testFromInstanceNameStar() {
        instance3.start();

        ServiceReference arch_ref = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), instance3.getInstanceName());
        assertNotNull("Check architecture availability", arch_ref);
        InstanceDescription id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id.getState() == ComponentInstance.INVALID);

        providerA.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 0", id.getState() == ComponentInstance.VALID);

        providerB.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 1", id.getState() == ComponentInstance.VALID);

        providerB.stop();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 4", id.getState() == ComponentInstance.VALID);

        providerA.stop();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 3", id.getState() == ComponentInstance.INVALID);

        providerB.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 5", id.getState() == ComponentInstance.VALID);

        id = null;
        providerB.stop();
        getContext().ungetService(arch_ref);
    }

    @Test
    public void testFromAndFilter() {
        instance4.start();

        ServiceReference arch_ref = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), instance4.getInstanceName());
        assertNotNull("Check architecture availability", arch_ref);
        InstanceDescription id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id.getState() == ComponentInstance.INVALID);

        providerA.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id.getState() == ComponentInstance.INVALID);

        providerD.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 1", id.getState() == ComponentInstance.VALID);

        providerD.stop();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 4", id.getState() == ComponentInstance.INVALID);

        providerD.start();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance validity - 3", id.getState() == ComponentInstance.VALID);

        providerD.stop();

        id = ((Architecture) getContext().getService(arch_ref)).getInstanceDescription();
        assertTrue("Check instance invalidity - 5", id.getState() == ComponentInstance.INVALID);

        id = null;
        providerA.stop();
        getContext().ungetService(arch_ref);
    }


}
