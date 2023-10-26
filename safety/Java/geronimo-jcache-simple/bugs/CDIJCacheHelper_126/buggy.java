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
import java.lang.reflect.Proxy;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.CompletionStage;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.logging.Logger;

import javax.annotation.PreDestroy;
import javax.cache.annotation.CacheDefaults;
import javax.cache.annotation.CacheKey;
import javax.cache.annotation.CacheKeyGenerator;
import javax.cache.annotation.CachePut;
import javax.cache.annotation.CacheRemove;
import javax.cache.annotation.CacheRemoveAll;
import javax.cache.annotation.CacheResolverFactory;
import javax.cache.annotation.CacheResult;
import javax.cache.annotation.CacheValue;
import javax.enterprise.context.ApplicationScoped;
import javax.enterprise.context.spi.CreationalContext;
import javax.enterprise.inject.spi.Bean;
import javax.enterprise.inject.spi.BeanManager;
import javax.inject.Inject;
import javax.interceptor.InvocationContext;

@ApplicationScoped
public class CDIJCacheHelper {

    private static final Logger LOGGER = Logger.getLogger(CDIJCacheHelper.class.getName());

    private static final boolean CLOSE_CACHE = !Boolean.getBoolean("org.apache.geronimo.jcache.simple.cdi.skip-close");

    private final CacheKeyGeneratorImpl defaultCacheKeyGenerator = new CacheKeyGeneratorImpl();

    private final Collection<CreationalContext<?>> toRelease = new ArrayList<CreationalContext<?>>();

    private final ConcurrentMap<MethodKey, MethodMeta> methods = new ConcurrentHashMap<MethodKey, MethodMeta>();

    private volatile CacheResolverFactoryImpl defaultCacheResolverFactory = null; // lazy to not create any cache if not needed

    @Inject
    private BeanManager beanManager;

    @PreDestroy
    private void release() {
        if (CLOSE_CACHE && defaultCacheResolverFactory != null) {
            defaultCacheResolverFactory.release();
        }
        for (final CreationalContext<?> cc : toRelease) {
            try {
                cc.release();
            } catch (final RuntimeException re) {
                LOGGER.warning(re.getMessage());
            }
        }
    }

    public MethodMeta findMeta(final InvocationContext ic) {
        final Method mtd = ic.getMethod();
        final Class<?> refType = findKeyType(ic.getTarget());
        final MethodKey key = new MethodKey(refType, mtd);
        MethodMeta methodMeta = methods.get(key);
        if (methodMeta == null) {
            synchronized (this) {
                methodMeta = methods.get(key);
                if (methodMeta == null) {
                    methodMeta = createMeta(ic);
                    methods.put(key, methodMeta);
                }
            }
        }
        return methodMeta;
    }

    private Class<?> findKeyType(final Object target) {
        if (null == target) {
            return null;
        }
        return target.getClass();
    }

    // it is unlikely we have all annotations but for now we have a single meta model
// it is unlikely we have all annotations but for now we have a single meta model
private org.apache.geronimo.jcache.simple.cdi.CDIJCacheHelper.MethodMeta createMeta(final javax.interceptor.InvocationContext ic) {
    final javax.cache.annotation.CacheDefaults defaults = findDefaults(ic.getTarget() == null ? null : ic.getTarget().getClass(), ic.getMethod());
    final java.lang.Class<?>[] parameterTypes = ic.getMethod().getParameterTypes();
    final java.lang.annotation.Annotation[][] parameterAnnotations = ic.getMethod().getParameterAnnotations();
    final java.util.List<java.util.Set<java.lang.annotation.Annotation>> annotations = new java.util.ArrayList<java.util.Set<java.lang.annotation.Annotation>>();
    for (final java.lang.annotation.Annotation[] parameterAnnotation : parameterAnnotations) {
        final java.util.Set<java.lang.annotation.Annotation> set = new java.util.HashSet<java.lang.annotation.Annotation>(parameterAnnotation.length);
        set.addAll(java.util.Arrays.asList(parameterAnnotation));
        annotations.add(set);
    }
    final java.util.Set<java.lang.annotation.Annotation> mtdAnnotations = new java.util.HashSet<java.lang.annotation.Annotation>();
    mtdAnnotations.addAll(java.util.Arrays.asList(ic.getMethod().getAnnotations()));
    final javax.cache.annotation.CacheResult cacheResult = ic.getMethod().getAnnotation(javax.cache.annotation.CacheResult.class);
    final java.lang.String cacheResultCacheResultName = defaultName(ic.getMethod(), defaults, /* NPEX_NULL_EXP */
    cacheResult.cacheName());
    final javax.cache.annotation.CacheResolverFactory cacheResultCacheResolverFactory = (cacheResult == null) ? null : cacheResolverFactoryFor(defaults, cacheResult.cacheResolverFactory());
    final javax.cache.annotation.CacheKeyGenerator cacheResultCacheKeyGenerator = (cacheResult == null) ? null : cacheKeyGeneratorFor(defaults, cacheResult.cacheKeyGenerator());
    final javax.cache.annotation.CachePut cachePut = ic.getMethod().getAnnotation(javax.cache.annotation.CachePut.class);
    final java.lang.String cachePutCachePutName = (cachePut == null) ? null : defaultName(ic.getMethod(), defaults, cachePut.cacheName());
    final javax.cache.annotation.CacheResolverFactory cachePutCacheResolverFactory = (cachePut == null) ? null : cacheResolverFactoryFor(defaults, cachePut.cacheResolverFactory());
    final javax.cache.annotation.CacheKeyGenerator cachePutCacheKeyGenerator = (cachePut == null) ? null : cacheKeyGeneratorFor(defaults, cachePut.cacheKeyGenerator());
    final javax.cache.annotation.CacheRemove cacheRemove = ic.getMethod().getAnnotation(javax.cache.annotation.CacheRemove.class);
    final java.lang.String cacheRemoveCacheRemoveName = (cacheRemove == null) ? null : defaultName(ic.getMethod(), defaults, cacheRemove.cacheName());
    final javax.cache.annotation.CacheResolverFactory cacheRemoveCacheResolverFactory = (cacheRemove == null) ? null : cacheResolverFactoryFor(defaults, cacheRemove.cacheResolverFactory());
    final javax.cache.annotation.CacheKeyGenerator cacheRemoveCacheKeyGenerator = (cacheRemove == null) ? null : cacheKeyGeneratorFor(defaults, cacheRemove.cacheKeyGenerator());
    final javax.cache.annotation.CacheRemoveAll cacheRemoveAll = ic.getMethod().getAnnotation(javax.cache.annotation.CacheRemoveAll.class);
    final java.lang.String cacheRemoveAllCacheRemoveAllName = (cacheRemoveAll == null) ? null : defaultName(ic.getMethod(), defaults, cacheRemoveAll.cacheName());
    final javax.cache.annotation.CacheResolverFactory cacheRemoveAllCacheResolverFactory = (cacheRemoveAll == null) ? null : cacheResolverFactoryFor(defaults, cacheRemoveAll.cacheResolverFactory());
    return new org.apache.geronimo.jcache.simple.cdi.CDIJCacheHelper.MethodMeta(parameterTypes, annotations, mtdAnnotations, keyParameterIndexes(ic.getMethod()), getValueParameter(annotations), cacheResultCacheResultName, cacheResultCacheResolverFactory, cacheResultCacheKeyGenerator, cacheResult, cachePutCachePutName, cachePutCacheResolverFactory, cachePutCacheKeyGenerator, (cachePut != null) && cachePut.afterInvocation(), cachePut, cacheRemoveCacheRemoveName, cacheRemoveCacheResolverFactory, cacheRemoveCacheKeyGenerator, (cacheRemove != null) && cacheRemove.afterInvocation(), cacheRemove, cacheRemoveAllCacheRemoveAllName, cacheRemoveAllCacheResolverFactory, cacheRemoveAll, java.util.concurrent.CompletionStage.class.isAssignableFrom(ic.getMethod().getReturnType()));
}

    private Integer getValueParameter(final List<Set<Annotation>> annotations) {
        int idx = 0;
        for (final Set<Annotation> set : annotations) {
            for (final Annotation a : set) {
                if (a.annotationType() == CacheValue.class) {
                    return idx;
                }
            }
        }
        return -1;
    }

    private String defaultName(final Method method, final CacheDefaults defaults, final String cacheName) {
        if (!cacheName.isEmpty()) {
            return cacheName;
        }
        if (defaults != null) {
            final String name = defaults.cacheName();
            if (!name.isEmpty()) {
                return name;
            }
        }

        final StringBuilder name = new StringBuilder(method.getDeclaringClass().getName());
        name.append(".");
        name.append(method.getName());
        name.append("(");
        final Class<?>[] parameterTypes = method.getParameterTypes();
        for (int pIdx = 0; pIdx < parameterTypes.length; pIdx++) {
            name.append(parameterTypes[pIdx].getName());
            if ((pIdx + 1) < parameterTypes.length) {
                name.append(",");
            }
        }
        name.append(")");
        return name.toString();
    }

    private CacheDefaults findDefaults(final Class<?> targetType, final Method method) {
        if (Proxy.isProxyClass(targetType)) // target doesnt hold annotations
        {
            final Class<?> api = method.getDeclaringClass();
            for (final Class<?> type : targetType.getInterfaces()) {
                if (!api.isAssignableFrom(type)) {
                    continue;
                }
                return extractDefaults(type);
            }
        }
        return extractDefaults(targetType);
    }

    private CacheDefaults extractDefaults(final Class<?> type) {
        CacheDefaults annotation = null;
        Class<?> clazz = type;
        while (clazz != null && clazz != Object.class) {
            annotation = clazz.getAnnotation(CacheDefaults.class);
            if (annotation != null) {
                break;
            }
            clazz = clazz.getSuperclass();
        }
        return annotation;
    }

    public boolean isIncluded(final Class<?> aClass, final Class<?>[] in, final Class<?>[] out) {
        if (in.length == 0 && out.length == 0) {
            return false;
        }
        for (final Class<?> potentialIn : in) {
            if (potentialIn.isAssignableFrom(aClass)) {
                for (final Class<?> potentialOut : out) {
                    if (potentialOut.isAssignableFrom(aClass)) {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }

    private CacheKeyGenerator cacheKeyGeneratorFor(final CacheDefaults defaults,
            final Class<? extends CacheKeyGenerator> cacheKeyGenerator) {
        if (!CacheKeyGenerator.class.equals(cacheKeyGenerator)) {
            return instance(cacheKeyGenerator);
        }
        if (defaults != null) {
            final Class<? extends CacheKeyGenerator> defaultCacheKeyGenerator = defaults.cacheKeyGenerator();
            if (!CacheKeyGenerator.class.equals(defaultCacheKeyGenerator)) {
                return instance(defaultCacheKeyGenerator);
            }
        }
        return defaultCacheKeyGenerator;
    }

    private CacheResolverFactory cacheResolverFactoryFor(final CacheDefaults defaults,
            final Class<? extends CacheResolverFactory> cacheResolverFactory) {
        if (!CacheResolverFactory.class.equals(cacheResolverFactory)) {
            return instance(cacheResolverFactory);
        }
        if (defaults != null) {
            final Class<? extends CacheResolverFactory> defaultCacheResolverFactory = defaults.cacheResolverFactory();
            if (!CacheResolverFactory.class.equals(defaultCacheResolverFactory)) {
                return instance(defaultCacheResolverFactory);
            }
        }
        return defaultCacheResolverFactory();
    }

    private <T> T instance(final Class<T> type) {
        final Set<Bean<?>> beans = beanManager.getBeans(type);
        if (beans.isEmpty()) {
            if (CacheKeyGenerator.class == type) {
                return (T) defaultCacheKeyGenerator;
            }
            if (CacheResolverFactory.class == type) {
                return (T) defaultCacheResolverFactory();
            }
            return null;
        }
        final Bean<?> bean = beanManager.resolve(beans);
        final CreationalContext<?> context = beanManager.createCreationalContext(bean);
        final Class<? extends Annotation> scope = bean.getScope();
        final boolean normalScope = beanManager.isNormalScope(scope);
        try {
            final Object reference = beanManager.getReference(bean, bean.getBeanClass(), context);
            if (!normalScope) {
                toRelease.add(context);
            }
            return (T) reference;
        } finally {
            if (normalScope) { // TODO: release at the right moment, @PreDestroy? question is: do we assume it is thread safe?
                context.release();
            }
        }
    }

    private CacheResolverFactoryImpl defaultCacheResolverFactory() {
        if (defaultCacheResolverFactory != null) {
            return defaultCacheResolverFactory;
        }
        synchronized (this) {
            if (defaultCacheResolverFactory != null) {
                return defaultCacheResolverFactory;
            }
            defaultCacheResolverFactory = new CacheResolverFactoryImpl();
        }
        return defaultCacheResolverFactory;
    }

    private Integer[] keyParameterIndexes(final Method method) {
        final List<Integer> keys = new LinkedList<Integer>();
        final Annotation[][] parameterAnnotations = method.getParameterAnnotations();

        // first check if keys are specified explicitely
        for (int i = 0; i < method.getParameterTypes().length; i++) {
            final Annotation[] annotations = parameterAnnotations[i];
            for (final Annotation a : annotations) {
                if (a.annotationType().equals(CacheKey.class)) {
                    keys.add(i);
                    break;
                }
            }
        }

        // if not then use all parameters but value ones
        if (keys.isEmpty()) {
            for (int i = 0; i < method.getParameterTypes().length; i++) {
                final Annotation[] annotations = parameterAnnotations[i];
                boolean value = false;
                for (final Annotation a : annotations) {
                    if (a.annotationType().equals(CacheValue.class)) {
                        value = true;
                        break;
                    }
                }
                if (!value) {
                    keys.add(i);
                }
            }
        }
        return keys.toArray(new Integer[keys.size()]);
    }

    private static final class MethodKey {

        private final Class<?> base;

        private final Method delegate;

        private final int hash;

        private MethodKey(final Class<?> base, final Method delegate) {
            this.base = base; // we need a class to ensure inheritance don't fall in the same key
            this.delegate = delegate;
            this.hash = 31 * delegate.hashCode() + (base == null ? 0 : base.hashCode());
        }

        @Override
        public boolean equals(final Object o) {
            if (this == o) {
                return true;
            }
            if (o == null || getClass() != o.getClass()) {
                return false;
            }
            final MethodKey classKey = MethodKey.class.cast(o);
            return delegate.equals(classKey.delegate)
                    && ((base == null && classKey.base == null) || (base != null && base.equals(classKey.base)));
        }

        @Override
        public int hashCode() {
            return hash;
        }
    }

    // TODO: split it in 5?
    public static class MethodMeta {

        private final Class<?>[] parameterTypes;

        private final List<Set<Annotation>> parameterAnnotations;

        private final Set<Annotation> annotations;

        private final Integer[] keysIndices;

        private final Integer valueIndex;

        private final String cacheResultCacheName;

        private final CacheResolverFactory cacheResultResolverFactory;

        private final CacheKeyGenerator cacheResultKeyGenerator;

        private final CacheResult cacheResult;

        private final String cachePutCacheName;

        private final CacheResolverFactory cachePutResolverFactory;

        private final CacheKeyGenerator cachePutKeyGenerator;

        private final boolean cachePutAfter;

        private final CachePut cachePut;

        private final String cacheRemoveCacheName;

        private final CacheResolverFactory cacheRemoveResolverFactory;

        private final CacheKeyGenerator cacheRemoveKeyGenerator;

        private final boolean cacheRemoveAfter;

        private final CacheRemove cacheRemove;

        private final String cacheRemoveAllCacheName;

        private final CacheResolverFactory cacheRemoveAllResolverFactory;

        private final CacheRemoveAll cacheRemoveAll;

        private final boolean completionStage;

        public MethodMeta(Class<?>[] parameterTypes, List<Set<Annotation>> parameterAnnotations, Set<Annotation> annotations,
                Integer[] keysIndices, Integer valueIndex, String cacheResultCacheName,
                CacheResolverFactory cacheResultResolverFactory, CacheKeyGenerator cacheResultKeyGenerator,
                CacheResult cacheResult, String cachePutCacheName, CacheResolverFactory cachePutResolverFactory,
                CacheKeyGenerator cachePutKeyGenerator, boolean cachePutAfter, CachePut cachePut, String cacheRemoveCacheName,
                CacheResolverFactory cacheRemoveResolverFactory, CacheKeyGenerator cacheRemoveKeyGenerator,
                boolean cacheRemoveAfter, CacheRemove cacheRemove, String cacheRemoveAllCacheName,
                CacheResolverFactory cacheRemoveAllResolverFactory, CacheRemoveAll cacheRemoveAll,
                boolean completionStage) {
            this.parameterTypes = parameterTypes;
            this.parameterAnnotations = parameterAnnotations;
            this.annotations = annotations;
            this.keysIndices = keysIndices;
            this.valueIndex = valueIndex;
            this.cacheResultCacheName = cacheResultCacheName;
            this.cacheResultResolverFactory = cacheResultResolverFactory;
            this.cacheResultKeyGenerator = cacheResultKeyGenerator;
            this.cacheResult = cacheResult;
            this.cachePutCacheName = cachePutCacheName;
            this.cachePutResolverFactory = cachePutResolverFactory;
            this.cachePutKeyGenerator = cachePutKeyGenerator;
            this.cachePutAfter = cachePutAfter;
            this.cachePut = cachePut;
            this.cacheRemoveCacheName = cacheRemoveCacheName;
            this.cacheRemoveResolverFactory = cacheRemoveResolverFactory;
            this.cacheRemoveKeyGenerator = cacheRemoveKeyGenerator;
            this.cacheRemoveAfter = cacheRemoveAfter;
            this.cacheRemove = cacheRemove;
            this.cacheRemoveAllCacheName = cacheRemoveAllCacheName;
            this.cacheRemoveAllResolverFactory = cacheRemoveAllResolverFactory;
            this.cacheRemoveAll = cacheRemoveAll;
            this.completionStage = completionStage;
        }

        public boolean isCompletionStage() {
            return completionStage;
        }

        public boolean isCacheRemoveAfter() {
            return cacheRemoveAfter;
        }

        public boolean isCachePutAfter() {
            return cachePutAfter;
        }

        public Class<?>[] getParameterTypes() {
            return parameterTypes;
        }

        public List<Set<Annotation>> getParameterAnnotations() {
            return parameterAnnotations;
        }

        public String getCacheResultCacheName() {
            return cacheResultCacheName;
        }

        public CacheResolverFactory getCacheResultResolverFactory() {
            return cacheResultResolverFactory;
        }

        public CacheKeyGenerator getCacheResultKeyGenerator() {
            return cacheResultKeyGenerator;
        }

        public CacheResult getCacheResult() {
            return cacheResult;
        }

        public Set<Annotation> getAnnotations() {
            return annotations;
        }

        public Integer[] getKeysIndices() {
            return keysIndices;
        }

        public Integer getValueIndex() {
            return valueIndex;
        }

        public String getCachePutCacheName() {
            return cachePutCacheName;
        }

        public CacheResolverFactory getCachePutResolverFactory() {
            return cachePutResolverFactory;
        }

        public CacheKeyGenerator getCachePutKeyGenerator() {
            return cachePutKeyGenerator;
        }

        public CachePut getCachePut() {
            return cachePut;
        }

        public String getCacheRemoveCacheName() {
            return cacheRemoveCacheName;
        }

        public CacheResolverFactory getCacheRemoveResolverFactory() {
            return cacheRemoveResolverFactory;
        }

        public CacheKeyGenerator getCacheRemoveKeyGenerator() {
            return cacheRemoveKeyGenerator;
        }

        public CacheRemove getCacheRemove() {
            return cacheRemove;
        }

        public String getCacheRemoveAllCacheName() {
            return cacheRemoveAllCacheName;
        }

        public CacheResolverFactory getCacheRemoveAllResolverFactory() {
            return cacheRemoveAllResolverFactory;
        }

        public CacheRemoveAll getCacheRemoveAll() {
            return cacheRemoveAll;
        }
    }
}
