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
package org.apache.tiles.el;

import static org.easymock.classextension.EasyMock.*;
import static org.junit.Assert.*;

import javax.el.ExpressionFactory;
import javax.servlet.ServletContext;
import javax.servlet.jsp.JspApplicationContext;
import javax.servlet.jsp.JspFactory;

import org.apache.tiles.request.ApplicationContext;
import org.junit.Test;

/**
 * Tests {@link JspExpressionFactoryFactory}.
 *
 * @version $Rev$ $Date$
 */
public class JspExpressionFactoryFactoryTest {

    /**
     * Test method for {@link org.apache.tiles.el.JspExpressionFactoryFactory#getExpressionFactory()}.
     */
    @Test
    public void testGetExpressionFactory() {
        ApplicationContext applicationContext = createMock(ApplicationContext.class);
        ServletContext servletContext = createMock(ServletContext.class);
        JspFactory jspFactory = createMock(JspFactory.class);
        JspApplicationContext jspApplicationContext = createMock(JspApplicationContext.class);
        ExpressionFactory expressionFactory = createMock(ExpressionFactory.class);

        expect(applicationContext.getContext()).andReturn(servletContext);
        expect(jspFactory.getJspApplicationContext(servletContext)).andReturn(jspApplicationContext);
        expect(jspApplicationContext.getExpressionFactory()).andReturn(expressionFactory);

        replay(applicationContext, servletContext, jspFactory,
                jspApplicationContext, expressionFactory);
        JspFactory.setDefaultFactory(jspFactory);
        JspExpressionFactoryFactory factory = new JspExpressionFactoryFactory();
        factory.setApplicationContext(applicationContext);
        assertEquals(expressionFactory, factory.getExpressionFactory());
        verify(applicationContext, servletContext, jspFactory,
                jspApplicationContext, expressionFactory);
    }

    /**
     * Test method for {@link org.apache.tiles.el.JspExpressionFactoryFactory#getExpressionFactory()}.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testSetApplicationContextIllegal() {
        ApplicationContext applicationContext = createMock(ApplicationContext.class);
        Integer servletContext = new Integer(1);

        expect(applicationContext.getContext()).andReturn(servletContext);

        replay(applicationContext);
        try {
            JspExpressionFactoryFactory factory = new JspExpressionFactoryFactory();
            factory.setApplicationContext(applicationContext);
        } finally {
            verify(applicationContext);
        }
    }

}
