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
package org.apache.felix.bundlerepository.impl;

import org.apache.felix.bundlerepository.Capability;
import org.apache.felix.bundlerepository.Resource;


public class ResourceCapabilityImpl implements ResourceCapability
{
    private final Resource resource;
    private final Capability capability;

    public ResourceCapabilityImpl(Resource resource, Capability capability)
    {
        this.resource = resource;
        this.capability = capability;
    }

    public Resource getResource()
    {
        return resource;
    }

    public Capability getCapability()
    {
        return capability;
    }

}
