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

package org.apache.taglibs.standard.tag.el.fmt;

import java.util.Locale;

import jakarta.servlet.jsp.JspException;

import org.apache.taglibs.standard.lang.support.ExpressionEvaluatorManager;
import org.apache.taglibs.standard.tag.common.fmt.LocaleUtil;
import org.apache.taglibs.standard.tag.common.fmt.ParseDateSupport;

/**
 * <p>A handler for &lt;parseDate&gt; that accepts attributes as Strings
 * and evaluates them as expressions at runtime.</p>
 *
 * @author Jan Luehe
 */

public class ParseDateTag extends ParseDateSupport {

    //*********************************************************************
    // 'Private' state (implementation details)

    private String value_;                       // stores EL-based property
    private String type_;                        // stores EL-based property
    private String dateStyle_;                 // stores EL-based property
    private String timeStyle_;                 // stores EL-based property
    private String pattern_;                 // stores EL-based property
    private String timeZone_;                 // stores EL-based property
    private String parseLocale_;             // stores EL-based property


    //*********************************************************************
    // Constructor

    /**
     * Constructs a new ParseDateTag.  As with TagSupport, subclasses
     * should not provide other constructors and are expected to call
     * the superclass constructor
     */
    public ParseDateTag() {
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

    public void setValue(String value_) {
        this.value_ = value_;
        this.valueSpecified = true;
    }

    // for EL-based attribute

    public void setType(String type_) {
        this.type_ = type_;
    }

    // for EL-based attribute

    public void setDateStyle(String dateStyle_) {
        this.dateStyle_ = dateStyle_;
    }

    // for EL-based attribute

    public void setTimeStyle(String timeStyle_) {
        this.timeStyle_ = timeStyle_;
    }

    // for EL-based attribute

    public void setPattern(String pattern_) {
        this.pattern_ = pattern_;
    }

    // for EL-based attribute

    public void setTimeZone(String timeZone_) {
        this.timeZone_ = timeZone_;
    }

    // for EL-based attribute

    public void setParseLocale(String parseLocale_) {
        this.parseLocale_ = parseLocale_;
    }


    //*********************************************************************
    // Private (utility) methods

    // (re)initializes state (during release() or construction)

    private void init() {
        // null implies "no expression"
        value_ = type_ = dateStyle_ = timeStyle_ = pattern_ = timeZone_ = null;
        parseLocale_ = null;
    }

    // Evaluates expressions as necessary

    private void evaluateExpressions() throws JspException {
        /* 
         * Note: we don't check for type mismatches here; we assume
         * the expression evaluator will return the expected type
         * (by virtue of knowledge we give it about what that type is).
         * A ClassCastException here is truly unexpected, so we let it
         * propagate up.
         */

        // 'value' attribute
        if (value_ != null) {
            value = (String) ExpressionEvaluatorManager.evaluate(
                    "value", value_, String.class, this, pageContext);
        }

        // 'type' attribute
        if (type_ != null) {
            type = (String) ExpressionEvaluatorManager.evaluate(
                    "type", type_, String.class, this, pageContext);
        }

        // 'dateStyle' attribute
        if (dateStyle_ != null) {
            dateStyle = (String) ExpressionEvaluatorManager.evaluate(
                    "dateStyle", dateStyle_, String.class, this, pageContext);
        }

        // 'timeStyle' attribute
        if (timeStyle_ != null) {
            timeStyle = (String) ExpressionEvaluatorManager.evaluate(
                    "timeStyle", timeStyle_, String.class, this, pageContext);
        }

        // 'pattern' attribute
        if (pattern_ != null) {
            pattern = (String) ExpressionEvaluatorManager.evaluate(
                    "pattern", pattern_, String.class, this, pageContext);
        }

        // 'timeZone' attribute
        if (timeZone_ != null) {
            timeZone = ExpressionEvaluatorManager.evaluate(
                    "timeZone", timeZone_, Object.class, this, pageContext);
        }

        // 'parseLocale' attribute
        if (parseLocale_ != null) {
            Object obj = ExpressionEvaluatorManager.evaluate(
                    "parseLocale", parseLocale_, Object.class, this, pageContext);
            Locale locale = LocaleUtil.parseLocaleAttributeValue(obj);
            if (locale != null) {
                this.parseLocale = locale;
            }
        }
    }
}
