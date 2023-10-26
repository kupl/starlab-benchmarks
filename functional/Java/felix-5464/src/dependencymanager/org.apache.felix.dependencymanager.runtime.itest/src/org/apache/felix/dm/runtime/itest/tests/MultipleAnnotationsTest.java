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
import org.apache.felix.dm.runtime.itest.components.MultipleAnnotations;
import org.osgi.framework.ServiceRegistration;

/**
 * Use case: Verify complex Annotation usage.
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@SuppressWarnings("rawtypes")
public class MultipleAnnotationsTest extends TestBase {
    
    public void testMultipleAnnotations() {
        Ensure e = new Ensure();
        ServiceRegistration sr = register(e, MultipleAnnotations.ENSURE);
        e.waitForStep(7, 10000);
        sr.unregister();
        e.waitForStep(11, 10000);
    }
}
