/*
 * $Id$
 *
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
package org.apache.tiles.autotag.model;

import static org.easymock.EasyMock.*;
import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.junit.Test;

/**
 * Tests {@link TemplateSuite}.
 *
 * @version $Rev$ $Date$
 */
public class TemplateSuiteTest {

    /**
     * Test method for {@link TemplateSuite#TemplateSuite(java.lang.String, java.lang.String)}.
     */
    @Test
    public void testTemplateSuiteCostructor1() {
        TemplateSuite suite = new TemplateSuite("name", "docs");
        assertEquals("name", suite.getName());
        assertEquals("docs", suite.getDocumentation());
        assertTrue(suite.getTemplateClasses().isEmpty());
    }

    /**
     * Test method for {@link TemplateSuite#TemplateSuite(java.lang.String, java.lang.String, java.lang.Iterable)}.
     */
    @Test
    public void testTemplateSuiteConstructor2() {
        TemplateClass class1 = createMock(TemplateClass.class);
        TemplateClass class2 = createMock(TemplateClass.class);
        expect(class1.getName()).andReturn("class1");
        expect(class2.getName()).andReturn("class2");

        replay(class1, class2);
        List<TemplateClass> classes = new ArrayList<TemplateClass>();
        classes.add(class1);
        classes.add(class2);
        TemplateSuite suite = new TemplateSuite("name", "docs", classes);
        assertEquals("name", suite.getName());
        assertEquals("docs", suite.getDocumentation());
        Iterator<TemplateClass> clazzes = suite.getTemplateClasses().iterator();
        assertSame(class1, clazzes.next());
        assertSame(class2, clazzes.next());
        assertFalse(clazzes.hasNext());
        assertSame(class1, suite.getTemplateClassByName("class1"));
        assertSame(class2, suite.getTemplateClassByName("class2"));
        verify(class1, class2);
    }

    /**
     * Test method for {@link TemplateSuite#addTemplateClass(org.apache.tiles.autotag.model.TemplateClass)}.
     */
    @Test
    public void testAddTemplateClass() {
        TemplateClass class1 = createMock(TemplateClass.class);
        TemplateClass class2 = createMock(TemplateClass.class);
        expect(class1.getName()).andReturn("class1");
        expect(class2.getName()).andReturn("class2");

        replay(class1, class2);
        List<TemplateClass> classes = new ArrayList<TemplateClass>();
        classes.add(class1);
        classes.add(class2);
        TemplateSuite suite = new TemplateSuite("name", "docs");
        assertEquals("name", suite.getName());
        assertEquals("docs", suite.getDocumentation());
        assertTrue(suite.getTemplateClasses().isEmpty());
        suite.addTemplateClass(class1);
        suite.addTemplateClass(class2);
        Iterator<TemplateClass> clazzes = suite.getTemplateClasses().iterator();
        assertSame(class1, clazzes.next());
        assertSame(class2, clazzes.next());
        assertFalse(clazzes.hasNext());
        assertSame(class1, suite.getTemplateClassByName("class1"));
        assertSame(class2, suite.getTemplateClassByName("class2"));
        verify(class1, class2);
    }

    /**
     * Test method for {@link TemplateSuite#toString()}.
     */
    @Test
    public void testToString() {
        TemplateSuite suite = new TemplateSuite("name", "docs");
        assertEquals("TemplateSuite [name=name, documentation=docs, templateClasses={}]", suite.toString());
    }
}
