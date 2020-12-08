/*
 * Copyright (c) OSGi Alliance (2016). All Rights Reserved.
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
package org.apache.felix.schematizer;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.lang.reflect.Field;
import java.lang.reflect.Type;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

import org.osgi.util.converter.TypeReference;

public interface Node {

    @Retention(RetentionPolicy.RUNTIME)
    @Target(ElementType.FIELD)
    public static @interface CollectionType {
        Class<?> value() default Object.class;
    }

    static class DTO extends org.osgi.dto.DTO {
        public String name;
        public String path;
        public String type;
        public String collectionType;
        public boolean isCollection;
        public Map<String, Node.DTO> children = new HashMap<>();
    }

    String name();
    /**
     * Return the absolute path of this Node relative to the root Node.
     */
    String absolutePath();
    Type type();
    Field field();
    Optional<TypeReference<?>> typeReference();
    boolean isCollection();
    Map<String, Node> children();
    Class<? extends Collection<?>> collectionType();

    static Node ERROR = new Node() {
        @Override public String name() { return "ERROR"; }
        @Override public String absolutePath() { return "ERROR"; }
        @Override public Type type() { return Object.class; }
        @Override public Field field() { return null; }
        @Override public Optional<TypeReference<?>> typeReference() { return Optional.empty(); }
        @Override public boolean isCollection() { return false; }
        @Override public Map<String, Node> children() { return Collections.emptyMap(); }
        @Override public Class<? extends Collection<?>> collectionType() { return null; }        
    };
}
