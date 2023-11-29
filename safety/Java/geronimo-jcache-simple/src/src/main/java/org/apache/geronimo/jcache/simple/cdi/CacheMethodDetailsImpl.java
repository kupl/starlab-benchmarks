/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
package org.apache.geronimo.jcache.simple.cdi;

import java.lang.annotation.Annotation;
import java.lang.reflect.Method;
import java.util.Set;

import javax.cache.annotation.CacheMethodDetails;
import javax.interceptor.InvocationContext;

public class CacheMethodDetailsImpl<A extends Annotation> implements CacheMethodDetails<A> {

    protected final InvocationContext delegate;

    protected final CDIJCacheHelper.MethodMeta meta;

    private final Set<Annotation> annotations;

    private final A cacheAnnotation;

    private final String cacheName;

    public CacheMethodDetailsImpl(final InvocationContext delegate, final A cacheAnnotation, final String cacheName,
            final CDIJCacheHelper.MethodMeta meta) {
        this.delegate = delegate;
        this.annotations = meta.getAnnotations();
        this.cacheAnnotation = cacheAnnotation;
        this.cacheName = cacheName;
        this.meta = meta;
    }

    @Override
    public Method getMethod() {
        return delegate.getMethod();
    }

    @Override
    public Set<Annotation> getAnnotations() {
        return annotations;
    }

    @Override
    public A getCacheAnnotation() {
        return cacheAnnotation;
    }

    @Override
    public String getCacheName() {
        return cacheName;
    }
}
