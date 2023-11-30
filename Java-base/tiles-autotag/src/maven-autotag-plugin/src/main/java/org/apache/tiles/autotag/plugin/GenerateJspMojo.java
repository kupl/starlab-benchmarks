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

/*
 * Copyright 2001-2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import java.util.HashMap;
import java.util.Map;

import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.plugins.annotations.ResolutionScope;
import org.apache.tiles.autotag.generate.TemplateGeneratorBuilder;
import org.apache.tiles.autotag.generate.TemplateGeneratorFactory;
import org.apache.tiles.autotag.jsp.JspTemplateGeneratorFactory;
import org.apache.velocity.app.VelocityEngine;


/**
 * Goal which touches a timestamp file.
 */
@Mojo(
	name = "generate-jsp",
	defaultPhase = LifecyclePhase.GENERATE_SOURCES,
	requiresDependencyResolution = ResolutionScope.COMPILE)
public class GenerateJspMojo extends AbstractGenerateMojo {

    /**
     * URI of the tag library.
     */
	@Parameter(required = true)
    String taglibURI;

    /**
     * Name of the Runtime.
     */
	@Parameter(defaultValue = "org.apache.tiles.autotag.jsp.runtime.Runtime", required = true)
    String jspRuntime;

    /** {@inheritDoc} */
    @Override
    protected Map<String, String> getParameters() {
        Map<String, String> params = new HashMap<String, String>();
        params.put("taglibURI", taglibURI);
        return params;
    }

    /** {@inheritDoc} */
    @Override
    protected String getRuntimeClass() {
        return jspRuntime;
    }

    @Override
    protected TemplateGeneratorFactory createTemplateGeneratorFactory(
            VelocityEngine velocityEngine) {
        return new JspTemplateGeneratorFactory(classesOutputLocator,
                resourcesOutputLocator, velocityEngine,
                TemplateGeneratorBuilder.createNewInstance());
    }
}
