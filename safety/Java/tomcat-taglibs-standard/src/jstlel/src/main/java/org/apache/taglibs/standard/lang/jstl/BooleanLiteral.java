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

/**
 * <p>An expression representing a boolean literal value
 *
 * @author Nathan Abramson - Art Technology Group
 */

public class BooleanLiteral
        extends Literal {
    //-------------------------------------
    // Member variables
    //-------------------------------------

    public static final BooleanLiteral TRUE = new BooleanLiteral("true");
    public static final BooleanLiteral FALSE = new BooleanLiteral("false");

    //-------------------------------------

    /**
     * Constructor
     */
    public BooleanLiteral(String pToken) {
        super(getValueFromToken(pToken));
    }

    //-------------------------------------

    /**
     * Parses the given token into the literal value
     */
    static Object getValueFromToken(String pToken) {
        return
                ("true".equals(pToken)) ?
                        Boolean.TRUE :
                        Boolean.FALSE;
    }

    //-------------------------------------
    // Expression methods
    //-------------------------------------

    /**
     * Returns the expression in the expression language syntax
     */
    public String getExpressionString() {
        return (getValue() == Boolean.TRUE) ? "true" : "false";
    }

    //-------------------------------------
}
