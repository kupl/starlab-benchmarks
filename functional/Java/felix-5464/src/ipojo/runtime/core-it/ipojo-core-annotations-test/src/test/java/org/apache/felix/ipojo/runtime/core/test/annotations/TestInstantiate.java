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

package org.apache.felix.ipojo.runtime.core.test.annotations;

import org.apache.felix.ipojo.metadata.Element;
import org.apache.felix.ipojo.parser.ManifestMetadataParser;
import org.apache.felix.ipojo.parser.ParseException;
import org.junit.Test;
import org.osgi.framework.Bundle;
import org.osgi.framework.InvalidSyntaxException;
import org.osgi.framework.ServiceReference;

import java.util.ArrayList;
import java.util.List;

import static junit.framework.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;

public class TestInstantiate extends Common {


    @Test
    public void testInstantiateSimple() {
        Element[] meta = getInstanceMetadata(getTestBundle(),
                "org.apache.felix.ipojo.runtime.core.test" +
                        ".components" +
                        ".InstantiateSimple");
        assertNotNull(meta);
        assertEquals(1, meta.length);
        assertNull(meta[0].getAttribute("name"));
        assertEquals(0, meta[0].getElements().length);
    }

    @Test
    public void testInstantiateWithName() {
        // Check instance tag
        Element[] meta = getInstanceMetadata(getTestBundle(), "org.apache.felix.ipojo.runtime.core.test.components" +
                ".InstantiateWithName");
        assertNotNull(meta);
        assertEquals(1, meta.length);
        assertNotNull(meta[0].getAttribute("name"));
        assertEquals("myInstantiatedInstance", meta[0].getAttribute("name"));
        assertEquals(0, meta[0].getElements().length);
    }

    @Test
    public void testInstanceCreation() throws InvalidSyntaxException {
        String in = "org.apache.felix.ipojo.runtime.core.test.components.InstantiateSimple-0";
        ServiceReference ref = ipojoHelper.getServiceReferenceByName(org.apache.felix.ipojo.architecture.Architecture.class.getName(),
                in);
        assertNotNull(ref);
    }

    @Test
    public void testInstanceCreationWithName() {
        String in = "myInstantiatedInstance";
        ServiceReference ref = ipojoHelper.getServiceReferenceByName(org.apache.felix.ipojo.architecture.Architecture.class.getName(),
                in);
        assertNotNull(ref);
    }

    /**
     * Returns the instance metadatas of the component with the given name,
     * defined in the given bundle.
     *
     * @param bundle    the bundle from which the component is defined.
     * @param component the name of the defined component.
     * @return the list of instance metadata of the component with the given name,
     *         defined in the given bundle, or {@code null} if not found.
     */
    public static Element[] getInstanceMetadata(Bundle bundle, String component) {

        // Retrieves the component description from the bundle's manifest.
        String elem = (String) bundle.getHeaders().get("iPOJO-Components");
        if (elem == null) {
            throw new IllegalArgumentException(
                    "Cannot find iPOJO-Components descriptor in the specified bundle ("
                            + bundle.getSymbolicName()
                            + "). Not an iPOJO bundle.");
        }

        // Parses the retrieved description and find the component with the
        // given name.
        List<Element> list = new ArrayList<Element>();
        try {
            Element element = ManifestMetadataParser.parseHeaderMetadata(elem);
            Element[] childs = element.getElements("instance");
            for (int i = 0; i < childs.length; i++) {
                String name = childs[i].getAttribute("component");
                if (name != null && name.equalsIgnoreCase(component)) {
                    list.add(childs[i]);
                }
            }

            if (list.isEmpty()) {
                // Component not found...
                return null;
            } else {
                return (Element[]) list.toArray(new Element[list.size()]);
            }

        } catch (ParseException e) {
            throw new IllegalStateException(
                    "Cannot parse the components from specified bundle ("
                            + bundle.getSymbolicName() + "): " + e.getMessage());
        }
    }

}

