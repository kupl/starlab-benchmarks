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

package org.apache.tiles.definition.pattern.wildcard;

import static org.junit.Assert.*;

import org.apache.tiles.Attribute;
import org.apache.tiles.Definition;
import org.apache.tiles.definition.pattern.DefinitionPatternMatcher;
import org.apache.tiles.util.WildcardHelper;
import org.junit.Test;

/**
 * Tests {@link WildcardDefinitionPatternMatcher}.
 *
 * @version $Rev$ $Date$
 * @since 2.2.0
 */
public class WildcardDefinitionPatternMatcherTest {

    /**
     * Test method for {@link WildcardDefinitionPatternMatcher#createDefinition(String)}.
     */
    @Test
    public void testResolveDefinition() {
        Definition def = new Definition();
        def.setName("testDef*.message*");
        def.setTemplateAttribute(Attribute.createTemplateAttribute("/test{1}.jsp"));
        def.putAttribute("body", new Attribute("message{2}"));
        DefinitionPatternMatcher patternMatcher = new WildcardDefinitionPatternMatcher(
                "testDef*.message*", def, new WildcardHelper());
        Definition result = patternMatcher.createDefinition("testDefOne.messageTwo");
        assertNotNull(result);
        assertEquals("testDefOne.messageTwo", result.getName());
        assertEquals("/testOne.jsp", result.getTemplateAttribute().getValue());
        assertEquals("messageTwo", result.getAttribute("body").getValue());
    }
}
