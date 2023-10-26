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
 * <p>An expression representing a literal value
 *
 * @author Nathan Abramson - Art Technology Group
 * @author Shawn Bayern
 */

public abstract class Literal
        extends Expression {
    //-------------------------------------
    // Properties
    //-------------------------------------
    // property value

    Object mValue;

    public Object getValue() {
        return mValue;
    }

    public void setValue(Object pValue) {
        mValue = pValue;
    }

    //-------------------------------------

    /**
     * Constructor
     */
    public Literal(Object pValue) {
        mValue = pValue;
    }

    //-------------------------------------
    // Expression methods
    //-------------------------------------

    /**
     * Evaluates to the literal value
     */
    public Object evaluate(Object pContext,
                           VariableResolver pResolver,
                           Map functions,
                           String defaultPrefix,
                           Logger pLogger)
            throws ELException {
        return mValue;
    }

    //-------------------------------------
}
