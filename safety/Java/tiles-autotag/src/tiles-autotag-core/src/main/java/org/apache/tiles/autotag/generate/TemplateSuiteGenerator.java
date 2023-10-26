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
package org.apache.tiles.autotag.generate;

import java.util.Map;

import org.apache.tiles.autotag.core.OutputLocator;
import org.apache.tiles.autotag.model.TemplateSuite;

/**
 * Generates code from a template suite.
 *
 * @version $Rev$ $Date$
 */
public interface TemplateSuiteGenerator {

    /**
     * Generates the code.
     *
     * @param outputLocator The base directory where the code will be put.
     * @param packageName The package name.
     * @param suite The template suite.
     * @param parameters Configuration parameters.
     */
    void generate(OutputLocator outputLocator, String packageName, TemplateSuite suite, Map<String, String> parameters);
}
