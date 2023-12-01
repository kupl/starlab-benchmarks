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

package org.apache.taglibs.standard.lang.jstl;

import java.util.Map;

/**
 * <p>Represents a name that can be used as the first element of a
 * value.
 *
 * @author Nathan Abramson - Art Technology Group
 * @author Shawn Bayern
 */

public class NamedValue
        extends Expression {
    //-------------------------------------
    // Constants
    //-------------------------------------

    //-------------------------------------
    // Properties
    //-------------------------------------
    // property name

    String mName;

    public String getName() {
        return mName;
    }

    //-------------------------------------

    /**
     * Constructor
     */
    public NamedValue(String pName) {
        mName = pName;
    }

    //-------------------------------------
    // Expression methods
    //-------------------------------------

    /**
     * Returns the expression in the expression language syntax
     */
    public String getExpressionString() {
        return StringLiteral.toIdentifierToken(mName);
    }

    //-------------------------------------

    /**
     * Evaluates by looking up the name in the VariableResolver
     */
    public Object evaluate(Object pContext,
                           VariableResolver pResolver,
                           Map functions,
                           String defaultPrefix,
                           Logger pLogger)
            throws ELException {
        if (pResolver == null) {
            return null;
        } else {
            return pResolver.resolveVariable(mName, pContext);
        }
    }

    //-------------------------------------
}
