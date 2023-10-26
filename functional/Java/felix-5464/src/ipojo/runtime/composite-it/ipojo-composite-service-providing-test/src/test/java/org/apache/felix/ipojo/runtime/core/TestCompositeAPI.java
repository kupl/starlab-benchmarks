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
import org.apache.felix.ipojo.ServiceContext;
import org.apache.felix.ipojo.runtime.core.services.BazService;
import org.apache.felix.ipojo.runtime.core.services.CheckService;
import org.apache.felix.ipojo.runtime.core.services.FooService;
import org.junit.Test;
import org.osgi.framework.InvalidSyntaxException;
import org.osgi.framework.ServiceReference;

import java.util.Properties;

import static org.junit.Assert.*;

public class TestCompositeAPI extends Common {

    @Test
    public void testAPI() {
        Factory fact1 = ipojoHelper.getFactory("composite.empty");
        Properties p = new Properties();
        p.put("instance.name", "empty-1");
        ComponentInstance empty = null;
        try {
            empty = fact1.createComponentInstance(p);
        } catch (Exception e) {
            e.printStackTrace();
            fail(e.getMessage());
        }

        ServiceContext sc = getServiceContext(empty);

        Factory fact2 = ipojoHelper.getFactory("composite.test.2");
        Properties props2 = new Properties();
        props2.put("instance.name", "2"); // 2
        Factory fact3 = ipojoHelper.getFactory("composite.test.3");
        Properties props3 = new Properties();
        props3.put("instance.name", "3");
        ComponentInstance comp2 = null;
        ComponentInstance comp3 = null;
        try {
            comp2 = fact2.createComponentInstance(props2, sc);
            comp3 = fact3.createComponentInstance(props3, sc);
        } catch (Throwable e) {
            e.printStackTrace();
            fail(e.getMessage());
        }

        assertTrue("Test comp3", comp3.getState() == ComponentInstance.VALID);
        assertTrue("Test comp2", comp2.getState() == ComponentInstance.VALID);

        ServiceReference ref = null;

        ref = ipojoHelper.getServiceReferenceByName(sc, CheckService.class.getName(), "2"); // 2

        assertNotNull("Check ref", ref);
        CheckService cs = (CheckService) sc.getService(ref);
        assertTrue("Check invoke", cs.check());

        comp3.dispose();
        comp2.dispose();
        empty.dispose();
    }

    @Test
    public void testInstantiator() {
        String type = "composite.instantiator";
        Factory fact = ipojoHelper.getFactory(type);
        ComponentInstance ci = null;
        Properties p = new Properties();
        p.put("instance.name", "mon_coeur");
        try {
            ci = fact.createComponentInstance(p);
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertTrue("Check ci", ci.getState() == ComponentInstance.VALID);
        ServiceReference ref = ipojoHelper.getServiceReferenceByName(BazService.class.getName(), "mon_coeur");
        assertNotNull("Check ref", ref);
        BazService bs = (BazService) getContext().getService(ref);
        assertTrue("Check invocation", bs.foo());
        getContext().ungetService(ref);
        ref = ipojoHelper.getServiceReferenceByName(FooService.class.getName(), "mon_coeur");
        assertNotNull("Check ref 2 ", ref);
        FooService fs = (FooService) getContext().getService(ref);
        assertTrue("Check invocation", fs.foo());
        getContext().ungetService(ref);
        ci.dispose();
    }

    @Test
    public void testAPI2() {
        Factory fact1 = ipojoHelper.getFactory("composite.empty");
        Properties p = new Properties();
        p.put("instance.name", "empty-2");
        ComponentInstance empty = null;
        try {
            empty = fact1.createComponentInstance(p);
        } catch (Exception e) {
            e.printStackTrace();
        }

        ServiceContext sc = getServiceContext(empty);

        Factory fact2 = ipojoHelper.getFactory(sc, "composite.test.2");
        Properties props2 = new Properties();
        props2.put("instance.name", "4");
        Factory fact3 = ipojoHelper.getFactory(sc, "composite.test.3");
        Properties props3 = new Properties();
        props3.put("instance.name", "5");
        ComponentInstance comp2 = null;
        ComponentInstance comp3 = null;
        try {
            comp2 = fact2.createComponentInstance(props2, sc);
            comp3 = fact3.createComponentInstance(props3, sc);
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertTrue("Test comp3", comp3.getState() == ComponentInstance.VALID);
        assertTrue("Test comp2", comp2.getState() == ComponentInstance.VALID);

        ServiceReference ref = null;

        ref = ipojoHelper.getServiceReferenceByName(sc, CheckService.class.getName(), "4");

        assertNotNull("Check ref", ref);
        CheckService cs = (CheckService) sc.getService(ref);
        assertTrue("Check invoke", cs.check());

        comp3.dispose();
        comp2.dispose();
        empty.dispose();
    }

    @Test
    public void testApplication() {
        Factory factory = ipojoHelper.getFactory("composite.test.1");
        ComponentInstance ci = null;
        Properties props = new Properties();
        props.put("instance.name", "Test");
        try {
            ci = factory.createComponentInstance(props);
        } catch (Exception e) {
            fail("Cannot instantiate Test " + e.getMessage());
        }

        assertTrue("Check ci state", ci.getState() == ComponentInstance.VALID);

        ServiceReference[] refs = null;
        try {
            refs = getContext().getServiceReferences(CheckService.class.getName(), "(instance.name=Test)");
        } catch (InvalidSyntaxException e) {
            fail("Invalid filter : " + e.getMessage());
        }
        assertNotNull("Check refs not null", refs);
        CheckService cs = (CheckService) getContext().getService(refs[0]);

        assertTrue("Check invocation", cs.check());
        ci.dispose();

    }

}
