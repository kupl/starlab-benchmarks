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

import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.JspTagException;
import jakarta.servlet.jsp.tagext.BodyTagSupport;
import jakarta.servlet.jsp.tagext.Tag;

import org.apache.taglibs.standard.resources.Resources;

/**
 * Support for tag handlers for &lt;param&gt;, the message argument
 * subtag in JSTL 1.0 which supplies an argument for parametric replacement
 * to its parent &lt;message&gt; tag.
 *
 * @author Jan Luehe
 * @see MessageSupport
 */

public abstract class ParamSupport extends BodyTagSupport {

    //*********************************************************************
    // Protected state

    protected Object value;                          // 'value' attribute
    protected boolean valueSpecified;                 // status


    //*********************************************************************
    // Constructor and initialization

    public ParamSupport() {
        super();
        init();
    }

    private void init() {
        value = null;
        valueSpecified = false;
    }


    //*********************************************************************
    // Tag logic

    // Supply our value to our parent <fmt:message> tag

    @Override
    public int doEndTag() throws JspException {
        Tag t = findAncestorWithClass(this, MessageSupport.class);
        if (t == null) {
            throw new JspTagException(Resources.getMessage(
                    "PARAM_OUTSIDE_MESSAGE"));
        }
        MessageSupport parent = (MessageSupport) t;

        /*
       * Get argument from 'value' attribute or body, as appropriate, and
       * add it to enclosing <fmt:message> tag, even if it is null or equal
       * to "".
       */
        Object input = null;
        // determine the input by...
        if (valueSpecified) {
            // ... reading 'value' attribute
            input = value;
        } else {
            // ... retrieving and trimming our body (TLV has ensured that it's
            // non-empty)
            input = bodyContent.getString().trim();
        }
        parent.addParam(input);

        return EVAL_PAGE;
    }

    // Releases any resources we may have (or inherit)

    @Override
    public void release() {
        init();
    }
}
