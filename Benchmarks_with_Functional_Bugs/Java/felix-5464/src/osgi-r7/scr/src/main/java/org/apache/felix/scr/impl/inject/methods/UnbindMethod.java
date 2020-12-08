/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
package org.apache.felix.scr.impl.inject.methods;

import org.apache.felix.scr.impl.metadata.DSVersion;


/**
 * Component method to be invoked on service unbinding.
 */
public class UnbindMethod extends BindMethod
implements org.apache.felix.scr.impl.inject.ReferenceMethod
{

    public UnbindMethod( final String methodName,
            final Class<?> componentClass, final String referenceClassName, final DSVersion dsVersion, final boolean configurableServiceProperties )
    {
        super( methodName, componentClass, referenceClassName, dsVersion, configurableServiceProperties );
    }


    @Override
    protected String getMethodNamePrefix()
    {
        return "unbind";
    }

}