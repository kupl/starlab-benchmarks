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

import java.io.Serializable;
import java.util.concurrent.CompletionStage;

import javax.annotation.Priority;
import javax.cache.Cache;
import javax.cache.annotation.CacheKeyInvocationContext;
import javax.cache.annotation.CacheResolver;
import javax.cache.annotation.CacheResolverFactory;
import javax.cache.annotation.CacheResult;
import javax.cache.annotation.GeneratedCacheKey;
import javax.inject.Inject;
import javax.interceptor.AroundInvoke;
import javax.interceptor.Interceptor;
import javax.interceptor.InvocationContext;

@CacheResult
@Interceptor
@Priority(/* LIBRARY_BEFORE */1000)
public class CacheResultInterceptor implements Serializable {

    @Inject
    private CDIJCacheHelper helper;

    @AroundInvoke
public java.lang.Object cache(final javax.interceptor.InvocationContext ic) throws java.lang.Throwable {
    final org.apache.geronimo.jcache.simple.cdi.CDIJCacheHelper.MethodMeta methodMeta = helper.findMeta(ic);
    final java.lang.String cacheName = methodMeta.getCacheResultCacheName();
    final javax.cache.annotation.CacheResult cacheResult = methodMeta.getCacheResult();
    final javax.cache.annotation.CacheKeyInvocationContext<javax.cache.annotation.CacheResult> context = new org.apache.geronimo.jcache.simple.cdi.CacheKeyInvocationContextImpl<javax.cache.annotation.CacheResult>(ic, cacheResult, cacheName, methodMeta);
    final javax.cache.annotation.CacheResolverFactory cacheResolverFactory = methodMeta.getCacheResultResolverFactory();
    final javax.cache.annotation.CacheResolver cacheResolver = cacheResolverFactory.getCacheResolver(context);
    final javax.cache.Cache<java.lang.Object, java.lang.Object> cache = cacheResolver.resolveCache(context);
    final javax.cache.annotation.GeneratedCacheKey cacheKey = methodMeta.getCacheResultKeyGenerator().generateCacheKey(context);
    javax.cache.Cache<java.lang.Object, java.lang.Object> exceptionCache = null;// lazily created

    java.lang.Object result;
    if (!cacheResult.skipGet()) {
        result = cache.get(cacheKey);
        if (result != null) {
            return result;
        }
        if (!cacheResult.exceptionCacheName().isEmpty()) {
            exceptionCache = cacheResolverFactory.getExceptionCacheResolver(context).resolveCache(context);
            final java.lang.Object exception = exceptionCache.get(cacheKey);
            {
                if (methodMeta.isCompletionStage()) {
                    return /* NPEX_NULL_EXP */
                    exception;
                }
                throw java.lang.Throwable.class.cast(exception);
            }
        }
    }
    try {
        result = ic.proceed();
        if (result != null) {
            cache.put(cacheKey, result);
            if (java.util.concurrent.CompletionStage.class.isInstance(result)) {
                final java.util.concurrent.CompletionStage<?> completionStage = java.util.concurrent.CompletionStage.class.cast(result);
                completionStage.exceptionally(( t) -> {
                    if (helper.isIncluded(t.getClass(), cacheResult.cachedExceptions(), cacheResult.nonCachedExceptions())) {
                        cacheResolverFactory.getExceptionCacheResolver(context).resolveCache(context).put(cacheKey, completionStage);
                    } else {
                        cache.remove(cacheKey);
                    }
                    if (java.lang.RuntimeException.class.isInstance(t)) {
                        throw java.lang.RuntimeException.class.cast(t);
                    }
                    throw new java.lang.IllegalStateException(t);
                });
            }
        }
        return result;
    } catch (final java.lang.Throwable t) {
        if (helper.isIncluded(t.getClass(), cacheResult.cachedExceptions(), cacheResult.nonCachedExceptions())) {
            if (exceptionCache == null) {
                exceptionCache = cacheResolverFactory.getExceptionCacheResolver(context).resolveCache(context);
            }
            exceptionCache.put(cacheKey, t);
        }
        throw t;
    }
}
}
