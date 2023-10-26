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

import org.apache.felix.ipojo.*;
import org.apache.felix.ipojo.architecture.ComponentTypeDescription;
import org.apache.felix.ipojo.metadata.Element;
import org.apache.felix.ipojo.runtime.core.services.CheckService;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceReference;
import org.osgi.framework.ServiceRegistration;

import java.util.ArrayList;
import java.util.Dictionary;
import java.util.List;
import java.util.Properties;

import static org.junit.Assert.*;

public class FactoryManagementTest extends Common {

    private FakeFactory fake1 = new FakeFactory("fake");
    private FakeFactory fake2 = new FakeFactory("fake2");

    private Factory emptyFactory;
    private ComponentInstance empty;

    private class FakeFactory implements Factory {

        private String m_name;

        public FakeFactory(String name) {
            m_name = name;
        }

        public ComponentInstance createComponentInstance(Dictionary arg0) throws UnacceptableConfiguration {
            return null;
        }

        public ComponentInstance createComponentInstance(Dictionary arg0, ServiceContext arg1) throws UnacceptableConfiguration {
            return null;
        }

        public Element getDescription() {
            return null;
        }

        public String getName() {
            return m_name;
        }

        public boolean isAcceptable(Dictionary arg0) {
            return false;
        }

        public void reconfigure(Dictionary arg0) throws UnacceptableConfiguration {
        }

        public void addFactoryStateListener(FactoryStateListener arg0) {
        }

        public List getMissingHandlers() {
            return null;
        }

        public List getRequiredHandlers() {
            return null;
        }

        public void removeFactoryStateListener(FactoryStateListener arg0) {
        }

        public ComponentTypeDescription getComponentDescription() {
            return null;
        }

        public String getClassName() {
            return "";
        }

        public int getState() {
            return Factory.VALID;
        }

        public BundleContext getBundleContext() {
            return getContext();
        }

        public String getVersion() {
            return null;
        }

        public Element getComponentMetadata() {
            return null;
        }

        public List<ComponentInstance> getInstances() {
		    return new ArrayList<ComponentInstance>();
	    }

	    public List<String> getInstancesNames() {
		    return new ArrayList<String>();
	    }

    }

    @Before
    public void setUp() {
        emptyFactory = ipojoHelper.getFactory("composite.empty");
        Properties props = new Properties();
        props.put("instance.name", "empty-1");
        try {
            empty = emptyFactory.createComponentInstance(props);
        } catch (Exception e) {
            fail("Cannot create empty instance " + e.getMessage());
        }
    }

    @After
    public void tearDown() {
        empty.dispose();
        empty = null;
    }

    @Test
    public void testOneLevelExposition() {
        ServiceReference[] parentsFactoryReferences = osgiHelper.getServiceReferences(Factory.class.getName(), null);
        ServiceContext sc = getServiceContext(empty);
        ServiceReference[] internalFactoryReferences = ipojoHelper.getServiceReferences(sc, Factory.class.getName(), null);

        assertEquals("Check the number of available factories", parentsFactoryReferences.length, internalFactoryReferences.length);

        for (int i = 0; i < parentsFactoryReferences.length; i++) {
            Factory factory = (Factory) getContext().getService(parentsFactoryReferences[i]);
            assertTrue("Check the avaibility of " + factory.getName(), isExposed(factory, internalFactoryReferences, sc));
        }
    }

    @Test
    public void testTwoLevelExposition() {
        ServiceReference[] parentsFactoryReferences = osgiHelper.getServiceReferences(Factory.class.getName(), null);
        ServiceContext sc1 = getServiceContext(empty);
        ServiceReference[] Level1FactoryReferences = ipojoHelper.getServiceReferences(sc1, Factory.class.getName(), null);

        Factory fact = ipojoHelper.getFactory(sc1, "composite.empty");
        Properties p = new Properties();
        p.put("instance.name", "empty2");
        ComponentInstance empty2 = null;
        try {
            empty2 = fact.createComponentInstance(p);
        } catch (Exception e) {
            fail("Cannot instantiate empty2 instance : " + e.getMessage());
        }

        ServiceContext sc2 = getServiceContext(empty2);
        ServiceReference[] Level2FactoryReferences = ipojoHelper.getServiceReferences(sc2, Factory.class.getName(), null);

        assertEquals("Check the number of available factories - 1", parentsFactoryReferences.length, Level1FactoryReferences.length);
        assertEquals("Check the number of available factories - 2", parentsFactoryReferences.length, Level2FactoryReferences.length);
        assertEquals("Check the number of available factories - 3", Level1FactoryReferences.length, Level2FactoryReferences.length);

        for (int i = 0; i < Level1FactoryReferences.length; i++) {
            Factory factory = (Factory) getContext().getService(parentsFactoryReferences[i]);
            assertTrue("Check the avaibility of " + factory.getName(), isExposed(factory, Level2FactoryReferences, sc2));
        }

        empty2.dispose();
    }

    @Test
    public void testDynamism() {
        ServiceReference[] parentsFactoryReferences = osgiHelper.getServiceReferences(Factory.class.getName(), null);
        ServiceContext sc1 = getServiceContext(empty);
        ServiceReference[] Level1FactoryReferences = ipojoHelper.getServiceReferences(sc1, Factory.class.getName(), null);

        Factory fact = ipojoHelper.getFactory(sc1, "composite.empty");
        Properties p = new Properties();
        p.put("instance.name", "empty2");
        ComponentInstance empty2 = null;
        try {
            empty2 = fact.createComponentInstance(p);
        } catch (Exception e) {
            fail("Cannot instantiate empty2 instance : " + e.getMessage());
        }

        ServiceContext sc2 = getServiceContext(empty2);
        ServiceReference[] Level2FactoryReferences = ipojoHelper.getServiceReferences(sc2, Factory.class.getName(), null);

        assertEquals("Check the number of available factories - 1", parentsFactoryReferences.length, Level1FactoryReferences.length);
        assertEquals("Check the number of available factories - 2", parentsFactoryReferences.length, Level2FactoryReferences.length);
        assertEquals("Check the number of available factories - 3", Level1FactoryReferences.length, Level2FactoryReferences.length);

        for (int i = 0; i < Level1FactoryReferences.length; i++) {
            Factory factory = (Factory) getContext().getService(parentsFactoryReferences[i]);
            assertTrue("Check the avaibility of " + factory.getName(), isExposed(factory, Level2FactoryReferences, sc2));
        }

        // Publish fake1
        ServiceRegistration reg1 = getContext().registerService(Factory.class.getName(), fake1, null);

        parentsFactoryReferences = osgiHelper.getServiceReferences(Factory.class.getName(), null);
        sc1 = getServiceContext(empty);
        Level1FactoryReferences = ipojoHelper.getServiceReferences(sc1, Factory.class.getName(), null);
        sc2 = getServiceContext(empty2);
        Level2FactoryReferences = ipojoHelper.getServiceReferences(sc2, Factory.class.getName(), null);

        assertEquals("Check the number of available factories - 1.1", parentsFactoryReferences.length, Level1FactoryReferences.length);
        assertEquals("Check the number of available factories - 1.2", parentsFactoryReferences.length, Level2FactoryReferences.length);
        assertEquals("Check the number of available factories - 1.3", Level1FactoryReferences.length, Level2FactoryReferences.length);

        // 	Publish fake2
        ServiceRegistration reg2 = getContext().registerService(Factory.class.getName(), fake2, null);

        parentsFactoryReferences = osgiHelper.getServiceReferences(Factory.class.getName(), null);
        sc1 = getServiceContext(empty);
        Level1FactoryReferences = ipojoHelper.getServiceReferences(sc1, Factory.class.getName(), null);
        sc2 = getServiceContext(empty2);
        Level2FactoryReferences = ipojoHelper.getServiceReferences(sc2, Factory.class.getName(), null);

        assertEquals("Check the number of available factories - 1.1", parentsFactoryReferences.length, Level1FactoryReferences.length);
        assertEquals("Check the number of available factories - 1.2", parentsFactoryReferences.length, Level2FactoryReferences.length);
        assertEquals("Check the number of available factories - 1.3", Level1FactoryReferences.length, Level2FactoryReferences.length);

        reg1.unregister();

        parentsFactoryReferences = osgiHelper.getServiceReferences(Factory.class.getName(), null);
        sc1 = getServiceContext(empty);
        Level1FactoryReferences = ipojoHelper.getServiceReferences(sc1, Factory.class.getName(), null);
        sc2 = getServiceContext(empty2);
        Level2FactoryReferences = ipojoHelper.getServiceReferences(sc2, Factory.class.getName(), null);

        assertEquals("Check the number of available factories - 1.1", parentsFactoryReferences.length, Level1FactoryReferences.length);
        assertEquals("Check the number of available factories - 1.2", parentsFactoryReferences.length, Level2FactoryReferences.length);
        assertEquals("Check the number of available factories - 1.3", Level1FactoryReferences.length, Level2FactoryReferences.length);

        reg2.unregister();

        parentsFactoryReferences = osgiHelper.getServiceReferences(Factory.class.getName(), null);
        sc1 = getServiceContext(empty);
        Level1FactoryReferences = ipojoHelper.getServiceReferences(sc1, Factory.class.getName(), null);
        sc2 = getServiceContext(empty2);
        Level2FactoryReferences = ipojoHelper.getServiceReferences(sc2, Factory.class.getName(), null);

        assertEquals("Check the number of available factories - 1.1", parentsFactoryReferences.length, Level1FactoryReferences.length);
        assertEquals("Check the number of available factories - 1.2", parentsFactoryReferences.length, Level2FactoryReferences.length);
        assertEquals("Check the number of available factories - 1.3", Level1FactoryReferences.length, Level2FactoryReferences.length);

        empty2.dispose();
    }

    @Test
    public void testInvocation() {
        ServiceContext sc1 = getServiceContext(empty);
        Factory fact = ipojoHelper.getFactory(sc1, "composite.empty");
        Properties p = new Properties();
        p.put("instance.name", "empty2");
        ComponentInstance empty2 = null;
        try {
            empty2 = fact.createComponentInstance(p);
        } catch (Exception e) {
            fail("Cannot instantiate empty2 instance : " + e.getMessage());
        }

        ServiceContext sc2 = getServiceContext(empty2);

        Factory fact1 = ipojoHelper.getFactory(sc2, "COMPO-SimpleCheckServiceProvider");
        Properties props = new Properties();
        props.put("instance.name", "client");
        ComponentInstance client = null;
        try {
            client = fact1.createComponentInstance(props);
        } catch (Exception e) {
            e.printStackTrace();
            fail("Cannot instantiate the client : " + e.getMessage());
        }

        Factory fact2 = ipojoHelper.getFactory(sc2, "COMPO-FooProviderType-1");
        Properties props2 = new Properties();
        props2.put("instance.name", "provider");
        ComponentInstance provider = null;
        try {
            provider = fact2.createComponentInstance(props2);
        } catch (Exception e) {
            fail("Cannot instantiate the provider : " + e.getMessage());
        }

        ServiceReference ref = sc2.getServiceReference(CheckService.class.getName());
        assertNotNull("Check ref existency", ref);
        CheckService check = (CheckService) sc2.getService(ref);

        assertTrue("Check invocation", check.check());
        client.dispose();
        provider.dispose();
        empty2.dispose();
    }


    private boolean isExposed(Factory fact, ServiceReference[] refs, ServiceContext sc) {
        for (int i = 0; i < refs.length; i++) {
            Factory f = (Factory) sc.getService(refs[i]);
            if (fact.getName().equals(f.getName())) {
                sc.ungetService(refs[i]);
                return true;
            }
            sc.ungetService(refs[i]);
        }
        return false;
    }

}
