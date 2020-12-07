/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.sling.feature.maven.mojos;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Properties;
import java.util.Set;

import org.apache.maven.model.Dependency;
import org.apache.sling.feature.maven.ProjectHelper;

public class Scan extends FeatureSelectionConfig {

    private Set<String> includeTasks = new HashSet<>();

    private Set<String> excludeTasks = new HashSet<>();

    private Map<String, Properties> taskConfiguration = new HashMap<>();

    private Dependency framework;

    public void setIncludeTask(final String name) {
        this.includeTasks.add(name);
    }

    public void setExcludeTask(final String name) {
        this.excludeTasks.add(name);
    }

    public Set<String> getIncludeTasks() {
        return this.includeTasks.isEmpty() ? null : this.includeTasks;
    }

    public Set<String> getExcludeTasks() {
        return this.excludeTasks.isEmpty() ? null : this.excludeTasks;
    }

    public Map<String, Map<String,String>> getTaskConfiguration() {
        final Map<String, Map<String,String>> result = new HashMap<>();
        for (final Map.Entry<String, Properties> entry : taskConfiguration.entrySet()) {
            result.put(entry.getKey(), ProjectHelper.propertiesToMap(entry.getValue()));
        }
        return result;
    }

    public void setFramework(final Dependency d) {
        this.framework = d;
    }

    public Dependency getFramework() {
        return this.framework;
    }

    @Override
    public String toString() {
        return "Scan [selections=" + getSelections() + ", filesExcludes=" + getFilesExcludes()
                + "includeTasks=" + includeTasks
                + ", excludeTasks=" + excludeTasks + ", contextConfiguration=" + taskConfiguration + ", framework="
                + ProjectHelper.toString(framework) + "]";
    }
}