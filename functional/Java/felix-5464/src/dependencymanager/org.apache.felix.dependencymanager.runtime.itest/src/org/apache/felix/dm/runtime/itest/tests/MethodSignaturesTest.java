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
package org.apache.felix.dm.runtime.itest.tests;

import org.apache.felix.dm.itest.util.Ensure;
import org.apache.felix.dm.itest.util.TestBase;
import org.apache.felix.dm.runtime.itest.components.MethodSignatures;
import org.osgi.framework.ServiceRegistration;

/**
 * Use case: Validates proper injection on various bind method signatures.
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@SuppressWarnings("rawtypes")
public class MethodSignaturesTest extends TestBase {
    /**
     * Validates ServiceDependency method signatures.
     */
    public void testServiceDependencyBindSignatures() {
        Ensure e = new Ensure();
        ServiceRegistration sr = register(e, MethodSignatures.ENSURE_SERVICE_DEPENDENCY);
        e.waitForStep(10, 5000);
        sr.unregister();
        // make sure configurations are deleted
        e.waitForStep(14, 5000);
    }
    
    /**
     * Validates FactoryConfigurationAdapter updated callback signatures.
     */
    public void testFactoryPidUpdatedSignature1() {
        Ensure e = new Ensure();
        ServiceRegistration sr = register(e, MethodSignatures.ENSURE_FACTORYPID);
        e.waitForStep(3, 5000);
        sr.unregister();
        // make sure configurations are deleted
        e.waitForStep(6, 5000);
    }
}
