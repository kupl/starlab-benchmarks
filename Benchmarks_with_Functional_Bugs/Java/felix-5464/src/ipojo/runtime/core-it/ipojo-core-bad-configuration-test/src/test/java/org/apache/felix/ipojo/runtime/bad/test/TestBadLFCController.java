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
package org.apache.felix.ipojo.runtime.bad.test;

import org.apache.felix.ipojo.ComponentFactory;
import org.apache.felix.ipojo.metadata.Attribute;
import org.apache.felix.ipojo.metadata.Element;
import org.apache.felix.ipojo.parser.ManifestMetadataParser;
import org.apache.felix.ipojo.parser.ParseException;
import org.junit.Test;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;

public class TestBadLFCController extends Common {

    private String clazz = "org.apache.felix.ipojo.test.scenarios.component.LifecycleControllerTest";

    private Element getNoFieldController() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        Element controller = new Element("controller", "");
        elem.addElement(controller);
        return elem;
    }

    private Element getBadFieldController() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        Element controller = new Element("controller", "");
        controller.addAttribute(new Attribute("field", "controller")); // Missing field
        elem.addElement(controller);
        elem.addElement(getManipulationForComponent("BAD-lcTest"));
        return elem;
    }

    private Element getManipulationForComponent(String comp_name) {
        // On KF we must cast the result
        String header = (String) getTestBundle().getHeaders().get("iPOJO-Components");
        Element elem = null;
        try {
            elem = ManifestMetadataParser.parseHeaderMetadata(header);
        } catch (ParseException e) {
            fail("Parse Exception when parsing iPOJO-Component");
        }

        assertNotNull("Check elem not null", elem);

        Element manip = getManipulationForComponent(elem, comp_name);
        assertNotNull("Check manipulation metadata not null for " + comp_name, manip);
        return manip;
    }

    private Element getManipulationForComponent(Element metadata, String comp_name) {
        Element[] comps = metadata.getElements("component");
        for (Element comp : comps) {
            if (comp.containsAttribute("name") && comp.getAttribute("name").equals(comp_name)) {
                return comp.getElements("manipulation")[0];
            }
        }
        return null;
    }

    @Test
    public void testNoField() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getNoFieldController());
            cf.start();
            cf.stop();
            fail("A lifecycle controller with a missing field must be rejected " + cf);
        } catch (Exception e) {
            // OK
        }
    }

    @Test
    public void testBadField() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getBadFieldController());
            cf.start();
            cf.stop();
            fail("A lifecycle controller with a bad field must be rejected " + cf);
        } catch (Exception e) {
            // OK
        }
    }

}
