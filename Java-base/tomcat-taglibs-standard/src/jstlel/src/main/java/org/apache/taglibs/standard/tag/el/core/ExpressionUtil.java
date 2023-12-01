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
import jakarta.servlet.jsp.PageContext;
import jakarta.servlet.jsp.tagext.Tag;

import org.apache.taglibs.standard.lang.support.ExpressionEvaluatorManager;
import org.apache.taglibs.standard.tag.common.core.NullAttributeException;

/**
 * <p>Contains some static utilities to facilitate common forms of
 * expression evaluation.</p>
 *
 * @author Shawn Bayern
 */

public class ExpressionUtil {

    /**
     * Evaluates an expression if present, but does not allow the expression
     * to evaluate to 'null', throwing a NullAttributeException if it
     * does.  The function <b>can</b> return null, however, if the
     * expression itself is null.
     */
    public static Object evalNotNull(String tagName,
                                     String attributeName,
                                     String expression,
                                     Class expectedType,
                                     Tag tag,
                                     PageContext pageContext)
            throws JspException {
        if (expression != null) {
            Object r = ExpressionEvaluatorManager.evaluate(
                    attributeName, expression, expectedType, tag, pageContext);
            if (r == null) {
                throw new NullAttributeException(tagName, attributeName);
            }
            return r;
        } else {
            return null;
        }
    }
}
