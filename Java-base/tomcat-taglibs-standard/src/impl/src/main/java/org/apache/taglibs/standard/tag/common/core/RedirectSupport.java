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

import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.JspTagException;
import jakarta.servlet.jsp.PageContext;
import jakarta.servlet.jsp.tagext.BodyTagSupport;

import org.apache.taglibs.standard.util.UrlUtil;

/**
 * <p>Support for tag handlers for &lt;redirect&gt;, JSTL 1.0's tag
 * for redirecting to a new URL (with optional query parameters).</p>
 *
 * @author Shawn Bayern
 */

public abstract class RedirectSupport extends BodyTagSupport
        implements ParamParent {

    //*********************************************************************
    // Protected state

    protected String url;                        // 'url' attribute
    protected String context;                    // 'context' attribute

    //*********************************************************************
    // Private state

    private String var;                          // 'var' attribute
    private int scope;                 // processed 'scope' attr
    private ParamSupport.ParamManager params;     // added parameters

    //*********************************************************************
    // Constructor and initialization

    public RedirectSupport() {
        super();
        init();
    }

    private void init() {
        url = var = null;
        params = null;
        scope = PageContext.PAGE_SCOPE;
    }


    //*********************************************************************
    // Tag attributes known at translation time

    public void setVar(String var) {
        this.var = var;
    }

    public void setScope(String scope) {
        this.scope = Util.getScope(scope);
    }


    //*********************************************************************
    // Collaboration with subtags

    // inherit Javadoc

    public void addParameter(String name, String value) {
        params.addParameter(name, value);
    }


    //*********************************************************************
    // Tag logic

    // resets any parameters that might be sent

    @Override
    public int doStartTag() throws JspException {
        params = new ParamSupport.ParamManager();
        return EVAL_BODY_BUFFERED;
    }


    // gets the right value, encodes it, and prints or stores it

    @Override
    public int doEndTag() throws JspException {
        String result;                // the eventual result

        // add (already encoded) parameters
        String baseUrl = UrlSupport.resolveUrl(url, context, pageContext);
        result = params.aggregateParams(baseUrl);

        // if the URL is relative, rewrite it with 'redirect' encoding rules
        HttpServletResponse response =
                ((HttpServletResponse) pageContext.getResponse());
        if (!UrlUtil.isAbsoluteUrl(result)) {
            result = response.encodeRedirectURL(result);
        }

        // redirect!
        try {
            response.sendRedirect(result);
        } catch (java.io.IOException ex) {
            throw new JspTagException(ex.toString(), ex);
        }

        return SKIP_PAGE;
    }

    // Releases any resources we may have (or inherit)

    @Override
    public void release() {
        init();
    }
}
