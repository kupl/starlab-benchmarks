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

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

/**
 * <p>Represents a function call.</p>
 *
 * @author Shawn Bayern (in the style of Nathan's other classes)
 */

public class FunctionInvocation
        extends Expression {
    //-------------------------------------
    // Properties
    //-------------------------------------
    // property index

    private String functionName;
    private List argumentList;

    public String getFunctionName() {
        return functionName;
    }

    public void setFunctionName(String f) {
        functionName = f;
    }

    public List getArgumentList() {
        return argumentList;
    }

    public void setArgumentList(List l) {
        argumentList = l;
    }

    //-------------------------------------

    /**
     * Constructor
     */
    public FunctionInvocation(String functionName, List argumentList) {
        this.functionName = functionName;
        this.argumentList = argumentList;
    }

    //-------------------------------------
    // Expression methods
    //-------------------------------------

    /**
     * Returns the expression in the expression language syntax
     */
    public String getExpressionString() {
        StringBuilder b = new StringBuilder();
        b.append(functionName);
        b.append("(");
        Iterator i = argumentList.iterator();
        while (i.hasNext()) {
            b.append(((Expression) i.next()).getExpressionString());
            if (i.hasNext()) {
                b.append(", ");
            }
        }
        b.append(")");
        return b.toString();
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

        // if the Map is null, then the function is invalid
        if (functions == null) {
            pLogger.logError(Constants.UNKNOWN_FUNCTION, functionName);
        }

        // normalize function name against default prefix
        String functionName = this.functionName;
        if (functionName.indexOf(":") == -1) {
            if (defaultPrefix == null) {
                pLogger.logError(Constants.UNKNOWN_FUNCTION, functionName);
            }
            functionName = defaultPrefix + ":" + functionName;
        }

        // ensure that the function's name is mapped
        Method target = (Method) functions.get(functionName);
        if (target == null) {
            pLogger.logError(Constants.UNKNOWN_FUNCTION, functionName);
        }

        // ensure that the number of arguments matches the number of parameters
        Class[] params = target.getParameterTypes();
        if (params.length != argumentList.size()) {
            pLogger.logError(Constants.INAPPROPRIATE_FUNCTION_ARG_COUNT,
                    new Integer(params.length),
                    new Integer(argumentList.size()));
        }

        // now, walk through each parameter, evaluating and casting its argument
        Object[] arguments = new Object[argumentList.size()];
        for (int i = 0; i < params.length; i++) {
            // evaluate
            arguments[i] = ((Expression) argumentList.get(i)).evaluate(pContext,
                    pResolver,
                    functions,
                    defaultPrefix,
                    pLogger);
            // coerce
            arguments[i] = Coercions.coerce(arguments[i], params[i], pLogger);
        }

        // finally, invoke the target method, which we know to be static
        try {
            return (target.invoke(null, arguments));
        } catch (InvocationTargetException ex) {
            pLogger.logError(Constants.FUNCTION_INVOCATION_ERROR,
                    ex.getTargetException(),
                    functionName);
            return null;
        } catch (Exception ex) {
            pLogger.logError(Constants.FUNCTION_INVOCATION_ERROR, ex, functionName);
            return null;
        }
    }

    //-------------------------------------
}
