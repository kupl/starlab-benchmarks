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
package org.apache.felix.scrplugin.helper;

import java.util.ArrayList;
import java.util.List;

public class MetatypeContainer {

    private String id;

    private String name;

    private String description;

    private String factoryPid;

    private final List<MetatypeAttributeDefinition> properties = new ArrayList<MetatypeAttributeDefinition>();

    public String getId() {
        return this.id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return this.description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public List<MetatypeAttributeDefinition> getProperties() {
        return this.properties;
    }

    public String getFactoryPid() {
        return this.factoryPid;
    }

    public void setFactoryPid(String pid) {
        this.factoryPid = pid;
    }
}
