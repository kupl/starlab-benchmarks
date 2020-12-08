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
package org.apache.felix.scr.integration.components.activatesignature;


import java.util.HashMap;
import java.util.Map;

import org.osgi.service.component.ComponentConstants;
import org.osgi.service.component.ComponentContext;


public abstract class AbstractActivateSignatureTestComponent
{
	
	private static final  Map<String, AbstractActivateSignatureTestComponent> instances = new HashMap<String, AbstractActivateSignatureTestComponent>();
	
	public static  AbstractActivateSignatureTestComponent getInstance(String name)
	{
		return instances.get(name);
	}

    private String methodCalled;


    public String getMethodCalled()
    {
        return methodCalled;
    }


    protected void setMethodCalled( String methodCalled )
    {
        this.methodCalled = methodCalled;
        instances.put(methodCalled, this);
    }


    protected void setMethodCalled( ComponentContext context )
    {
        final String method = ( String ) context.getProperties().get( ComponentConstants.COMPONENT_NAME );
        setMethodCalled( method );
    }


    protected void setMethodCalled( Map<?, ?> context )
    {
        final String method = ( String ) context.get( ComponentConstants.COMPONENT_NAME );
        setMethodCalled( method );
    }

}
