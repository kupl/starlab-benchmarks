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

import java.io.IOException;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.MissingResourceException;
import java.util.ResourceBundle;

import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.JspTagException;
import jakarta.servlet.jsp.PageContext;
import jakarta.servlet.jsp.jstl.fmt.LocalizationContext;
import jakarta.servlet.jsp.tagext.BodyTagSupport;
import jakarta.servlet.jsp.tagext.Tag;

import org.apache.taglibs.standard.tag.common.core.Util;

/**
 * Support for tag handlers for &lt;message&gt;, the message formatting tag
 * in JSTL 1.0.
 *
 * @author Jan Luehe
 */

public abstract class MessageSupport extends BodyTagSupport {

    //*********************************************************************
    // Public constants

    public static final String UNDEFINED_KEY = "???";


    //*********************************************************************
    // Protected state

    protected String keyAttrValue;       // 'key' attribute value
    protected boolean keySpecified;     // 'key' attribute specified
    protected LocalizationContext bundleAttrValue; // 'bundle' attribute value
    protected boolean bundleSpecified;   // 'bundle' attribute specified?


    //*********************************************************************
    // Private state

    private String var;                           // 'var' attribute
    private int scope;                            // 'scope' attribute
    private List params;


    //*********************************************************************
    // Constructor and initialization

    public MessageSupport() {
        super();
        params = new ArrayList();
        init();
    }

    private void init() {
        var = null;
        scope = PageContext.PAGE_SCOPE;
        keyAttrValue = null;
        keySpecified = false;
        bundleAttrValue = null;
        bundleSpecified = false;
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

    /**
     * Adds an argument (for parametric replacement) to this tag's message.
     *
     * @see ParamSupport
     */
    public void addParam(Object arg) {
        params.add(arg);
    }


    //*********************************************************************
    // Tag logic

    @Override
    public int doStartTag() throws JspException {
        params.clear();
        return EVAL_BODY_BUFFERED;
    }

    @Override
    public int doEndTag() throws JspException {

        String key = null;
        LocalizationContext locCtxt = null;

        // determine the message key by...
        if (keySpecified) {
            // ... reading 'key' attribute
            key = keyAttrValue;
        } else {
            // ... retrieving and trimming our body
            if (bodyContent != null && bodyContent.getString() != null) {
                key = bodyContent.getString().trim();
            }
        }

        if ((key == null) || key.equals("")) {
            try {
                pageContext.getOut().print("??????");
            } catch (IOException ioe) {
                throw new JspTagException(ioe.toString(), ioe);
            }
            return EVAL_PAGE;
        }

        String prefix = null;
        if (!bundleSpecified) {
            Tag t = findAncestorWithClass(this, BundleSupport.class);
            if (t != null) {
                // use resource bundle from parent <bundle> tag
                BundleSupport parent = (BundleSupport) t;
                locCtxt = parent.getLocalizationContext();
                prefix = parent.getPrefix();
            } else {
                locCtxt = BundleSupport.getLocalizationContext(pageContext);
            }
        } else {
            // localization context taken from 'bundle' attribute
            locCtxt = bundleAttrValue;
            if (locCtxt.getLocale() != null) {
                SetLocaleSupport.setResponseLocale(pageContext,
                        locCtxt.getLocale());
            }
        }

        String message = UNDEFINED_KEY + key + UNDEFINED_KEY;
        if (locCtxt != null) {
            ResourceBundle bundle = locCtxt.getResourceBundle();
            if (bundle != null) {
                try {
                    // prepend 'prefix' attribute from parent bundle
                    if (prefix != null) {
                        key = prefix + key;
                    }
                    message = bundle.getString(key);
                    // Perform parametric replacement if required
                    if (!params.isEmpty()) {
                        Object[] messageArgs = params.toArray();
                        MessageFormat formatter = new MessageFormat(""); // empty pattern, default Locale
                        if (locCtxt.getLocale() != null) {
                            formatter.setLocale(locCtxt.getLocale());
                        } else {
                            // For consistency with the <fmt:formatXXX> actions,
                            // we try to get a locale that matches the user's preferences
                            // as well as the locales supported by 'date' and 'number'.
                            //System.out.println("LOCALE-LESS LOCCTXT: GETTING FORMATTING LOCALE");
                            Locale locale = SetLocaleSupport.getFormattingLocale(pageContext);
                            //System.out.println("LOCALE: " + locale);
                            if (locale != null) {
                                formatter.setLocale(locale);
                            }
                        }
                        formatter.applyPattern(message);
                        message = formatter.format(messageArgs);
                    }
                } catch (MissingResourceException mre) {
                    message = UNDEFINED_KEY + key + UNDEFINED_KEY;
                }
            }
        }

        if (var != null) {
            pageContext.setAttribute(var, message, scope);
        } else {
            try {
                pageContext.getOut().print(message);
            } catch (IOException ioe) {
                throw new JspTagException(ioe.toString(), ioe);
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
