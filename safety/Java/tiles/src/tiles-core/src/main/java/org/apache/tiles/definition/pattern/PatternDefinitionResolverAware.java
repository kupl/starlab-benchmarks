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

package org.apache.tiles.definition.pattern;

/**
 * It indicates an object that uses a {@link PatternDefinitionResolver}.
 *
 * @param <T> The type of the customization key.
 * @version $Rev$ $Date$
 * @since 2.2.0
 */
public interface PatternDefinitionResolverAware<T> {

    /**
     * Sets the pattern definition resolver to use.
     *
     * @param definitionResolver The pattern definition resolver.
     * @since 2.2.0
     */
    void setPatternDefinitionResolver(PatternDefinitionResolver<T> definitionResolver);
}
