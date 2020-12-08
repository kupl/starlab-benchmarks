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
package org.apache.felix.ipojo.runtime.core.providing;

import org.apache.felix.ipojo.ComponentFactory;
import org.apache.felix.ipojo.ComponentInstance;
import org.apache.felix.ipojo.Factory;
import org.apache.felix.ipojo.architecture.Architecture;
import org.apache.felix.ipojo.runtime.core.Common;
import org.apache.felix.ipojo.runtime.core.components.TotoProvider;
import org.apache.felix.ipojo.runtime.core.services.Tota;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.osgi.framework.ServiceReference;

import java.util.Properties;

import static org.junit.Assert.*;

public class TestComp2 extends Common {

    private ComponentFactory tataFactory;
    private ComponentFactory totoFactory;
    private ComponentInstance totoProv, totoProv2;
    private ComponentInstance under;
    private ComponentFactory tataFactory2;

    @Before
    public void setUp() {
        tataFactory = (ComponentFactory) ipojoHelper.getFactory("tata");
        totoFactory = (ComponentFactory) ipojoHelper.getFactory("toto");
        tataFactory2 = (ComponentFactory) ipojoHelper.getFactory("comp-6");
        tataFactory2.stop();
        tataFactory.stop();

        Properties props = new Properties();
        props.put("instance.name", "toto provider");
        try {
            totoProv = totoFactory.createComponentInstance(props);
        } catch (Exception e) {
            e.printStackTrace();
        }

        Properties props3 = new Properties();
        props3.put("instance.name", "toto provider 2");
        try {
            totoProv2 = totoFactory.createComponentInstance(props3);
        } catch (Exception e) {
            e.printStackTrace();
        }

        totoProv.stop();
        totoProv2.stop();

        Factory factory = ipojoHelper.getFactory("comp-2");
        Properties props2 = new Properties();
        props2.put("instance.name", "ff");
        try {
            under = factory.createComponentInstance(props2);
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    @After
    public void tearDown() {
        tataFactory.start();
        totoProv.dispose();
        totoProv = null;
        totoProv2.dispose();
        totoProv2 = null;
        tataFactory2.start();

        // Reset counters
        TotoProvider.toto = 0;
        TotoProvider.toto_2 = 0;
        TotoProvider.toto_3 = 0;
        TotoProvider.toto_4 = 0;
        TotoProvider.toto1 = 0;
    }

    @Test
    public void testSimple() {
        // Neither factory nor instance
        assertTrue("Assert under state - 1", under.getState() == ComponentInstance.INVALID);
        assertNull("Assert no tota service - 1", getContext().getServiceReference(Tota.class.getName()));

        // Start the importer
        totoProv.start();
        assertTrue("Assert under state - 2 (" + under.getState() + ")", under.getState() == ComponentInstance.INVALID);
        assertNull("Assert no tota service - 2", getContext().getServiceReference(Tota.class.getName()));

        // Start the factory
        tataFactory.start();
        assertTrue("Assert under state - 3", under.getState() == ComponentInstance.VALID);
        assertNotNull("Assert tota service - 3", getContext().getServiceReference(Tota.class.getName()));
        ServiceReference ref = getContext().getServiceReference(Tota.class.getName());
        Tota tota = (Tota) getContext().getService(ref);

        invokeAll(tota);

        // Check toto
        Properties props = tota.getProps();
        Integer toto = (Integer) props.get("toto");
        Integer toto_2 = (Integer) props.get("toto_2");
        Integer toto_3 = (Integer) props.get("toto_3");
        Integer toto_4 = (Integer) props.get("toto_4");
        Integer toto_1 = (Integer) props.get("toto1");
        assertEquals("Assert toto - 3 (" + toto.intValue() + ")", toto.intValue(), 1);
        assertEquals("Assert toto_2 - 3", toto_2.intValue(), 1);
        assertEquals("Assert toto_3 - 3", toto_3.intValue(), 1);
        assertEquals("Assert toto_4 - 3", toto_4.intValue(), 0);
        assertEquals("Assert toto1 - 3 (" + toto_1.intValue() + ")", toto_1.intValue(), 1);
        //Check tata
        props = tota.getPropsTata();
        Integer tata = (Integer) props.get("tata");
        assertEquals("Assert tata - 3", tata.intValue(), 1);

        getContext().ungetService(ref);
        tota = null;

        // Start a second import
        totoProv2.start();
        assertTrue("Assert under state - 4", under.getState() == ComponentInstance.VALID);
        assertNotNull("Assert tota service - 4", getContext().getServiceReference(Tota.class.getName()));
        ref = getContext().getServiceReference(Tota.class.getName());
        tota = (Tota) getContext().getService(ref);
        invokeAll(tota);
        // Check toto
        props = tota.getProps();
        toto = (Integer) props.get("toto");
        toto_2 = (Integer) props.get("toto_2");
        toto_3 = (Integer) props.get("toto_3");
        toto_4 = (Integer) props.get("toto_4");
        toto_1 = (Integer) props.get("toto1");
        assertEquals("Assert toto - 4 (" + toto.intValue() + ")", toto.intValue(), 2);
        assertEquals("Assert toto_2 - 4 (" + toto_2.intValue() + ")", toto_2.intValue(), 2);
        assertEquals("Assert toto_3 - 4", toto_3.intValue(), 2);
        assertEquals("Assert toto_4 - 4", toto_4.intValue(), 0);
        assertEquals("Assert toto1 - 4", toto_1.intValue(), 3);
        //Check tata
        props = tota.getPropsTata();
        tata = (Integer) props.get("tata");
        assertEquals("Assert tata - 4", tata.intValue(), 2);

        getContext().ungetService(ref);
        tota = null;

        tataFactory.stop();
        assertTrue("Assert under state - 5", under.getState() == ComponentInstance.INVALID);
        assertNull("Assert no tota service - 5", getContext().getServiceReference(Tota.class.getName()));

        totoProv2.stop();
        tataFactory.start();
        assertTrue("Assert under state - 6", under.getState() == ComponentInstance.VALID);
        assertNotNull("Assert tota service - 6", getContext().getServiceReference(Tota.class.getName()));
        ref = getContext().getServiceReference(Tota.class.getName());
        tota = (Tota) getContext().getService(ref);
        invokeAll(tota);
        // Check toto
        props = tota.getProps();
        toto = (Integer) props.get("toto");
        toto_2 = (Integer) props.get("toto_2");
        toto_3 = (Integer) props.get("toto_3");
        toto_4 = (Integer) props.get("toto_4");
        toto_1 = (Integer) props.get("toto1");
        assertEquals("Assert toto - 6 (" + toto.intValue() + ")", toto.intValue(), 3);
        assertEquals("Assert toto_2 - 6 (" + toto_2.intValue() + ")", toto_2.intValue(), 3);
        assertEquals("Assert toto_3 - 6", toto_3.intValue(), 3);
        assertEquals("Assert toto_4 - 6", toto_4.intValue(), 0);
        assertEquals("Assert toto1 - 6", toto_1.intValue(), 4);
        //Check tata
        props = tota.getPropsTata();
        tata = (Integer) props.get("tata");
        assertEquals("Assert tata - 6", tata.intValue(), 1);

        getContext().ungetService(ref);
        tota = null;

        // Is arch exposed
        assertNotNull("Test arch", ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), "ff"));

        totoProv.stop();

        assertTrue("Assert under state - 7", under.getState() == ComponentInstance.INVALID);
        assertNotNull("Test arch-2", ipojoHelper.getServiceReferenceByName(Architecture.class.getName(), "ff"));
        assertNull("Assert no tota service - 7", getContext().getServiceReference(Tota.class.getName()));

        under.dispose();
        under = null;
    }

    private void invoke(Tota tota) {
        tota.tata();

        assertEquals("Assert invoke tataint", tota.tataInt(2), 2);
        assertEquals("Assert invoke tataLong", tota.tataLong(2), 2);
        assertEquals("Assert invoke tataDouble", tota.tataDouble(2), 2, 0);
        assertEquals("Assert invoke tataChar", tota.tataChar('a'), 'a');
        assertTrue("Assert invoke tataBoolean", tota.tataBoolean(true));
        assertEquals("Assert invoke tataByte", tota.tataByte((byte) 2), 2);
        assertEquals("Assert invoke tataShort", tota.tataShort((short) 5), 5);
        assertEquals("Assert invoke tataFloat", tota.tataFloat(5), 5, 0);

    }

    private void invokeToto(Tota tota) {
        tota.toto();
        assertEquals("Assert toto", tota.toto("foo"), "foo");
        tota.toto(1, 2);
        tota.toto1("foo2");
    }

    private void invokeAll(Tota tota) {
        invoke(tota);
        invokeToto(tota);
    }


}
