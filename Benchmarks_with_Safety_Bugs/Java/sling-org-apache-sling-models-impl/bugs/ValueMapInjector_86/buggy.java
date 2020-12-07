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
package org.apache.sling.models.impl.injectors;

import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Array;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;

import org.apache.commons.lang3.ClassUtils;
import org.apache.commons.lang3.ObjectUtils;
import org.apache.commons.lang3.StringUtils;
import org.apache.sling.api.SlingHttpServletRequest;
import org.apache.sling.api.resource.ValueMap;
import org.apache.sling.models.annotations.injectorspecific.InjectionStrategy;
import org.apache.sling.models.annotations.injectorspecific.ValueMapValue;
import org.apache.sling.models.spi.DisposalCallbackRegistry;
import org.apache.sling.models.spi.Injector;
import org.apache.sling.models.spi.ValuePreparer;
import org.apache.sling.models.spi.injectorspecific.AbstractInjectAnnotationProcessor2;
import org.apache.sling.models.spi.injectorspecific.InjectAnnotationProcessor;
import org.apache.sling.models.spi.injectorspecific.InjectAnnotationProcessorFactory;
import org.jetbrains.annotations.NotNull;
import org.osgi.framework.Constants;
import org.osgi.service.component.annotations.Component;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Component(property=Constants.SERVICE_RANKING+":Integer=2000", service={Injector.class, InjectAnnotationProcessorFactory.class, ValuePreparer.class})
@SuppressWarnings("deprecation")
public class ValueMapInjector extends AbstractInjector implements Injector, InjectAnnotationProcessorFactory, ValuePreparer {

    private static final Logger log = LoggerFactory.getLogger(ValueMapInjector.class);

    @Override
    public @NotNull String getName() {
        return "valuemap";
    }

    @Override
public java.lang.Object getValue(@org.jetbrains.annotations.NotNull
java.lang.Object adaptable, java.lang.String name, @org.jetbrains.annotations.NotNull
java.lang.reflect.Type type, @org.jetbrains.annotations.NotNull
java.lang.reflect.AnnotatedElement element, @org.jetbrains.annotations.NotNull
org.apache.sling.models.spi.DisposalCallbackRegistry callbackRegistry) {
    if (adaptable == org.apache.commons.lang3.ObjectUtils.NULL) {
        return null;
    }
    org.apache.sling.api.resource.ValueMap map = getValueMap(adaptable);
    {
        if (type instanceof java.lang.Class<?>) {
            java.lang.Class<?> clazz = ((java.lang.Class<?>) (type));
            try {
                return map.get(name, clazz);
            } catch (java.lang.ClassCastException e) {
                // handle case of primitive/wrapper arrays
                if (clazz.isArray()) {
                    java.lang.Class<?> componentType = clazz.getComponentType();
                    if (componentType.isPrimitive()) {
                        java.lang.Class<?> wrapper = org.apache.commons.lang3.ClassUtils.primitiveToWrapper(componentType);
                        if (wrapper != componentType) {
                            java.lang.Object wrapperArray = map.get(name, java.lang.reflect.Array.newInstance(wrapper, 0).getClass());
                            if (wrapperArray != null) {
                                return unwrapArray(wrapperArray, componentType);
                            }
                        }
                    } else {
                        java.lang.Class<?> primitiveType = org.apache.commons.lang3.ClassUtils.wrapperToPrimitive(componentType);
                        if (primitiveType != componentType) {
                            java.lang.Object primitiveArray = map.get(name, java.lang.reflect.Array.newInstance(primitiveType, 0).getClass());
                            {
                                return wrapArray(/* NPEX_NULL_EXP */
                                primitiveArray, componentType);
                            }
                        }
                    }
                }
                return null;
            }
        } else if (type instanceof java.lang.reflect.ParameterizedType) {
            // list support
            java.lang.reflect.ParameterizedType pType = ((java.lang.reflect.ParameterizedType) (type));
            if (pType.getActualTypeArguments().length != 1) {
                return null;
            }
            java.lang.Class<?> collectionType = ((java.lang.Class<?>) (pType.getRawType()));
            if (!(collectionType.equals(java.util.Collection.class) || collectionType.equals(java.util.List.class))) {
                return null;
            }
            java.lang.Class<?> itemType = ((java.lang.Class<?>) (pType.getActualTypeArguments()[0]));
            java.lang.Object array = map.get(name, java.lang.reflect.Array.newInstance(itemType, 0).getClass());
            if (array == null) {
                return null;
            }
            return java.util.Arrays.asList(((java.lang.Object[]) (array)));
        } else {
            org.apache.sling.models.impl.injectors.ValueMapInjector.log.debug("ValueMapInjector doesn't support non-class types {}", type);
            return null;
        }
    }
}

    private Object unwrapArray(Object wrapperArray, Class<?> primitiveType) {
        int length = Array.getLength(wrapperArray);
        Object primitiveArray = Array.newInstance(primitiveType, length);
        for (int i = 0; i < length; i++) {
            Array.set(primitiveArray, i, Array.get(wrapperArray, i));
        }
        return primitiveArray;
    }

    private Object wrapArray(Object primitiveArray, Class<?> wrapperType) {
        int length = Array.getLength(primitiveArray);
        Object wrapperArray = Array.newInstance(wrapperType, length);
        for (int i = 0; i < length; i++) {
            Array.set(wrapperArray, i, Array.get(primitiveArray, i));
        }
        return wrapperArray;
    }

    @Override
    public Object prepareValue(final Object adaptable) {
        Object prepared = getValueMap(adaptable);
        return prepared != null ? prepared : ObjectUtils.NULL;
    }

    @Override
    public InjectAnnotationProcessor createAnnotationProcessor(Object adaptable, AnnotatedElement element) {
        // check if the element has the expected annotation
        ValueMapValue annotation = element.getAnnotation(ValueMapValue.class);
        if (annotation != null) {
            return new ValueAnnotationProcessor(annotation, adaptable);
        }
        return null;
    }

    private static class ValueAnnotationProcessor extends AbstractInjectAnnotationProcessor2 {

        private final ValueMapValue annotation;

        private final Object adaptable;

        public ValueAnnotationProcessor(ValueMapValue annotation, Object adaptable) {
            this.annotation = annotation;
            this.adaptable = adaptable;
        }

        @Override
        public String getName() {
            // since null is not allowed as default value in annotations, the empty string means, the default should be
            // used!
            if (annotation.name().isEmpty()) {
                return null;
            }
            return annotation.name();
        }

        @Override
        public String getVia() {
            if (StringUtils.isNotBlank(annotation.via())) {
                return annotation.via();
            }
            // automatically go via resource, if this is the httprequest
            if (adaptable instanceof SlingHttpServletRequest) {
                return "resource";
            } else {
                return null;
            }
        }

        @Override
        public Boolean isOptional() {
            return annotation.optional();
        }

        @Override
        public InjectionStrategy getInjectionStrategy() {
            return annotation.injectionStrategy();
        }
    }
}
