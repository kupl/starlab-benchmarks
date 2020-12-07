/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. The ASF
 * licenses this file to You under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package org.apache.sling.feature.maven;

import java.util.Map;
import java.util.TreeMap;

import org.apache.maven.model.Plugin;
import org.apache.maven.project.MavenProject;
import org.apache.sling.feature.Feature;

public class FeatureProjectInfo {

    public MavenProject project;
    public Plugin       plugin;

    public boolean featureDone = false;
    public boolean testFeatureDone = false;

    public final Map<String, Feature> features = new TreeMap<>();
    public final Map<String, Feature> assembledFeatures = new TreeMap<>();

    public final Map<String, Feature> testFeatures = new TreeMap<>();
    public final Map<String, Feature> assembledTestFeatures = new TreeMap<>();
}

