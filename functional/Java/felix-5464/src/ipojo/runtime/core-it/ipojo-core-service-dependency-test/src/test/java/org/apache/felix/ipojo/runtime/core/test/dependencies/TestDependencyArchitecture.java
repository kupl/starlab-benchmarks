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
package org.apache.felix.ipojo.runtime.core.test.dependencies;

import org.apache.felix.ipojo.ComponentInstance;
import org.apache.felix.ipojo.PrimitiveInstanceDescription;
import org.apache.felix.ipojo.architecture.Architecture;
import org.apache.felix.ipojo.architecture.InstanceDescription;
import org.apache.felix.ipojo.handlers.dependency.Dependency;
import org.apache.felix.ipojo.handlers.dependency.DependencyDescription;
import org.apache.felix.ipojo.handlers.dependency.DependencyHandlerDescription;
import org.apache.felix.ipojo.handlers.providedservice.ProvidedServiceHandlerDescription;
import org.apache.felix.ipojo.runtime.core.test.services.CheckService;
import org.apache.felix.ipojo.runtime.core.test.services.FooService;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.osgi.framework.ServiceReference;

import java.util.Properties;

import static org.junit.Assert.*;

public class TestDependencyArchitecture extends Common {

    ComponentInstance fooProvider1, fooProvider2;

    ComponentInstance instance1, instance2, instance3, instance4, instance5;

    @Before
    public void setUp() {
        try {
            Properties prov = new Properties();
            prov.put("instance.name", "FooProvider");
            fooProvider1 = ipojoHelper.getFactory("FooProviderType-1").createComponentInstance(prov);
            fooProvider1.stop();

            Properties prov2 = new Properties();
            prov2.put("instance.name", "FooProvider2");
            fooProvider2 = ipojoHelper.getFactory("FooProviderType-1").createComponentInstance(prov2);
            fooProvider2.stop();

            Properties i1 = new Properties();
            i1.put("instance.name", "Simple");
            instance1 = ipojoHelper.getFactory("SimpleCheckServiceProvider").createComponentInstance(i1);

            Properties i5 = new Properties();
            i5.put("instance.name", "ProxiedSimple");
            instance5 = ipojoHelper.getFactory("ProxiedSimpleCheckServiceProvider").createComponentInstance(i5);

            Properties i2 = new Properties();
            i2.put("instance.name", "Optional");
            instance2 = ipojoHelper.getFactory("SimpleOptionalCheckServiceProvider").createComponentInstance(i2);

            Properties i3 = new Properties();
            i3.put("instance.name", "Multiple");
            instance3 = ipojoHelper.getFactory("SimpleMultipleCheckServiceProvider").createComponentInstance(i3);

            Properties i4 = new Properties();
            i4.put("instance.name", "OptionalMultiple");
            instance4 = ipojoHelper.getFactory("SimpleOptionalMultipleCheckServiceProvider").createComponentInstance(i4);
        } catch (Exception e) {
            throw new RuntimeException(e.getMessage());
        }
    }

    @After
    public void tearDown() {
        instance1.dispose();
        instance2.dispose();
        instance3.dispose();
        instance4.dispose();
        instance5.dispose();
        fooProvider1.dispose();
        fooProvider2.dispose();
        instance1 = null;
        instance2 = null;
        instance3 = null;
        instance4 = null;
        instance5 = null;
        fooProvider1 = null;
        fooProvider2 = null;
    }

    private DependencyHandlerDescription getDependencyDesc(InstanceDescription id) {
        DependencyHandlerDescription handler = (DependencyHandlerDescription) id.getHandlerDescription("org.apache.felix.ipojo:requires");
        if (handler == null) {
            fail("Dependency Handler not found");
            return null;
        } else {
            return handler;
        }
    }

    private DependencyDescription getDependencyDescBySpecification(
            PrimitiveInstanceDescription id, String spec) {
        return id.getDependency(spec);
    }

    private ProvidedServiceHandlerDescription getPSDesc(InstanceDescription id) {
        ProvidedServiceHandlerDescription handler = (ProvidedServiceHandlerDescription) id.getHandlerDescription("org.apache.felix.ipojo:provides");
        if (handler == null) {
            fail("Provided Service Handler not found");
            return null;
        } else {
            return handler;
        }
    }

    @Test
    public void testSimpleDependency() {
        ServiceReference arch_dep = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), instance1.getInstanceName());
        assertNotNull("Check architecture availability", arch_dep);
        PrimitiveInstanceDescription id_dep = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_dep.getState() == ComponentInstance.INVALID);

        // Check dependency handler invalidity
        DependencyHandlerDescription dhd = getDependencyDesc(id_dep);
        assertFalse("Check dependency handler invalidity", dhd.isValid());

        // Check dependency metadata
        assertEquals("Check dependency interface", dhd.getDependencies()[0].getInterface(), FooService.class.getName());
        assertEquals("Check dependency id", dhd.getDependencies()[0].getId(), FooService.class.getName());
        assertFalse("Check dependency cardinality", dhd.getDependencies()[0].isMultiple());
        assertFalse("Check dependency optionality", dhd.getDependencies()[0].isOptional());
        assertNull("Check dependency ref -1", dhd.getDependencies()[0].getServiceReferences());
        assertFalse("Check dependency proxy", dhd.getDependencies()[0].isProxy());

        fooProvider1.start();

        ServiceReference arch_ps = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps);
        PrimitiveInstanceDescription id_ps = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps.getState() == ComponentInstance.VALID);

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler validity", dhd.isValid());
        assertEquals("Check dependency ref - 2 ", dhd.getDependencies()[0].getServiceReferences().size(), 1);

        ServiceReference cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance1.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        CheckService cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps = ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        ProvidedServiceHandlerDescription psh = getPSDesc(id_ps);
        assertEquals("Check POJO creation", id_ps.getCreatedObjects().length, 1);

        fooProvider1.stop();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id_dep.getState() == ComponentInstance.INVALID);
        dhd = getDependencyDesc(id_dep);
        assertFalse("Check dependency handler invalidity", dhd.isValid());

        fooProvider1.start();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        arch_ps = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps);
        //id_ps = ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps.getState() == ComponentInstance.VALID);
        psh = getPSDesc(id_ps);
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        assertTrue("Check dependency handler validity", dhd.isValid());

        assertEquals("Check dependency ref -3", dhd.getDependencies()[0].getServiceReferences().size(), 1);

        cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance1.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph 
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps = ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        psh = getPSDesc(id_ps);


        fooProvider1.stop();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id_dep.getState() == ComponentInstance.INVALID);
        dhd = getDependencyDesc(id_dep);
        assertFalse("Check dependency handler invalidity", dhd.isValid());

        id_dep = null;
        cs = null;
        getContext().ungetService(arch_dep);
        getContext().ungetService(cs_ref);
    }

    @Test
    public void testProxiedSimpleDependency() {
        ServiceReference arch_dep = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), instance5.getInstanceName());
        assertNotNull("Check architecture availability", arch_dep);
        PrimitiveInstanceDescription id_dep = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_dep.getState() == ComponentInstance.INVALID);

        // Check dependency handler invalidity
        DependencyHandlerDescription dhd = getDependencyDesc(id_dep);
        assertFalse("Check dependency handler invalidity", dhd.isValid());

        // Check dependency metadata
        assertEquals("Check dependency interface", dhd.getDependencies()[0].getInterface(), FooService.class.getName());
        assertEquals("Check dependency id", dhd.getDependencies()[0].getId(), FooService.class.getName());
        assertFalse("Check dependency cardinality", dhd.getDependencies()[0].isMultiple());
        assertFalse("Check dependency optionality", dhd.getDependencies()[0].isOptional());
        assertNull("Check dependency ref -1", dhd.getDependencies()[0].getServiceReferences());
        assertTrue("Check dependency proxy", dhd.getDependencies()[0].isProxy());

        fooProvider1.start();

        ServiceReference arch_ps = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps);
        PrimitiveInstanceDescription id_ps = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps.getState() == ComponentInstance.VALID);

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler validity", dhd.isValid());
        assertEquals("Check dependency ref - 2 ", dhd.getDependencies()[0].getServiceReferences().size(), 1);

        ServiceReference cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance5.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        CheckService cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps = ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        ProvidedServiceHandlerDescription psh = getPSDesc(id_ps);
        assertEquals("Check POJO creation", id_ps.getCreatedObjects().length, 1);

        fooProvider1.stop();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id_dep.getState() == ComponentInstance.INVALID);
        dhd = getDependencyDesc(id_dep);
        assertFalse("Check dependency handler invalidity", dhd.isValid());

        fooProvider1.start();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        arch_ps = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps);
        //id_ps = ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps.getState() == ComponentInstance.VALID);
        psh = getPSDesc(id_ps);
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        assertTrue("Check dependency handler validity", dhd.isValid());

        assertEquals("Check dependency ref -3", dhd.getDependencies()[0].getServiceReferences().size(), 1);

        cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance5.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph 
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps = ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        psh = getPSDesc(id_ps);

        fooProvider1.stop();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id_dep.getState() == ComponentInstance.INVALID);
        dhd = getDependencyDesc(id_dep);
        assertFalse("Check dependency handler invalidity", dhd.isValid());

        id_dep = null;
        cs = null;
        getContext().ungetService(arch_dep);
        getContext().ungetService(cs_ref);
    }

    @Test
    public void testOptionalDependency() {
        ServiceReference arch_dep = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), instance2.getInstanceName());
        assertNotNull("Check architecture availability", arch_dep);
        PrimitiveInstanceDescription id_dep = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_dep.getState() == ComponentInstance.VALID);

        // Check dependency handler invalidity
        DependencyHandlerDescription dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler invalidity", dhd.isValid());

        // Check dependency metadata
        assertEquals("Check dependency interface", dhd.getDependencies()[0].getInterface(), FooService.class.getName());
        assertEquals("Check dependency id", dhd.getDependencies()[0].getId(), "FooService");
        assertFalse("Check dependency cardinality", dhd.getDependencies()[0].isMultiple());
        assertTrue("Check dependency optionality", dhd.getDependencies()[0].isOptional());
        assertNull("Check dependency ref -1", dhd.getDependencies()[0].getServiceReferences());

        fooProvider1.start();

        ServiceReference arch_ps = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps);
        PrimitiveInstanceDescription id_ps = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps.getState() == ComponentInstance.VALID);

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler validity", dhd.isValid());
        assertEquals("Check dependency ref - 2 ", dhd.getDependencies()[0].getServiceReferences().size(), 1);

        ServiceReference cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance2.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        CheckService cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps = ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        ProvidedServiceHandlerDescription psh = getPSDesc(id_ps);
        assertEquals("Check POJO creation", id_ps.getCreatedObjects().length, 1);

        fooProvider1.stop();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler invalidity", dhd.isValid());

        fooProvider1.start();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        arch_ps = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps);
        //id_ps = ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps.getState() == ComponentInstance.VALID);
        psh = getPSDesc(id_ps);
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        assertTrue("Check dependency handler validity", dhd.isValid());

        assertEquals("Check dependency ref -3", dhd.getDependencies()[0].getServiceReferences().size(), 1);

        cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance2.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph 
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps = ((Architecture) osgiHelper.getRawServiceObject(arch_ps)).getInstanceDescription();
        psh = getPSDesc(id_ps);

        fooProvider1.stop();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler invalidity", dhd.isValid());

        id_dep = null;
        cs = null;
        getContext().ungetService(arch_dep);
        getContext().ungetService(cs_ref);
    }

    @Test
    public void testMultipleDependency() {
        ServiceReference arch_dep = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), instance3.getInstanceName());
        assertNotNull("Check architecture availability", arch_dep);
        PrimitiveInstanceDescription id_dep = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_dep.getState() == ComponentInstance.INVALID);

        // Check dependency handler invalidity
        DependencyHandlerDescription dhd = getDependencyDesc(id_dep);
        DependencyDescription dd = getDependencyDescBySpecification(id_dep, FooService.class.getName());
        assertFalse("Check dependency handler invalidity", dhd.isValid());
        assertTrue("Check dependency invalidity", dd.getState() == Dependency.UNRESOLVED);


        // Check dependency metadata
        assertEquals("Check dependency interface", dhd.getDependencies()[0].getInterface(), FooService.class.getName());
        assertTrue("Check dependency cardinality", dhd.getDependencies()[0].isMultiple());
        assertFalse("Check dependency optionality", dhd.getDependencies()[0].isOptional());
        assertNull("Check dependency ref -1", dhd.getDependencies()[0].getServiceReferences());

        assertEquals("Check dependency interface", dd.getSpecification(), FooService.class.getName());
        assertTrue("Check dependency cardinality", dd.isMultiple());
        assertFalse("Check dependency optionality", dd.isOptional());
        assertNull("Check dependency ref -1", dd.getServiceReferences());
        assertFalse("Check dependency proxy", dhd.getDependencies()[0].isProxy());

        fooProvider1.start();

        ServiceReference arch_ps1 = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps1);
        PrimitiveInstanceDescription id_ps1 = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        assertTrue("Check instance validity - 1", id_ps1.getState() == ComponentInstance.VALID);

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler validity", dhd.isValid());
        assertEquals("Check dependency ref - 2 ", dhd.getDependencies()[0].getServiceReferences().size(), 1);
        assertEquals("Check used ref - 1 (" + dhd.getDependencies()[0].getUsedServices().size() + ")", dhd.getDependencies()[0].getUsedServices().size(), 0);

        ServiceReference cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance3.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        CheckService cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        ProvidedServiceHandlerDescription psh = getPSDesc(id_ps1);
        assertEquals("Check POJO creation", id_ps1.getCreatedObjects().length, 1);

        // Start a second foo service provider
        fooProvider2.start();

        arch_ps1 = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        ServiceReference arch_ps2 = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider2.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps1);
        assertNotNull("Check architecture 2 availability", arch_ps2);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        PrimitiveInstanceDescription id_ps2 = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_ps2)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps1.getState() == ComponentInstance.VALID);
        assertTrue("Check instance 2 invalidity - 1", id_ps2.getState() == ComponentInstance.VALID);

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler validity", dhd.isValid());
        assertEquals("Check dependency ref - 3 ", dhd.getDependencies()[0].getServiceReferences().size(), 2);
        assertEquals("Check used ref - 2 ", dhd.getDependencies()[0].getUsedServices().size(), 1); // provider 2 not already used

        cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance3.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        //id_ps2 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        ProvidedServiceHandlerDescription psh1 = getPSDesc(id_ps1);
        ProvidedServiceHandlerDescription psh2 = getPSDesc(id_ps2);
        assertEquals("Check POJO creation", id_ps1.getCreatedObjects().length, 1);
        assertEquals("Check POJO creation", id_ps2.getCreatedObjects().length, 1);
        assertEquals("Check used ref - 3 (" + dhd.getDependencies()[0].getUsedServices().size() + ")", dhd.getDependencies()[0].getUsedServices().size(), 2);

        fooProvider2.stop();

        arch_ps1 = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps1);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        assertTrue("Check instance validity - 1", id_ps1.getState() == ComponentInstance.VALID);

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler validity", dhd.isValid());
        assertEquals("Check dependency ref - 2 ", dhd.getDependencies()[0].getServiceReferences().size(), 1);
        assertEquals("Check used ref - 4 ", dhd.getDependencies()[0].getUsedServices().size(), 1);

        cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance3.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        psh = getPSDesc(id_ps1);
        assertEquals("Check POJO creation", id_ps1.getCreatedObjects().length, 1);
        assertEquals("Check used ref - 5 ", dhd.getDependencies()[0].getUsedServices().size(), 1);

        fooProvider1.stop();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertFalse("Check instance invalidity - 2", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertFalse("Check dependency handler invalidity", dhd.isValid());

        fooProvider2.start();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        arch_ps2 = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider2.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps1);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();

        assertTrue("Check instance invalidity - 1", id_ps2.getState() == ComponentInstance.VALID);

        psh = getPSDesc(id_ps2);
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        assertTrue("Check dependency handler validity", dhd.isValid());

        assertEquals("Check dependency ref -3", dhd.getDependencies()[0].getServiceReferences().size(), 1);
        assertEquals("Check used ref - 6 ", dhd.getDependencies()[0].getUsedServices().size(), 0);

        cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance3.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph 
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        psh = getPSDesc(id_ps2);
        assertEquals("Check used ref - 7 ", dhd.getDependencies()[0].getUsedServices().size(), 1);

        fooProvider2.stop();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertFalse("Check instance invalidity - 2", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertFalse("Check dependency handler invalidity", dhd.isValid());

        id_dep = null;
        cs = null;
        getContext().ungetService(arch_dep);
        getContext().ungetService(cs_ref);
    }

    @Test
    public void testMultipleOptionalDependency() {
        ServiceReference arch_dep = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), instance4.getInstanceName());
        assertNotNull("Check architecture availability", arch_dep);
        PrimitiveInstanceDescription id_dep = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_dep.getState() == ComponentInstance.VALID);

        // Check dependency handler invalidity
        DependencyHandlerDescription dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler invalidity", dhd.isValid());

        // Check dependency metadata
        assertEquals("Check dependency interface", dhd.getDependencies()[0].getInterface(), FooService.class.getName());
        assertTrue("Check dependency cardinality", dhd.getDependencies()[0].isMultiple());
        assertTrue("Check dependency optionality", dhd.getDependencies()[0].isOptional());
        assertNull("Check dependency ref -1", dhd.getDependencies()[0].getServiceReferences());

        fooProvider1.start();

        ServiceReference arch_ps1 = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps1);
        PrimitiveInstanceDescription id_ps1 = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps1.getState() == ComponentInstance.VALID);

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler validity", dhd.isValid());
        assertEquals("Check dependency ref - 2 ", dhd.getDependencies()[0].getServiceReferences().size(), 1);

        ServiceReference cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance4.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        CheckService cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        ProvidedServiceHandlerDescription psh = getPSDesc(id_ps1);
        assertEquals("Check POJO creation", id_ps1.getCreatedObjects().length, 1);

        // Start a second foo service provider
        fooProvider2.start();

        arch_ps1 = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        ServiceReference arch_ps2 = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider2.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps1);
        assertNotNull("Check architecture 2 availability", arch_ps2);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        PrimitiveInstanceDescription id_ps2 = (PrimitiveInstanceDescription) ((Architecture) osgiHelper.getRawServiceObject(arch_ps2)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps1.getState() == ComponentInstance.VALID);
        assertTrue("Check instance 2 invalidity - 1", id_ps2.getState() == ComponentInstance.VALID);

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler validity", dhd.isValid());
        assertEquals("Check dependency ref - 3 ", dhd.getDependencies()[0].getServiceReferences().size(), 2);

        cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance4.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        //id_ps2 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        ProvidedServiceHandlerDescription psh1 = getPSDesc(id_ps1);
        ProvidedServiceHandlerDescription psh2 = getPSDesc(id_ps2);
        assertEquals("Check POJO creation", id_ps1.getCreatedObjects().length, 1);
        assertEquals("Check POJO creation", id_ps2.getCreatedObjects().length, 1);

        fooProvider2.stop();

        arch_ps1 = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider1.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps1);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps1.getState() == ComponentInstance.VALID);

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler validity", dhd.isValid());
        assertEquals("Check dependency ref - 2 ", dhd.getDependencies()[0].getServiceReferences().size(), 1);

        cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance4.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        psh = getPSDesc(id_ps1);
        assertEquals("Check POJO creation", id_ps1.getCreatedObjects().length, 1);

        fooProvider1.stop();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler invalidity", dhd.isValid());

        fooProvider2.start();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        arch_ps2 = ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), fooProvider2.getInstanceName());
        assertNotNull("Check architecture availability", arch_ps2);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        assertTrue("Check instance invalidity - 1", id_ps2.getState() == ComponentInstance.VALID);
        psh = getPSDesc(id_ps2);
        assertTrue("Check instance validity", id_dep.getState() == ComponentInstance.VALID);
        assertTrue("Check dependency handler validity", dhd.isValid());

        assertEquals("Check dependency ref -3", dhd.getDependencies()[0].getServiceReferences().size(), 1);

        cs_ref = ipojoHelper.getServiceReferenceByName(CheckService.class.getName(), instance4.getInstanceName());
        assertNotNull("Check CheckService availability", cs_ref);
        cs = (CheckService) osgiHelper.getRawServiceObject(cs_ref);
        assertTrue("check CheckService invocation", cs.check());

        // Check object graph 
        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        dhd = getDependencyDesc(id_dep);
        //id_ps1 = ((Architecture) osgiHelper.getRawServiceObject(arch_ps1)).getInstanceDescription();
        psh = getPSDesc(id_ps2);

        fooProvider2.stop();

        //id_dep = ((Architecture) osgiHelper.getRawServiceObject(arch_dep)).getInstanceDescription();
        assertTrue("Check instance invalidity - 2", id_dep.getState() == ComponentInstance.VALID);
        dhd = getDependencyDesc(id_dep);
        assertTrue("Check dependency handler invalidity", dhd.isValid());

        id_dep = null;
        cs = null;
        getContext().ungetService(arch_dep);
        getContext().ungetService(cs_ref);
    }


}
