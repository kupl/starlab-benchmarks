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

import java.util.List;
import java.util.Map;

import org.apache.tiles.autotag.core.OutputLocator;
import org.apache.tiles.autotag.model.TemplateClass;
import org.apache.tiles.autotag.model.TemplateSuite;

/**
 * The basic template generator. Use {@link TemplateGeneratorBuilder} to
 * create instances of this class.
 *
 * @version $Rev$ $Date$
 */
class BasicTemplateGenerator implements TemplateGenerator {

    /**
     * The template suite generators.
     */
    private List<TSGeneratorDirectoryPair> templateSuiteGenerators;

    /**
     * The template class generators.
     */
    private List<TCGeneratorDirectoryPair> templateClassGenerators;

    /**
     * Indicates that this generator generates resources.
     */
    private boolean generatingResources = false;

    /**
     * Indicates that this generator generates classes.
     */
    private boolean generatingClasses = false;

    /**
     * Constructor.
     *
     * @param templateSuiteGenerators The template suite generators.
     * @param templateClassGenerators The template class generators.
     * @param generatingClasses Indicates that this generator generates classes.
     * @param generatingResources Indicates that this generator generates resources.
     */
    BasicTemplateGenerator(
            List<TSGeneratorDirectoryPair> templateSuiteGenerators,
            List<TCGeneratorDirectoryPair> templateClassGenerators,
            boolean generatingClasses, boolean generatingResources) {
        this.templateSuiteGenerators = templateSuiteGenerators;
        this.templateClassGenerators = templateClassGenerators;
        this.generatingClasses = generatingClasses;
        this.generatingResources = generatingResources;
    }



    @Override
    public void generate(String packageName, TemplateSuite suite, Map<String, String> parameters,
        String runtimeClass, String requestClass) {
        for (TSGeneratorDirectoryPair pair : templateSuiteGenerators) {
            pair.getGenerator().generate(pair.getOutputLocator(), packageName, suite, parameters);
        }
        for (TemplateClass templateClass : suite.getTemplateClasses()) {
            for (TCGeneratorDirectoryPair pair : templateClassGenerators) {
                pair.getGenerator().generate(pair.getOutputLocator(), packageName,
                        suite, templateClass, parameters, runtimeClass, requestClass);
            }
        }
    }

    /**
     * A pair of a template suite generator and a directory.
     *
     * @version $Rev$ $Date$
     */
    static class TSGeneratorDirectoryPair {
        /**
         * The directory where files are generated.
         */
        private OutputLocator outputLocator;

        /**
         * The generator.
         */
        private TemplateSuiteGenerator generator;

        /**
         * Constructor.
         *
         * @param directory The directory where files are generated.
         * @param generator The generator.
         */
        public TSGeneratorDirectoryPair(OutputLocator outputLocator,
                TemplateSuiteGenerator generator) {
            this.outputLocator = outputLocator;
            this.generator = generator;
        }

        /**
         * Returns the directory where files are generated.
         *
         * @return The directory where files are generated.
         */
        public OutputLocator getOutputLocator() {
            return outputLocator;
        }

        /**
         * Returns the generator.
         *
         * @return The generator.
         */
        public TemplateSuiteGenerator getGenerator() {
            return generator;
        }
    }

    /**
     * A pair of a template class generator and a directory.
     *
     * @version $Rev$ $Date$
     */
    static class TCGeneratorDirectoryPair {
        /**
         * The directory where files are generated.
         */
        private OutputLocator outputLocator;

        /**
         * The generator.
         */
        private TemplateClassGenerator generator;

        /**
         * Constructor.
         *
         * @param directory The directory where files are generated.
         * @param generator The generator.
         */
        public TCGeneratorDirectoryPair(OutputLocator outputLocator,
                TemplateClassGenerator generator) {
            this.outputLocator = outputLocator;
            this.generator = generator;
        }

        /**
         * Returns the directory where files are generated.
         *
         * @return The directory where files are generated.
         */
        public OutputLocator getOutputLocator() {
            return outputLocator;
        }

        /**
         * Returns the generator.
         *
         * @return The generator.
         */
        public TemplateClassGenerator getGenerator() {
            return generator;
        }
    }

    @Override
    public boolean isGeneratingResources() {
        return generatingResources;
    }

    @Override
    public boolean isGeneratingClasses() {
        return generatingClasses;
    }
}
