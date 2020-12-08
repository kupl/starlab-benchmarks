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

package org.apache.felix.ipojo.runtime.core.test.components.proxy;

import org.apache.felix.ipojo.runtime.core.test.services.CheckService;
import org.apache.felix.ipojo.runtime.core.test.services.FooService;
import org.osgi.framework.BundleContext;

import java.util.Properties;

public class CheckServiceNoDelegate implements CheckService {

    private FooService fs;
    
    private Helper helper;
    
    private BundleContext context;
    
    public CheckServiceNoDelegate(BundleContext bc) {
       context = bc;
       helper = new Helper(context, fs);
    }
    
    public void start() {
        helper.publish();
    }
    
    public void stop() {
        helper.unpublish();
    }
    
    public boolean check() {
        // Don't access the service
        // Just delegate
        return helper.check();
    }

    public Properties getProps() {
        // Don't access the service
        // Just delegate
        return helper.getProps();
    }

}
