/*
 * $Id$
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
package org.apache.tiles.autotag.plugin;

import java.util.Map;

import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.plugins.annotations.ResolutionScope;
import org.apache.tiles.autotag.freemarker.FMTemplateGeneratorFactory;
import org.apache.tiles.autotag.generate.TemplateGeneratorBuilder;
import org.apache.tiles.autotag.generate.TemplateGeneratorFactory;
import org.apache.velocity.app.VelocityEngine;

/**
 * Generates Freemarker code.
 */
@Mojo(
	name = "generate-freemarker",
	defaultPhase = LifecyclePhase.GENERATE_SOURCES,
	requiresDependencyResolution = ResolutionScope.COMPILE)
public class GenerateFreemarkerMojo extends AbstractGenerateMojo {

    /**
     * Name of the Runtime.
     */
	@Parameter(defaultValue = "org.apache.tiles.autotag.freemarker.runtime.Runtime", required = true)
    String freemarkerRuntime;

    /** {@inheritDoc} */
    @Override
    protected Map<String, String> getParameters() {
        return null;
    }

    /** {@inheritDoc} */
    @Override
    protected String getRuntimeClass() {
        return freemarkerRuntime;
    }

    @Override
    protected TemplateGeneratorFactory createTemplateGeneratorFactory(
            VelocityEngine velocityEngine) {
        return new FMTemplateGeneratorFactory(classesOutputLocator,
                velocityEngine, TemplateGeneratorBuilder.createNewInstance());
    }
}
