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
package org.apache.felix.ipojo.runtime.core.test.components;

import org.apache.felix.ipojo.runtime.core.test.services.CheckService;
import org.apache.felix.ipojo.runtime.core.test.services.FooService;
import org.osgi.framework.ServiceReference;

import java.util.Dictionary;
import java.util.Map;
import java.util.Properties;

public class DynCheckServiceProvider extends CheckProviderParentClass implements CheckService {
    
	FooService fs;
	
	int simpleB = 0;
	int objectB = 0;
	int refB = 0;
	int bothB = 0;
	 int mapB, mapU, dictB, dictU;

	public boolean check() {
		return fs.foo();
	}

	public Properties getProps() {
		Properties props = new Properties();
		props.put("voidB", new Integer(simpleB));
		props.put("objectB", new Integer(objectB));
		props.put("refB", new Integer(refB));
		props.put("bothB", new Integer(bothB));
		props.put("voidU", new Integer(simpleU));
		props.put("objectU", new Integer(objectU));
		props.put("refU", new Integer(refU));
		props.put("bothU", new Integer(bothU));
		props.put("mapB", new Integer(mapB));
        props.put("mapU", new Integer(mapU));
        props.put("dictB", new Integer(dictB));
        props.put("dictU", new Integer(dictU));
        
		if (fs != null) {
		    props.put("int", new Integer(fs.getInt()));
		    if(fs.getObject() != null) { props.put("object", fs.getObject()); }
		}
        props.put("static", CheckService.foo);
        props.put("class", CheckService.class.getName());
		return props;
	}
	
	private void voidBind() {
		simpleB++;
	}
	
	protected void objectBind(FooService o) {
	    if (o == null) {
	        System.err.println("Bind receive null !!! ");
	        return;
	    }
		if(o != null && o instanceof FooService) { objectB++; }
	}
	
	public void refBind(ServiceReference sr) {
		if(sr != null) { refB++; }
	}
	
    public void bothBind(FooService o, ServiceReference sr) {
	    if(sr != null && o != null && o instanceof FooService) { bothB++; }
	}
    
    protected void propertiesMapBind(FooService o, Map props) {
        if(props != null && o != null && o instanceof FooService && props.size() > 0) { mapB++; }
    }   
   
    
    protected void propertiesDictionaryBind(FooService o, Dictionary props) {
        if(props != null && o != null && o instanceof FooService && props.size() > 0) { dictB++; }
    }   
    

}
