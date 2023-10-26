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

package org.apache.tiles.definition.pattern.wildcard;

import org.apache.tiles.Definition;
import org.apache.tiles.definition.pattern.DefinitionPatternMatcher;
import org.apache.tiles.definition.pattern.DefinitionPatternMatcherFactory;
import org.apache.tiles.definition.pattern.PatternRecognizer;
import org.apache.tiles.util.WildcardHelper;

/**
 * Creates instances of {@link WildcardDefinitionPatternMatcher}.
 *
 * @version $Rev$ $Date$
 * @since 2.2.0
 */
public class WildcardDefinitionPatternMatcherFactory implements
        DefinitionPatternMatcherFactory, PatternRecognizer {

    /**
     * Allows to parse wildcard expressions and to recognize substitution
     * variables.
     */
    private WildcardHelper wildcardHelper = new WildcardHelper();

    /** {@inheritDoc} */
    public DefinitionPatternMatcher createDefinitionPatternMatcher(
            String pattern, Definition definition) {
        return new WildcardDefinitionPatternMatcher(pattern, definition, wildcardHelper);
    }

    /** {@inheritDoc} */
    public boolean isPatternRecognized(String candidatePattern) {
        return candidatePattern.indexOf('*') >= 0;
    }
}
