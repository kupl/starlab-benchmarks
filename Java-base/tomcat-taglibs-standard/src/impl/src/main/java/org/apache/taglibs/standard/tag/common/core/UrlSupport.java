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

import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.JspTagException;
import jakarta.servlet.jsp.PageContext;
import jakarta.servlet.jsp.tagext.BodyTagSupport;

import org.apache.taglibs.standard.resources.Resources;
import org.apache.taglibs.standard.util.UrlUtil;

import java.io.IOException;

/**
 * <p>Support for tag handlers for &lt;url&gt;, the URL creation
 * and rewriting tag in JSTL 1.0.</p>
 *
 * @author Shawn Bayern
 */

public abstract class UrlSupport extends BodyTagSupport
        implements ParamParent {

    //*********************************************************************
    // Protected state

    protected String value;                      // 'value' attribute
    protected String context;             // 'context' attribute

    //*********************************************************************
    // Private state

    private String var;                          // 'var' attribute
    private int scope;                 // processed 'scope' attr
    private ParamSupport.ParamManager params;     // added parameters

    //*********************************************************************
    // Constructor and initialization

    public UrlSupport() {
        super();
        init();
    }

    private void init() {
        value = var = null;
        params = null;
        context = null;
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
        String baseUrl = resolveUrl(value, context, pageContext);
        result = params.aggregateParams(baseUrl);

        // if the URL is relative, rewrite it
        if (!UrlUtil.isAbsoluteUrl(result)) {
            HttpServletResponse response =
                    ((HttpServletResponse) pageContext.getResponse());
            result = response.encodeURL(result);
        }

        // store or print the output
        if (var != null) {
            pageContext.setAttribute(var, result, scope);
        } else {
            try {
                pageContext.getOut().print(result);
            } catch (IOException ex) {
                throw new JspTagException(ex.toString(), ex);
            }
        }

        return EVAL_PAGE;
    }

    // Releases any resources we may have (or inherit)

    @Override
    public void release() {
        init();
    }

    //*********************************************************************
    // Utility methods

    public static String resolveUrl(
            String url, String context, PageContext pageContext)
            throws JspException {
        // don't touch absolute URLs
        if (UrlUtil.isAbsoluteUrl(url)) {
            return url;
        }

        // normalize relative URLs against a context root
        HttpServletRequest request =
                (HttpServletRequest) pageContext.getRequest();
        if (context == null) {
            if (url.startsWith("/")) {
                return (request.getContextPath() + url);
            } else {
                return url;
            }
        } else {
            if (!context.startsWith("/") || !url.startsWith("/")) {
                throw new JspTagException(
                        Resources.getMessage("IMPORT_BAD_RELATIVE"));
            }
            if (context.endsWith("/") && url.startsWith("/")) {
                // Don't produce string starting with '//', many
                // browsers interpret this as host name, not as
                // path on same host. Bug 22860
                // Also avoid // inside the url. Bug 34109
                return (context.substring(0, context.length() - 1) + url);
            } else {
                return (context + url);
            }
        }
    }

}
