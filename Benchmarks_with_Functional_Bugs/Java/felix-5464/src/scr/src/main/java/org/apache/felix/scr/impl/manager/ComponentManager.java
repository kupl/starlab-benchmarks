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
package org.apache.felix.scr.impl.manager;

import java.util.List;
import java.util.Map;

import org.osgi.service.component.runtime.dto.ComponentConfigurationDTO;


public interface ComponentManager<S> {
	
    int STATE_UNSATISFIED_CONFIGURATION = ComponentConfigurationDTO.UNSATISFIED_CONFIGURATION;
    int STATE_UNSATISFIED_REFERENCE = ComponentConfigurationDTO.UNSATISFIED_REFERENCE;
    int STATE_SATISFIED = ComponentConfigurationDTO.SATISFIED;
	int STATE_ACTIVE = ComponentConfigurationDTO.ACTIVE;
	int STATE_DISPOSED = 32;
	int STATE_DISABLED = 64; //TODO????

	Map<String, Object> getProperties();

	long getId();

	int getSpecState();
	
	List<? extends ReferenceManager<S, ?>> getReferenceManagers();
	
}