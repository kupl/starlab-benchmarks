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

package org.apache.taglibs.standard.tag.compat.sql;

import jakarta.el.ValueExpression;
import jakarta.servlet.jsp.JspException;

import org.apache.taglibs.standard.tag.common.sql.TransactionTagSupport;
import org.apache.taglibs.standard.util.ExpressionUtil;

/**
 * Implementation of JSTL 1.0 {@code <transaction>} using the container's EL implementation.
 */
public class TransactionTag extends TransactionTagSupport {
    private ValueExpression dataSourceExpression;
    private ValueExpression isolationExpression;

    @Override
    public int doStartTag() throws JspException {
        rawDataSource = ExpressionUtil.evaluate(dataSourceExpression, pageContext);
        if (isolationExpression != null) {
            super.setIsolation((String) ExpressionUtil.evaluate(isolationExpression, pageContext));
        }

        return super.doStartTag();
    }

    @Override
    public void release() {
        super.release();

        dataSourceExpression = null;
        isolationExpression = null;
    }

    public void setDataSource(String expression) {
        dataSourceExpression = ExpressionUtil.createValueExpression(pageContext, expression, Object.class);
        dataSourceSpecified = true;
    }

    public void setIsolation(String expression) {
        isolationExpression = ExpressionUtil.createValueExpression(pageContext, expression, String.class);
    }
}
