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

import jakarta.servlet.jsp.PageContext;

/**
 * <p>This is the JSTL-specific implementation of VariableResolver.
 * It looks up variable references in the PageContext, and also
 * recognizes references to implicit objects.
 *
 * @author Nathan Abramson - Art Technology Group
 */

public class JSTLVariableResolver
        implements VariableResolver {
    //-------------------------------------

    /**
     * Resolves the specified variable within the given context.
     * Returns null if the variable is not found.
     */
    public Object resolveVariable(String pName,
                                  Object pContext)
            throws ELException {
        PageContext ctx = (PageContext) pContext;

        // Check for implicit objects
        if ("pageContext".equals(pName)) {
            return ctx;
        } else if ("pageScope".equals(pName)) {
            return ImplicitObjects.
                    getImplicitObjects(ctx).
                    getPageScopeMap();
        } else if ("requestScope".equals(pName)) {
            return ImplicitObjects.
                    getImplicitObjects(ctx).
                    getRequestScopeMap();
        } else if ("sessionScope".equals(pName)) {
            return ImplicitObjects.
                    getImplicitObjects(ctx).
                    getSessionScopeMap();
        } else if ("applicationScope".equals(pName)) {
            return ImplicitObjects.
                    getImplicitObjects(ctx).
                    getApplicationScopeMap();
        } else if ("param".equals(pName)) {
            return ImplicitObjects.
                    getImplicitObjects(ctx).
                    getParamMap();
        } else if ("paramValues".equals(pName)) {
            return ImplicitObjects.
                    getImplicitObjects(ctx).
                    getParamsMap();
        } else if ("header".equals(pName)) {
            return ImplicitObjects.
                    getImplicitObjects(ctx).
                    getHeaderMap();
        } else if ("headerValues".equals(pName)) {
            return ImplicitObjects.
                    getImplicitObjects(ctx).
                    getHeadersMap();
        } else if ("initParam".equals(pName)) {
            return ImplicitObjects.
                    getImplicitObjects(ctx).
                    getInitParamMap();
        } else if ("cookie".equals(pName)) {
            return ImplicitObjects.
                    getImplicitObjects(ctx).
                    getCookieMap();
        }

        // Otherwise, just look it up in the page context
        else {
            return ctx.findAttribute(pName);
        }
    }

    //-------------------------------------
}
