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

package org.apache.taglibs.standard.tag.el.core;

import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.JspTagException;
import jakarta.servlet.jsp.jstl.core.ConditionalTagSupport;

import org.apache.taglibs.standard.lang.support.ExpressionEvaluatorManager;
import org.apache.taglibs.standard.tag.common.core.NullAttributeException;

/**
 * <p>Tag handler for &lt;if&gt; in JSTL's expression-evaluating library.
 * Because of the support provided by the ConditionalTagSupport class,
 * thistag is trivial enough not to require a separate base supporting
 * class common to both libraries.</p>
 *
 * @author Shawn Bayern
 */

public class IfTag extends ConditionalTagSupport {

    //*********************************************************************
    // Constructor and lifecycle management

    // initialize inherited and local state

    public IfTag() {
        super();
        init();
    }

    // Releases any resources we may have (or inherit)

    public void release() {
        super.release();
        init();
    }


    //*********************************************************************
    // Supplied conditional logic

    protected boolean condition() throws JspTagException {
        try {
            Object r = ExpressionEvaluatorManager.evaluate(
                    "test", test, Boolean.class, this, pageContext);
            if (r == null) {
                throw new NullAttributeException("if", "test");
            } else {
                return (((Boolean) r).booleanValue());
            }
        } catch (JspException ex) {
            throw new JspTagException(ex.toString(), ex);
        }
    }


    //*********************************************************************
    // Private state

    private String test;               // the value of the 'test' attribute


    //*********************************************************************
    // Accessors

    // receives the tag's 'test' attribute

    public void setTest(String test) {
        this.test = test;
    }


    //*********************************************************************
    // Private utility methods

    // resets internal state

    private void init() {
        test = null;
    }
}
