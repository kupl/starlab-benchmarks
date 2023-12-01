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
import java.util.List;
import java.util.Set;

import javax.cache.annotation.CacheInvocationContext;
import javax.cache.annotation.CacheInvocationParameter;
import javax.interceptor.InvocationContext;

public class CacheInvocationContextImpl<A extends Annotation> extends CacheMethodDetailsImpl<A>
        implements CacheInvocationContext<A> {

    private static final Object[] EMPTY_ARGS = new Object[0];

    private CacheInvocationParameter[] parameters = null;

    public CacheInvocationContextImpl(final InvocationContext delegate, final A cacheAnnotation, final String cacheName,
            final CDIJCacheHelper.MethodMeta meta) {
        super(delegate, cacheAnnotation, cacheName, meta);
    }

    @Override
    public Object getTarget() {
        return delegate.getTarget();
    }

    @Override
    public CacheInvocationParameter[] getAllParameters() {
        if (parameters == null) {
            parameters = doGetAllParameters(null);
        }
        return parameters;
    }

    @Override
    public <T> T unwrap(final Class<T> cls) {
        if (cls.isAssignableFrom(getClass())) {
            return cls.cast(this);
        }
        throw new IllegalArgumentException(cls.getName());
    }

    protected CacheInvocationParameter[] doGetAllParameters(final Integer[] indexes) {
        final Object[] parameters = delegate.getParameters();
        final Object[] args = parameters == null ? EMPTY_ARGS : parameters;
        final Class<?>[] parameterTypes = meta.getParameterTypes();
        final List<Set<Annotation>> parameterAnnotations = meta.getParameterAnnotations();

        final CacheInvocationParameter[] parametersAsArray = new CacheInvocationParameter[indexes == null ? args.length
                : indexes.length];
        if (indexes == null) {
            for (int i = 0; i < args.length; i++) {
                parametersAsArray[i] = newCacheInvocationParameterImpl(parameterTypes[i], args[i], parameterAnnotations.get(i),
                        i);
            }
        } else {
            int outIdx = 0;
            for (int idx = 0; idx < indexes.length; idx++) {
                final int i = indexes[idx];
                parametersAsArray[outIdx] = newCacheInvocationParameterImpl(parameterTypes[i], args[i],
                        parameterAnnotations.get(i), i);
                outIdx++;
            }
        }
        return parametersAsArray;
    }

    private CacheInvocationParameterImpl newCacheInvocationParameterImpl(final Class<?> type, final Object arg,
            final Set<Annotation> annotations, final int i) {
        return new CacheInvocationParameterImpl(type, arg, annotations, i);
    }
}
