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

package org.apache.taglibs.standard.tag.el.xml;

import jakarta.servlet.jsp.JspException;

import org.apache.taglibs.standard.tag.common.xml.ParamSupport;
import org.apache.taglibs.standard.tag.el.core.ExpressionUtil;

/**
 * <p>A handler for &lt;param&gt; that accepts attributes as Strings
 * and evaluates them as expressions at runtime.</p>
 *
 * @author Shawn Bayern
 */

public class ParamTag extends ParamSupport {

    //*********************************************************************
    // 'Private' state (implementation details)

    private String name_;                       // stores EL-based property
    private String value_;            // stores EL-based property


    //*********************************************************************
    // Constructor

    public ParamTag() {
        super();
        init();
    }


    //*********************************************************************
    // Tag logic

    // evaluates expression and chains to parent

    public int doStartTag() throws JspException {

        // evaluate any expressions we were passed, once per invocation
        evaluateExpressions();

        // chain to the parent implementation
        return super.doStartTag();
    }


    // Releases any resources we may have (or inherit)

    public void release() {
        super.release();
        init();
    }


    //*********************************************************************
    // Accessor methods

    // for EL-based attribute

    public void setName(String name_) {
        this.name_ = name_;
    }

    public void setValue(String value_) {
        this.value_ = value_;
    }


    //*********************************************************************
    // Private (utility) methods

    // (re)initializes state (during release() or construction)

    private void init() {
        // null implies "no expression"
        name_ = value_ = null;
    }

    /* Evaluates expressions as necessary */

    private void evaluateExpressions() throws JspException {
        /* 
         * Note: we don't check for type mismatches here; we assume
         * the expression evaluator will return the expected type
         * (by virtue of knowledge we give it about what that type is).
         * A ClassCastException here is truly unexpected, so we let it
         * propagate up.
         */

        name = (String) ExpressionUtil.evalNotNull(
                "param", "name", name_, String.class, this, pageContext);
        value = ExpressionUtil.evalNotNull(
                "param", "value", value_, Object.class, this, pageContext);
    }
}
