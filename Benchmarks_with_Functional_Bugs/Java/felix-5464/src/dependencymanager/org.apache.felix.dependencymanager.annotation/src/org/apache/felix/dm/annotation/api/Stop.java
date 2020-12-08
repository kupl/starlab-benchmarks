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
package org.apache.felix.dm.annotation.api;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Annotates a method which is invoked when the Service is being unregistered from the 
 * OSGi registry.
 * The method is called when the component's bundle is stopped, or when one of its
 * required dependency is lost, or when a {@link LifecycleController} is programatically used to
 * stop a service.
 * 
 * <h3>Usage Examples</h3>
 * <blockquote>
 * <pre>
 * &#64;Component
 * class MyComponent implements MyService {
 *     &#64;ServiceDependency
 *     private LogService logService; // Required dependency over the log service.
 *     
 *     &#64;Stop
 *     void stop() {} // We are unregistering from the OSGi registry.     
 * }
 * </pre>
 * </blockquote>
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@Retention(RetentionPolicy.CLASS)
@Target(ElementType.METHOD)
public @interface Stop
{
}
