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

package org.apache.taglibs.standard.tag.common.core;

import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.PageContext;
import jakarta.servlet.jsp.tagext.TagSupport;

/**
 * <p>A handler for the &lt;remove&gt; tag, which removes the variable
 * identified by 'var' (and 'scope', if present).
 *
 * @author Shawn Bayern
 */
public class RemoveTag extends TagSupport {

    //*********************************************************************
    // Internal state

    private int scope;                    // tag attribute
    private boolean scopeSpecified;            // ... by tag attribute
    private String var;                    // tag attribute


    //*********************************************************************
    // Construction and initialization

    /**
     * Constructs a new handler.  As with TagSupport, subclasses should
     * not provide other constructors and are expected to call the
     * superclass constructor.
     */
    public RemoveTag() {
        super();
        init();
    }

    // resets local state

    private void init() {
        var = null;
        scope = PageContext.PAGE_SCOPE;
        scopeSpecified = false;
    }

    // Releases any resources we may have (or inherit)

    @Override
    public void release() {
        super.release();
        init();
    }


    //*********************************************************************
    // Tag logic

    // removes the variable (from a specific scope, if specified)

    @Override
    public int doEndTag() throws JspException {
        if (!scopeSpecified) {
            pageContext.removeAttribute(var);
        } else {
            pageContext.removeAttribute(var, scope);
        }
        return EVAL_PAGE;
    }


    //*********************************************************************
    // Accessor methods

    // for tag attribute

    public void setVar(String var) {
        this.var = var;
    }

    // for tag attribute

    public void setScope(String scope) {
        this.scope = Util.getScope(scope);
        scopeSpecified = true;
    }
}
