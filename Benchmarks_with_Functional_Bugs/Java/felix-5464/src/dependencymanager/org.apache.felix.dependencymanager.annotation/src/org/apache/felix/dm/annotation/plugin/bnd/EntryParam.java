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
package org.apache.felix.dm.annotation.plugin.bnd;

/**
 * The type of parameters which can be found in a component descriptor.
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
public enum EntryParam
{
    init, 
    start, 
    stop, 
    destroy, 
    impl, 
    provides, 
    properties, 
    composition, 
    service,
    filter, 
    defaultImpl, 
    required, 
    added, 
    changed,
    removed,
    swap,
    autoConfig, 
    pid,
    pidClass,
    configType, // inject a proxy configuration type 
    factoryPid,
    factoryPidClass,
    propagate, 
    updated, 
    timeout,
    adapteeService,
    adapteeFilter,
    stateMask,
    ranking,
    factorySet,
    factoryName,
    factoryConfigure,
    factoryMethod,
    field,
    name, 
    starter,
    stopper,
    bundleContextField, 
    dependencyManagerField, 
    componentField, 
    registrationField, 
    registered, 
    unregistered,
    dereference
}
