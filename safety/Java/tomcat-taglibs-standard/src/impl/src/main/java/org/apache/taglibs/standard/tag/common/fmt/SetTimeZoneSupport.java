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

package org.apache.taglibs.standard.tag.common.fmt;

import java.util.TimeZone;

import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.PageContext;
import jakarta.servlet.jsp.jstl.core.Config;
import jakarta.servlet.jsp.tagext.TagSupport;

import org.apache.taglibs.standard.tag.common.core.Util;

/**
 * Support for tag handlers for &lt;setTimeZone&gt;, the time zone setting tag
 * in JSTL 1.0.
 *
 * @author Jan Luehe
 */

public abstract class SetTimeZoneSupport extends TagSupport {


    //*********************************************************************
    // Protected state

    protected Object value;                      // 'value' attribute


    //*********************************************************************
    // Private state

    private int scope;                           // 'scope' attribute
    private String var;                          // 'var' attribute


    //*********************************************************************
    // Constructor and initialization

    public SetTimeZoneSupport() {
        super();
        init();
    }

    // resets local state

    private void init() {
        value = var = null;
        scope = PageContext.PAGE_SCOPE;
    }


    //*********************************************************************
    // Tag attributes known at translation time

    public void setScope(String scope) {
        this.scope = Util.getScope(scope);
    }

    public void setVar(String var) {
        this.var = var;
    }


    //*********************************************************************
    // Tag logic

    @Override
    public int doEndTag() throws JspException {
        TimeZone timeZone = null;

        if (value == null) {
            timeZone = TimeZone.getTimeZone("GMT");
        } else if (value instanceof String) {
            if (((String) value).trim().equals("")) {
                timeZone = TimeZone.getTimeZone("GMT");
            } else {
                timeZone = TimeZone.getTimeZone((String) value);
            }
        } else {
            timeZone = (TimeZone) value;
        }

        if (var != null) {
            pageContext.setAttribute(var, timeZone, scope);
        } else {
            Config.set(pageContext, Config.FMT_TIME_ZONE, timeZone, scope);
        }

        return EVAL_PAGE;
    }

    // Releases any resources we may have (or inherit)

    @Override
    public void release() {
        init();
    }
}
