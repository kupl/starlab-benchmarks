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

import java.io.UnsupportedEncodingException;

import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.JspTagException;
import jakarta.servlet.jsp.PageContext;
import jakarta.servlet.jsp.tagext.TagSupport;

/**
 * Support for tag handlers for &lt;requestEncoding&gt;, the tag for setting
 * the request character encoding in JSTL 1.0.
 *
 * @author Jan Luehe
 * @author Pierre Delisle
 */

public abstract class RequestEncodingSupport extends TagSupport {

    //*********************************************************************
    // Package-scoped constants

    static final String REQUEST_CHAR_SET =
            "jakarta.servlet.jsp.jstl.fmt.request.charset";


    //*********************************************************************
    // Private constants

    private static final String DEFAULT_ENCODING = "ISO-8859-1";


    //*********************************************************************
    // Tag attributes

    protected String value;             // 'value' attribute


    //*********************************************************************
    // Derived information

    protected String charEncoding;   // derived from 'value' attribute  


    //*********************************************************************
    // Constructor and initialization

    public RequestEncodingSupport() {
        super();
        init();
    }

    private void init() {
        value = null;
    }


    //*********************************************************************
    // Tag logic

    @Override
    public int doEndTag() throws JspException {
        charEncoding = value;
        if ((charEncoding == null)
                && (pageContext.getRequest().getCharacterEncoding() == null)) {
            // Use charset from session-scoped attribute
            charEncoding = (String)
                    pageContext.getAttribute(REQUEST_CHAR_SET,
                            PageContext.SESSION_SCOPE);
            if (charEncoding == null) {
                // Use default encoding
                charEncoding = DEFAULT_ENCODING;
            }
        }

        /*
       * If char encoding was already set in the request, we don't need to
       * set it again.
       */
        if (charEncoding != null) {
            try {
                pageContext.getRequest().setCharacterEncoding(charEncoding);
            } catch (UnsupportedEncodingException uee) {
                throw new JspTagException(uee.toString(), uee);
            }
        }

        return EVAL_PAGE;
    }

    // Releases any resources we may have (or inherit)

    @Override
    public void release() {
        init();
    }
}
