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

import java.util.HashMap;
import java.util.Map;

import org.apache.maven.artifact.handler.manager.ArtifactHandlerManager;
import org.apache.maven.artifact.resolver.ArtifactResolver;
import org.apache.maven.execution.MavenSession;
import org.codehaus.plexus.logging.Logger;

/**
 * The environment provides required services to the {@link Preprocessor} as well
 * as the projects, the preprocessor should work on.
 */
public class Environment {

    public ArtifactHandlerManager artifactHandlerManager;
    public ArtifactResolver resolver;
    public MavenSession session;
    public Logger logger;

    /**
     * The projects for the preprocessor.
     * The key of this map is in the format : "{groupId}:{artifactId}"
     */
    public final Map<String, FeatureProjectInfo> modelProjects = new HashMap<>();
}

