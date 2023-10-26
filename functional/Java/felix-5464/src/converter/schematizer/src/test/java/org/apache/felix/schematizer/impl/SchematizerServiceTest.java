/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.felix.schematizer.impl;

import java.util.Map;

import org.apache.felix.schematizer.Node;
import org.apache.felix.schematizer.Schema;
import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.osgi.util.converter.TypeReference;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

public class SchematizerServiceTest {
    private SchematizerImpl schematizer;

    @Before
    public void setUp() {
        schematizer = new SchematizerImpl();
    }

    @After
    public void tearDown() {
        schematizer = null;
    }

    @Test
    public void testSchematizeDTO() {
        Schema s = schematizer
                .schematize("MyDTO", new TypeReference<MyDTO>(){})
                .get("MyDTO");
        assertNotNull(s);
        Node root = s.rootNode();
        assertNodeEquals("", "/", false, MyDTO.class, false, root);
        assertEquals(4, root.children().size());
        Node pingNode = root.children().get("/ping");
        assertNodeEquals("ping", "/ping", false, String.class, true, pingNode);
        Node pongNode = root.children().get("/pong");
        assertNodeEquals("pong", "/pong", false, Long.class, true, pongNode);
        Node countNode = root.children().get("/count");
        assertNodeEquals("count", "/count", false, MyDTO.Count.class, true, countNode);
        Node embeddedNode = root.children().get("/embedded");
        assertEquals(3, embeddedNode.children().size());
        assertNodeEquals("embedded", "/embedded", false, MyEmbeddedDTO.class, true, embeddedNode);
        Node marcoNode = embeddedNode.children().get("/marco");
        assertNodeEquals("marco", "/embedded/marco", false, String.class, true, marcoNode);
        Node poloNode = embeddedNode.children().get("/polo");
        assertNodeEquals("polo", "/embedded/polo", false, Long.class, true, poloNode);
        Node alphaNode = embeddedNode.children().get("/alpha");
        assertNodeEquals("alpha", "/embedded/alpha", false, MyEmbeddedDTO.Alpha.class, true, alphaNode);

        Node sRoot = s.nodeAtPath("/");
        assertNodeEquals("", "/", false, MyDTO.class, false, sRoot);
        Node sPingNode = s.nodeAtPath("/ping");
        assertNodeEquals("ping", "/ping", false, String.class, true, sPingNode);
        Node sPongNode = s.nodeAtPath("/pong");
        assertNodeEquals("pong", "/pong", false, Long.class, true, sPongNode);
        Node sCountNode = s.nodeAtPath("/count");
        assertNodeEquals("count", "/count", false, MyDTO.Count.class, true, sCountNode);
        Node sEmbeddedNode = s.nodeAtPath("/embedded");
        assertNodeEquals("embedded", "/embedded", false, MyEmbeddedDTO.class, true, sEmbeddedNode);
        Node sMarcoNode = s.nodeAtPath("/embedded/marco");
        assertNodeEquals("marco", "/embedded/marco", false, String.class, true, sMarcoNode);
        Node sPoloNode = s.nodeAtPath("/embedded/polo");
        assertNodeEquals("polo", "/embedded/polo", false, Long.class, true, sPoloNode);
        Node sAlphaNode = s.nodeAtPath("/embedded/alpha");
        assertNodeEquals("alpha", "/embedded/alpha", false, MyEmbeddedDTO.Alpha.class, true, sAlphaNode);
    }

    @Test
    public void testSchematizeDTOWithColletion() {
        Schema s = schematizer
                .type("MyDTO", "/embedded", new TypeReference<MyEmbeddedDTO2<String>>(){})
                .type("MyDTO", "/embedded/value", String.class)
                .schematize("MyDTO", new TypeReference<MyDTO3<MyEmbeddedDTO2<String>>>(){})
                .get("MyDTO");
        assertNotNull(s);
        Node root = s.rootNode();
        assertNodeEquals("", "/", false, new TypeReference<MyDTO3<MyEmbeddedDTO2<String>>>(){}.getType(), false, root);
        assertEquals(4, root.children().size());
        Node pingNode = root.children().get("/ping");
        assertNodeEquals("ping", "/ping", false, String.class, true, pingNode);
        Node pongNode = root.children().get("/pong");
        assertNodeEquals("pong", "/pong", false, Long.class, true, pongNode);
        Node countNode = root.children().get("/count");
        assertNodeEquals("count", "/count", false, MyDTO3.Count.class, true, countNode);
        Node embeddedNode = root.children().get("/embedded");
        assertEquals(1, embeddedNode.children().size());
        assertNodeEquals("embedded", "/embedded", true, new TypeReference<MyEmbeddedDTO2<String>>(){}.getType(), true, embeddedNode);
        Node valueNode = embeddedNode.children().get("/value");
        assertNodeEquals("value", "/embedded/value", false, String.class, true, valueNode);

        Node sRoot = s.nodeAtPath("/");
        assertNodeEquals("", "/", false, new TypeReference<MyDTO3<MyEmbeddedDTO2<String>>>(){}.getType(), false, sRoot);
        Node sPingNode = s.nodeAtPath("/ping");
        assertNodeEquals("ping", "/ping", false, String.class, true, sPingNode);
        Node sPongNode = s.nodeAtPath("/pong");
        assertNodeEquals("pong", "/pong", false, Long.class, true, sPongNode);
        Node sCountNode = s.nodeAtPath("/count");
        assertNodeEquals("count", "/count", false, MyDTO3.Count.class, true, sCountNode);
        Node sEmbeddedNode = s.nodeAtPath("/embedded");
        assertNodeEquals("embedded", "/embedded", true, new TypeReference<MyEmbeddedDTO2<String>>(){}.getType(), true, sEmbeddedNode);
        Node sValueNode = s.nodeAtPath("/embedded/value");
        assertNodeEquals("value", "/embedded/value", false, String.class, true, sValueNode);
    }

    @Test
    public void testSchematizeDTOWithAnnotatedColletion() {
        Schema s = schematizer
                .schematize("MyDTO4", new TypeReference<MyDTO4>(){})
                .get("MyDTO4");
        assertNotNull(s);
        Node root = s.rootNode();
        assertNodeEquals("", "/", false, MyDTO4.class, false, root);
        assertEquals(4, root.children().size());
        Node pingNode = root.children().get("/ping");
        assertNodeEquals("ping", "/ping", false, String.class, true, pingNode);
        Node pongNode = root.children().get("/pong");
        assertNodeEquals("pong", "/pong", false, Long.class, true, pongNode);
        Node countNode = root.children().get("/count");
        assertNodeEquals("count", "/count", false, MyDTO4.Count.class, true, countNode);
        Node embeddedNode = root.children().get("/embedded");
        assertEquals(3, embeddedNode.children().size());
        assertNodeEquals("embedded", "/embedded", true, MyEmbeddedDTO.class, true, embeddedNode);
        Node marcoNode = embeddedNode.children().get("/marco");
        assertNodeEquals("marco", "/embedded/marco", false, String.class, true, marcoNode);
        Node poloNode = embeddedNode.children().get("/polo");
        assertNodeEquals("polo", "/embedded/polo", false, Long.class, true, poloNode);
        Node alphaNode = embeddedNode.children().get("/alpha");
        assertNodeEquals("alpha", "/embedded/alpha", false, MyEmbeddedDTO.Alpha.class, true, alphaNode);

        Node sRoot = s.nodeAtPath("/");
        assertNodeEquals("", "/", false, MyDTO4.class, false, sRoot);
        Node sPingNode = s.nodeAtPath("/ping");
        assertNodeEquals("ping", "/ping", false, String.class, true, sPingNode);
        Node sPongNode = s.nodeAtPath("/pong");
        assertNodeEquals("pong", "/pong", false, Long.class, true, sPongNode);
        Node sCountNode = s.nodeAtPath("/count");
        assertNodeEquals("count", "/count", false, MyDTO4.Count.class, true, sCountNode);
        Node sEmbeddedNode = s.nodeAtPath("/embedded");
        assertNodeEquals("embedded", "/embedded", true, MyEmbeddedDTO.class, true, sEmbeddedNode);
        Node sMarcoNode = s.nodeAtPath("/embedded/marco");
        assertNodeEquals("marco", "/embedded/marco", false, String.class, true, sMarcoNode);
        Node sPoloNode = s.nodeAtPath("/embedded/polo");
        assertNodeEquals("polo", "/embedded/polo", false, Long.class, true, sPoloNode);
        Node sAlphaNode = s.nodeAtPath("/embedded/alpha");
        assertNodeEquals("alpha", "/embedded/alpha", false, MyEmbeddedDTO.Alpha.class, true, sAlphaNode);
    }

    @Test
    public void testSchematizeToMap() {
        Schema s = schematizer
                .schematize("MyDTO", new TypeReference<MyDTO>(){})
                .get("MyDTO");
        Map<String, Node.DTO> map = s.toMap();
        testMapValues(map);
    }

    private void testMapValues(Map<String, Node.DTO> map) {
        assertNotNull(map);
        assertEquals(1, map.size());
        Node.DTO root = map.get("/");
        assertEquals(4, root.children.size());
        assertNodeDTOEquals("", "/", false, MyDTO.class, root);
        Node.DTO pingNode = root.children.get("ping");
        assertNodeDTOEquals("ping", "/ping", false, String.class, pingNode);
        Node.DTO pongNode = root.children.get("pong");
        assertNodeDTOEquals("pong", "/pong", false, Long.class, pongNode);
        Node.DTO countNode = root.children.get("count");
        assertNodeDTOEquals("count", "/count", false, MyDTO.Count.class, countNode);
        Node.DTO embeddedNode = root.children.get("embedded");
        assertEquals(3, embeddedNode.children.size());
        assertNodeDTOEquals("embedded", "/embedded", false, MyEmbeddedDTO.class, embeddedNode);
        Node.DTO marcoNode = embeddedNode.children.get("marco");
        assertNodeDTOEquals("marco", "/embedded/marco", false, String.class, marcoNode);
        Node.DTO poloNode = embeddedNode.children.get("polo");
        assertNodeDTOEquals("polo", "/embedded/polo", false, Long.class, poloNode);
        Node.DTO alphaNode = embeddedNode.children.get("alpha");
        assertNodeDTOEquals("alpha", "/embedded/alpha", false, MyEmbeddedDTO.Alpha.class, alphaNode);
    }

    @SuppressWarnings( "unused" )
    @Test
    @Ignore("Probably no longer necessary...")
    public void testSchemaFromMap() {
        Schema s1 = schematizer
                .schematize("MyDTO", new TypeReference<MyDTO>(){})
                .get("MyDTO");
        Map<String, Node.DTO> map = s1.toMap();

//        Schema s2 = schematizer.from("MyDTO", map);
//        testSchema(s2);
    }

    @SuppressWarnings( "unused" )
    private void testSchema(Schema s) {
        // Assume that the map is serialized, then deserialized "as is".
        assertNotNull(s);
        Node root = s.rootNode();
        assertEquals(4, root.children().size());
        assertNodeEquals("", "/", false, MyDTO.class, false, root);
        Node pingNode = root.children().get("/ping");
        assertNodeEquals("ping", "/ping", false, String.class, true, pingNode);
        Node pongNode = root.children().get("/pong");
        assertNodeEquals("pong", "/pong", false, Long.class, true, pongNode);
        Node countNode = root.children().get("/count");
        assertNodeEquals("count", "/count", false, MyDTO.Count.class, true, countNode);
        Node embeddedNode = root.children().get("/embedded");
        assertEquals(3, embeddedNode.children().size());
        assertNodeEquals("embedded", "/embedded", false, MyEmbeddedDTO.class, true, embeddedNode);
        Node marcoNode = embeddedNode.children().get("/marco");
        assertNodeEquals("marco", "/embedded/marco", false, String.class, true, marcoNode);
        Node poloNode = embeddedNode.children().get("/polo");
        assertNodeEquals("polo", "/embedded/polo", false, Long.class, true, poloNode);
        Node alphaNode = embeddedNode.children().get("/alpha");
        assertNodeEquals("alpha", "/embedded/alpha", false, MyEmbeddedDTO.Alpha.class, true, alphaNode);

        Node sRoot = s.nodeAtPath("/");
        assertNodeEquals("", "/", false, MyDTO.class, false, sRoot);
        Node sPingNode = s.nodeAtPath("/ping");
        assertNodeEquals("ping", "/ping", false, String.class, true, sPingNode);
        Node sPongNode = s.nodeAtPath("/pong");
        assertNodeEquals("pong", "/pong", false, Long.class, true, sPongNode);
        Node sCountNode = s.nodeAtPath("/count");
        assertNodeEquals("count", "/count", false, MyDTO.Count.class, true, sCountNode);
        Node sEmbeddedNode = s.nodeAtPath("/embedded");
        assertNodeEquals("embedded", "/embedded", false, MyEmbeddedDTO.class, true, sEmbeddedNode);
        Node sMarcoNode = s.nodeAtPath("/embedded/marco");
        assertNodeEquals("marco", "/embedded/marco", false, String.class, true, sMarcoNode);
        Node sPoloNode = s.nodeAtPath("/embedded/polo");
        assertNodeEquals("polo", "/embedded/polo", false, Long.class, true, sPoloNode);
        Node sAlphaNode = s.nodeAtPath("/embedded/alpha");
        assertNodeEquals("alpha", "/embedded/alpha", false, MyEmbeddedDTO.Alpha.class, true, sAlphaNode);
    }

    @Test
    public void testVisitor() {
        Schema s = schematizer
                .schematize("MyDTO", new TypeReference<MyDTO>(){})
                .get("MyDTO");
        StringBuilder sb = new StringBuilder();
        s.visit( n -> sb.append("::").append(n.name()));
        assertEquals("::::count::embedded::alpha::marco::polo::ping::pong", sb.toString());
    }

    @Test
    public void testGetParentNode() {
        Schema s = schematizer
                .schematize("MyDTO", new TypeReference<MyDTO>(){})
                .get("MyDTO");
        assertNotNull(s);
        Node embeddedNode = s.nodeAtPath("/embedded/marco");
        assertTrue(!"ERROR".equals(embeddedNode.name()));
        Node parentNode = s.parentOf(embeddedNode);
        assertTrue(!"ERROR".equals(parentNode.name()));
        Node grandparentNode = s.parentOf(parentNode);
        assertTrue(!"ERROR".equals(grandparentNode.name()));
        assertEquals("/", grandparentNode.absolutePath());
    }

    @Test
    public void testTypeRules() {
        Schema s = schematizer
                .type("MyDTO", "/embedded", new TypeReference<MyEmbeddedDTO2<String>>(){})
                .type("MyDTO", "/embedded/value", String.class)
                .schematize("MyDTO", new TypeReference<MyDTO3<MyEmbeddedDTO2<String>>>(){})
                .get("MyDTO");
        assertNotNull(s);
        Node embeddedNode = s.nodeAtPath("/embedded/value");
        assertTrue(!"ERROR".equals(embeddedNode.name()));
        Node parentNode = s.parentOf(embeddedNode);
        assertTrue(!"ERROR".equals(parentNode.name()));
        Node grandparentNode = s.parentOf(parentNode);
        assertTrue(!"ERROR".equals(grandparentNode.name()));
        assertEquals("/", grandparentNode.absolutePath());
    }

    private void assertNodeEquals(String name, String path, boolean isCollection, Object type, boolean fieldNotNull, Node node) {
        assertNotNull(node);
        assertEquals(name, node.name());
        assertEquals(path, node.absolutePath());
        assertEquals(isCollection, node.isCollection());
        assertEquals(type, node.type());
        if (fieldNotNull)
            assertNotNull(node.field());
        else
            assertTrue(node.field() == null);
    }

    private void assertNodeDTOEquals(String name, String path, boolean isCollection, Class<?> type, Node.DTO node) {
        assertNotNull(node);
        assertEquals(name, node.name);
        assertEquals(path, node.path);
        assertEquals(isCollection, node.isCollection);
        assertEquals(type.getName(), node.type);
    }
}
