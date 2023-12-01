/*
 * $Id$
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.apache.tiles;

import static org.apache.tiles.CompareUtil.*;

/**
 * It is an expression, along with the expression language (e.g. EL, MVEL, OGNL)
 * it is expressed with.
 *
 * @version $Rev$ $Date$
 * @since 2.2.0
 */
public class Expression {

    /**
     * The expression itself.
     */
    private String expression;

    /**
     * The language of the expression.
     */
    private String language;

    /**
     * Constructor.
     *
     * @param expression The expression itself.
     * @param language The language of the expression.
     * @since 2.2.0
     */
    public Expression(String expression, String language) {
        this.expression = expression;
        this.language = language;
    }

    /**
     * Constructor, using the default (i.e. <code>null</code>) language.
     *
     * @param expression The expression itself.
     * @since 2.2.0
     */
    public Expression(String expression) {
        this(expression, null);
    }

    /**
     * Copy constructor.
     *
     * @param toCopy The expression to copy.
     * @since 2.2.0
     */
    public Expression(Expression toCopy) {
        this.expression = toCopy.expression;
        this.language = toCopy.language;
    }

    /**
     * Creates an Expression object from a string in the form
     * <code>LANGUAGE:EXPRESSION</code>.
     *
     * @param describedExpression The expression in the form
     * <code>LANGUAGE:EXPRESSION</code>. The LANGUAGE part should be expressed
     * only with letters and numbers.
     * @return The created object, or <code>null</code> if the expression is null.
     * @since 2.2.0
     */
    public static Expression createExpressionFromDescribedExpression(String describedExpression) {
        if (describedExpression != null) {
            String language = null;
            String expression = describedExpression;
            if (describedExpression.matches("[a-zA-Z0-9]+:.+")) {
                language = describedExpression.substring(0, describedExpression.indexOf(':'));
                expression = describedExpression.substring(describedExpression.indexOf(':') + 1);
            }
            return new Expression(expression, language);
        }

        return null;
    }

    /**
     * Creates an Expression object from the expression and its language.
     *
     * @param expression The expression itself.
     * @param language The language of the expression.
     * @return The created object, or <code>null</code> if the expression is null.
     * @since 2.2.0
     */
    public static Expression createExpression(String expression, String language) {
        if (expression != null) {
            return new Expression(expression, language);
        }

        return null;
    }

    /**
     * Returns the expression string.
     *
     * @return The expression itself.
     * @since 2.2.0
     */
    public String getExpression() {
        return expression;
    }

    /**
     * Returns the language in which the expression is expressed.
     *
     * @return The expression language.
     * @since 2.2.0
     */
    public String getLanguage() {
        return language;
    }

    /** {@inheritDoc} */
    @Override
    public boolean equals(Object obj) {
        Expression exp = (Expression) obj;
        return nullSafeEquals(expression, exp.expression)
                && nullSafeEquals(language, exp.language);
    }

    /** {@inheritDoc} */
    @Override
    public int hashCode() {
        return nullSafeHashCode(expression) + nullSafeHashCode(language);
    }

    /** {@inheritDoc} */
    @Override
    public String toString() {
        return (language == null ? "DEFAULT" : language) + ":" + expression;
    }
}
