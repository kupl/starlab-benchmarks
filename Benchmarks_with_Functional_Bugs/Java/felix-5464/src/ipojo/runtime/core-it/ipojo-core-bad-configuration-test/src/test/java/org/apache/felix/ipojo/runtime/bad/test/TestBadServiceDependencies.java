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

import org.apache.felix.ipojo.*;
import org.apache.felix.ipojo.metadata.Attribute;
import org.apache.felix.ipojo.metadata.Element;
import org.apache.felix.ipojo.parser.ManifestMetadataParser;
import org.apache.felix.ipojo.parser.ParseException;
import org.apache.felix.ipojo.runtime.bad.services.BarService;
import org.junit.Before;
import org.junit.Test;

import java.util.Properties;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;

public class TestBadServiceDependencies extends Common {

    private String clazz = "org.apache.felix.ipojo.test.scenarios.component.CheckServiceProvider";
    private String type = "BAD-BothCheckServiceProvider";
    private Element manipulation;
    private Properties props;

    @Before
    public void setUp() {
        manipulation = getManipulationForComponent();
        props = new Properties();
        props.put("instance.name", "BAD");
    }


    private Element getNothing() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        Element callback = new Element("requires", "");
        elem.addElement(callback);
        elem.addElement(manipulation);
        return elem;
    }

    private Element getNoField() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        Element callback = new Element("requires", "");
        callback.addAttribute(new Attribute("filter", "(foo=bar)"));
        elem.addElement(callback);
        elem.addElement(manipulation);
        return elem;
    }

    private Element getBadField() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        Element callback = new Element("requires", "");
        callback.addAttribute(new Attribute("field", "BAD_FIELD")); // missing field.
        elem.addElement(callback);
        elem.addElement(manipulation);
        return elem;
    }

    private Element getBadFilter() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        Element callback = new Element("requires", "");
        callback.addAttribute(new Attribute("field", "fs"));
        callback.addAttribute(new Attribute("filter", "(foo=bar)&(bar=foo)")); // Incorrect filter
        elem.addElement(callback);
        elem.addElement(manipulation);
        return elem;
    }

    private Element getBadFrom() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        Element callback = new Element("requires", "");
        callback.addAttribute(new Attribute("field", "fs"));
        callback.addAttribute(new Attribute("from", "ba(d&_")); // Incorrect from
        elem.addElement(callback);
        elem.addElement(manipulation);
        return elem;
    }

    private Element getBadType() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        Element callback = new Element("requires", "");
        callback.addAttribute(new Attribute("field", "fs"));
        callback.addAttribute(new Attribute("interface", BarService.class.getName()));
        elem.addElement(callback);
        elem.addElement(manipulation);
        return elem;
    }

    private Element getMissingSpecification() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        // iPOJO cannot determine the specification of this type of dependency.
        Element dependency = new Element("requires", "");
        Element callback = new Element("callback", "");
        callback.addAttribute(new Attribute("type", "bind"));
        callback.addAttribute(new Attribute("method", "refBind"));
        dependency.addElement(callback);
        elem.addElement(dependency);

        elem.addElement(manipulation);
        return elem;
    }

    private Element getMissingCallbackType() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        // iPOJO cannot determine the specification of this type of dependency.
        Element dependency = new Element("requires", "");
        dependency.addAttribute(new Attribute("field", "fs"));
        Element callback = new Element("callback", "");
        // callback.addAttribute(new Attribute("type", "bind")); --> Type missing.
        callback.addAttribute(new Attribute("method", "refBind"));
        dependency.addElement(callback);
        elem.addElement(dependency);

        elem.addElement(manipulation);
        return elem;
    }

    private Element getMissingCallbackMethod() {
        Element elem = new Element("component", "");
        elem.addAttribute(new Attribute("classname", clazz));

        // iPOJO cannot determine the specification of this type of dependency.
        Element dependency = new Element("requires", "");
        dependency.addAttribute(new Attribute("field", "fs"));
        Element callback = new Element("callback", "");
        callback.addAttribute(new Attribute("type", "bind"));
        // callback.addAttribute(new Attribute("method", "refBind"));  --> Method missing.
        dependency.addElement(callback);
        elem.addElement(dependency);

        elem.addElement(manipulation);
        return elem;
    }


    private Element getManipulationForComponent() {
        // On KF we must cast the result.
        String header = (String) getTestBundle().getHeaders().get("iPOJO-Components");
        Element elem = null;
        try {
            elem = ManifestMetadataParser.parse(header);
        } catch (ParseException e) {
            fail("Parse Exception when parsing iPOJO-Component");
        }

        assertNotNull("Check elem not null", elem);

        Element manip = getManipulationForComponent(elem, type);
        assertNotNull("Check manipulation metadata not null for " + type, manip);
        return manip;
    }

    private Element getManipulationForComponent(Element metadata, String comp_name) {
        Element[] comps = metadata.getElements("component");
        for (Element comp : comps) {
            if (comp.containsAttribute("factory") && comp.getAttribute("factory").equals(comp_name)) {
                return comp.getElements("manipulation")[0];
            }
            if (comp.containsAttribute("name") && comp.getAttribute("name").equals(comp_name)) {
                return comp.getElements("manipulation")[0];
            }
        }
        return null;
    }

    @Test
    public void testNothing() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getNothing());
            cf.start();
            ComponentInstance ci = cf.createComponentInstance(props);
            ci.dispose();
            cf.stop();
            fail("A service requirement with neither field and method must be rejected " + cf);
        } catch (ConfigurationException e) {
            // OK
        } catch (UnacceptableConfiguration e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        } catch (MissingHandlerException e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        }
    }

    @Test
    public void testNoField() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getNoField());
            cf.start();
            ComponentInstance ci = cf.createComponentInstance(props);
            ci.dispose();
            cf.stop();
            fail("A service requirement with neither field and method must be rejected " + cf);
        } catch (ConfigurationException e) {
            // OK
        } catch (UnacceptableConfiguration e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        } catch (MissingHandlerException e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        }
    }

    @Test
    public void testBadField() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getBadField());
            cf.start();
            ComponentInstance ci = cf.createComponentInstance(props);
            ci.dispose();
            cf.stop();
            fail("A service requirement with a bad field must be rejected " + cf);
        } catch (ConfigurationException e) {
            // OK
        } catch (UnacceptableConfiguration e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        } catch (MissingHandlerException e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        }
    }

    @Test
    public void testBadFilter() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getBadFilter());
            cf.start();
            ComponentInstance ci = cf.createComponentInstance(props);
            ci.dispose();
            cf.stop();
            fail("A service requirement with a bad filter must be rejected " + cf);
        } catch (ConfigurationException e) {
            //OK
        } catch (UnacceptableConfiguration e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        } catch (MissingHandlerException e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        }
    }

    @Test
    public void testBadFrom() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getBadFrom());
            cf.start();
            ComponentInstance ci = cf.createComponentInstance(props);
            ci.dispose();
            cf.stop();
            fail("A service requirement with a bad from must be rejected " + cf);
        } catch (ConfigurationException e) {
            //OK
        } catch (UnacceptableConfiguration e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        } catch (MissingHandlerException e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        }
    }

    @Test
    public void testBadType() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getBadType());
            cf.start();
            ComponentInstance ci = cf.createComponentInstance(props);
            ci.dispose();
            cf.stop();
            fail("A service requirement with a bad type must be rejected " + cf);
        } catch (ConfigurationException e) {
            // OK
        } catch (UnacceptableConfiguration e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        } catch (MissingHandlerException e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        }
    }

    /**
     * Check that a component using a service dependency without service specification is rejected.
     */
    @Test
    public void testDependencyWithoutSpecification() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getMissingSpecification());
            cf.start();
            ComponentInstance ci = cf.createComponentInstance(props);
            ci.dispose();
            cf.stop();
            fail("A service requirement with a bad type must be rejected " + cf);
        } catch (ConfigurationException e) {
            // OK
        } catch (UnacceptableConfiguration e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        } catch (MissingHandlerException e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        }
    }

    /**
     * Check that a component using a service dependency with a callback without its type is rejected.
     * The type is either 'bind' or 'unbind'.
     */
    @Test
    public void testDependencyWithACallbackWithoutType() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getMissingCallbackType());
            cf.start();
            ComponentInstance ci = cf.createComponentInstance(props);
            ci.dispose();
            cf.stop();
            fail("A service requirement with a bad type must be rejected " + cf);
        } catch (ConfigurationException e) {
            // OK
        } catch (UnacceptableConfiguration e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        } catch (MissingHandlerException e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        }
    }

    /**
     * Check that a component using a service dependency with a callback without its method is rejected.
     */
    @Test
    public void testDependencyWithACallbackWithoutMethod() {
        try {
            ComponentFactory cf = new ComponentFactory(osgiHelper.getContext(), getMissingCallbackMethod());
            cf.start();
            ComponentInstance ci = cf.createComponentInstance(props);
            ci.dispose();
            cf.stop();
            fail("A service requirement with a bad type must be rejected " + cf);
        } catch (ConfigurationException e) {
            // OK
        } catch (UnacceptableConfiguration e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        } catch (MissingHandlerException e) {
            fail("Unexpected exception when creating an instance : " + e.getMessage());
        }
    }
}
