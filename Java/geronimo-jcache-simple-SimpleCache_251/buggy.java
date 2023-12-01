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
package org.apache.geronimo.jcache.simple;

import static org.apache.geronimo.jcache.simple.Asserts.assertNotNull;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.CopyOnWriteArraySet;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.atomic.AtomicReference;

import javax.cache.Cache;
import javax.cache.CacheException;
import javax.cache.CacheManager;
import javax.cache.configuration.CacheEntryListenerConfiguration;
import javax.cache.configuration.Configuration;
import javax.cache.configuration.Factory;
import javax.cache.event.CacheEntryEvent;
import javax.cache.event.EventType;
import javax.cache.expiry.Duration;
import javax.cache.expiry.EternalExpiryPolicy;
import javax.cache.expiry.ExpiryPolicy;
import javax.cache.integration.CacheLoader;
import javax.cache.integration.CacheLoaderException;
import javax.cache.integration.CacheWriter;
import javax.cache.integration.CacheWriterException;
import javax.cache.integration.CompletionListener;
import javax.cache.processor.EntryProcessor;
import javax.cache.processor.EntryProcessorException;
import javax.cache.processor.EntryProcessorResult;
import javax.management.ObjectName;

public class SimpleCache<K, V> implements Cache<K, V> {

    private final SimpleManager manager;

    private final SimpleConfiguration<K, V> config;

    private final CacheLoader<K, V> loader;

    private final CacheWriter<? super K, ? super V> writer;

    private final ExpiryPolicy expiryPolicy;

    private final ObjectName cacheConfigObjectName;

    private final ObjectName cacheStatsObjectName;

    private final String name;

    private final ConcurrentHashMap<SimpleKey<K>, SimpleElement<V>> delegate;

    private final Map<CacheEntryListenerConfiguration<K, V>, SimpleListener<K, V>> listeners = new ConcurrentHashMap<>();

    private final Statistics statistics = new Statistics();

    private final ExecutorService pool;

    private final Serializations serializations;

    private final Collection<Future<?>> poolTasks = new CopyOnWriteArraySet<>();

    private volatile boolean closed = false;

    public SimpleCache(final ClassLoader classLoader, final SimpleManager mgr, final String cacheName,
            final SimpleConfiguration<K, V> configuration, final Properties properties,
            final ExecutorService executorService) {
        manager = mgr;

        name = cacheName;

        final int capacity = Integer.parseInt(property(properties, cacheName, "capacity", "1000"));
        final float loadFactor = Float.parseFloat(property(properties, cacheName, "loadFactor", "0.75"));
        final int concurrencyLevel = Integer.parseInt(property(properties, cacheName, "concurrencyLevel", "16"));
        delegate = new ConcurrentHashMap<>(capacity, loadFactor, concurrencyLevel);
        config = configuration;
        pool = executorService;

        final long evictionPause = Long.parseLong(
                properties.getProperty(cacheName + ".evictionPause", properties.getProperty("evictionPause", "30000")));
        if (evictionPause > 0) {
            final long maxDeleteByEvictionRun = Long.parseLong(property(properties, cacheName, "maxDeleteByEvictionRun", "100"));
            addPoolTask(new EvictionThread(evictionPause, maxDeleteByEvictionRun));
        }

        serializations = new Serializations(property(properties, cacheName, "serialization.whitelist", null));

        final Factory<CacheLoader<K, V>> cacheLoaderFactory = configuration.getCacheLoaderFactory();
        if (cacheLoaderFactory == null) {
            loader = NoLoader.INSTANCE;
        } else {
            loader = ExceptionWrapperHandler.newProxy(classLoader, cacheLoaderFactory.create(), CacheLoaderException.class,
                    CacheLoader.class);
        }

        final Factory<CacheWriter<? super K, ? super V>> cacheWriterFactory = configuration.getCacheWriterFactory();
        if (cacheWriterFactory == null) {
            writer = NoWriter.INSTANCE;
        } else {
            writer = ExceptionWrapperHandler.newProxy(classLoader, cacheWriterFactory.create(), CacheWriterException.class,
                    CacheWriter.class);
        }

        final Factory<ExpiryPolicy> expiryPolicyFactory = configuration.getExpiryPolicyFactory();
        if (expiryPolicyFactory == null) {
            expiryPolicy = new EternalExpiryPolicy();
        } else {
            expiryPolicy = expiryPolicyFactory.create();
        }

        for (final CacheEntryListenerConfiguration<K, V> listener : config.getCacheEntryListenerConfigurations()) {
            listeners.put(listener, new SimpleListener<>(listener));
        }

        statistics.setActive(config.isStatisticsEnabled());

        final String mgrStr = manager.getURI().toString().replaceAll(",|:|=|\n", ".");
        final String cacheStr = name.replaceAll(",|:|=|\n", ".");
        try {
            cacheConfigObjectName = new ObjectName(
                    "javax.cache:type=CacheConfiguration," + "CacheManager=" + mgrStr + "," + "Cache=" + cacheStr);
            cacheStatsObjectName = new ObjectName(
                    "javax.cache:type=CacheStatistics," + "CacheManager=" + mgrStr + "," + "Cache=" + cacheStr);
        } catch (final Exception e) {
            throw new IllegalArgumentException(e);
        }
        if (config.isManagementEnabled()) {
            JMXs.register(cacheConfigObjectName, new SimpleCacheMXBean<K, V>(this));
        }
        if (config.isStatisticsEnabled()) {
            JMXs.register(cacheStatsObjectName, new SimpleCacheStatisticsMXBean(statistics));
        }
    }

    private void assertNotClosed() {
        if (isClosed()) {
            throw new IllegalStateException("cache closed");
        }
    }

    @Override
    public V get(final K key) {
        assertNotClosed();
        assertNotNull(key, "key");
        final long getStart = Times.now(false);
        return doGetControllingExpiry(getStart, key, true, false, false, true, loader);
    }

    private V doLoad(final K key, final boolean update, final boolean propagateLoadException, final CacheLoader<K, V> loader) {
        V v = null;
        try {
            v = loader.load(key);
        } catch (final CacheLoaderException e) {
            if (propagateLoadException) {
                throw e;
            }
        }
        if (v != null) {
            final Duration duration = update ? expiryPolicy.getExpiryForUpdate() : expiryPolicy.getExpiryForCreation();
            if (isNotZero(duration)) {
                delegate.put(new SimpleKey<>(key), new SimpleElement<>(v, duration));
            }
        }
        return v;
    }

    private void touch(final SimpleKey<K> key, final SimpleElement<V> element) {
        if (config.isStoreByValue()) {
            delegate.put(new SimpleKey<>(serializations.copy(manager.getClassLoader(), key.getKey())), element);
        }
    }

    @Override
    public Map<K, V> getAll(final Set<? extends K> keys) {
        assertNotClosed();
        for (final K k : keys) {
            assertNotNull(k, "key");
        }

        final Map<K, V> result = new HashMap<>();
        for (final K key : keys) {
            assertNotNull(key, "key");

            final SimpleKey<K> simpleKey = new SimpleKey<>(key);
            final SimpleElement<V> elt = delegate.get(simpleKey);
            V val = elt != null ? elt.getElement() : null;
            if (val == null && config.isReadThrough()) {
                val = doLoad(key, false, false, loader);
                if (val != null) {
                    result.put(key, val);
                }
            } else if (elt != null) {
                final Duration expiryForAccess = expiryPolicy.getExpiryForAccess();
                if (isNotZero(expiryForAccess)) {
                    touch(simpleKey, elt);
                    result.put(key, val);
                } else {
                    expires(simpleKey);
                }
            }
        }
        return result;
    }

    @Override
    public boolean containsKey(final K key) {
        assertNotClosed();
        assertNotNull(key, "key");
        return delegate.get(new SimpleKey<>(key)) != null;
    }

    @Override
public void put(final K key, final V rawValue) {
    assertNotClosed();
    org.apache.geronimo.jcache.simple.Asserts.assertNotNull(key, "key");
    org.apache.geronimo.jcache.simple.Asserts.assertNotNull(rawValue, "value");
    final boolean storeByValue = config.isStoreByValue();
    final org.apache.geronimo.jcache.simple.SimpleKey<K> simpleKey = new org.apache.geronimo.jcache.simple.SimpleKey<>(storeByValue ? serializations.copy(manager.getClassLoader(), key) : key);
    final org.apache.geronimo.jcache.simple.SimpleElement<V> oldElt = delegate.get(simpleKey);
    final V old = /* NPEX_NULL_EXP */
    oldElt.getElement();
    final V value = (storeByValue) ? serializations.copy(manager.getClassLoader(), rawValue) : rawValue;
    final boolean created = old == null;
    final javax.cache.expiry.Duration duration = (created) ? expiryPolicy.getExpiryForCreation() : expiryPolicy.getExpiryForUpdate();
    if (org.apache.geronimo.jcache.simple.SimpleCache.isNotZero(duration)) {
        final boolean statisticsEnabled = config.isStatisticsEnabled();
        final long start = org.apache.geronimo.jcache.simple.Times.now(false);
        writer.write(new org.apache.geronimo.jcache.simple.SimpleEntry<>(key, value));
        delegate.put(simpleKey, new org.apache.geronimo.jcache.simple.SimpleElement<>(value, duration));
        if (!listeners.isEmpty()) {
            for (final org.apache.geronimo.jcache.simple.SimpleListener<K, V> listener : listeners.values()) {
                if (created) {
                    listener.onCreated(java.util.Collections.<javax.cache.event.CacheEntryEvent<? extends K, ? extends V>>singletonList(new org.apache.geronimo.jcache.simple.SimpleEvent<>(this, javax.cache.event.EventType.CREATED, null, key, value)));
                } else {
                    listener.onUpdated(java.util.Collections.<javax.cache.event.CacheEntryEvent<? extends K, ? extends V>>singletonList(new org.apache.geronimo.jcache.simple.SimpleEvent<>(this, javax.cache.event.EventType.UPDATED, old, key, value)));
                }
            }
        }
        if (statisticsEnabled) {
            statistics.increasePuts(1);
            statistics.addPutTime(org.apache.geronimo.jcache.simple.Times.now(false) - start);
        }
    } else if (!created) {
        expires(simpleKey);
    }
}

    private void expires(final SimpleKey<K> cacheKey) {
        final SimpleElement<V> elt = delegate.get(cacheKey);
        delegate.remove(cacheKey);
        onExpired(cacheKey, elt);
    }

    private void onExpired(final SimpleKey<K> cacheKey, final SimpleElement<V> elt) {
        for (final SimpleListener<K, V> listener : listeners.values()) {
            listener.onExpired(Collections.<CacheEntryEvent<? extends K, ? extends V>> singletonList(
                    new SimpleEvent<>(this, EventType.REMOVED, null, cacheKey.getKey(), elt.getElement())));
        }
    }

    @Override
    public V getAndPut(final K key, final V value) {
        assertNotClosed();
        assertNotNull(key, "key");
        assertNotNull(value, "value");
        final long getStart = Times.now(false);
        final V v = doGetControllingExpiry(getStart, key, false, false, true, false, loader);
        put(key, value);
        return v;
    }

    @Override
    public void putAll(final Map<? extends K, ? extends V> map) {
        assertNotClosed();
        final TempStateCacheView<K, V> view = new TempStateCacheView<K, V>(this);
        for (final Map.Entry<? extends K, ? extends V> e : map.entrySet()) {
            view.put(e.getKey(), e.getValue());
        }
        view.merge();
    }

    @Override
    public boolean putIfAbsent(final K key, final V value) {
        final boolean statisticsEnabled = config.isStatisticsEnabled();
        if (!containsKey(key)) {
            if (statisticsEnabled) {
                statistics.increaseMisses(1);
            }
            put(key, value);
            return true;
        } else {
            if (statisticsEnabled) {
                statistics.increaseHits(1);
            }
        }
        return false;
    }

    @Override
    public boolean remove(final K key) {
        assertNotClosed();
        assertNotNull(key, "key");

        final boolean statisticsEnabled = config.isStatisticsEnabled();
        final long start = Times.now(!statisticsEnabled);

        writer.delete(key);
        final SimpleKey<K> cacheKey = new SimpleKey<>(key);

        final SimpleElement<V> v = delegate.remove(cacheKey);
        if (v == null || v.isExpired()) {
            return false;
        }

        final V value = v.getElement();
        for (final SimpleListener<K, V> listener : listeners.values()) {
            listener.onRemoved(Collections.<CacheEntryEvent<? extends K, ? extends V>> singletonList(
                    new SimpleEvent<>(this, EventType.REMOVED, value, key, value)));
        }
        if (statisticsEnabled) {
            statistics.increaseRemovals(1);
            statistics.addRemoveTime(Times.now(false) - start);
        }

        return true;
    }

    @Override
    public boolean remove(final K key, final V oldValue) {
        assertNotClosed();
        assertNotNull(key, "key");
        assertNotNull(oldValue, "oldValue");
        final long getStart = Times.now(false);
        final V v = doGetControllingExpiry(getStart, key, false, false, false, false, loader);
        if (oldValue.equals(v)) {
            remove(key);
            return true;
        } else if (v != null) {
            // weird but just for stats to be right
            // (org.jsr107.tck.expiry.CacheExpiryTest.removeSpecifiedEntryShouldNotCallExpiryPolicyMethods())
            expiryPolicy.getExpiryForAccess();
        }
        return false;
    }

    @Override
    public V getAndRemove(final K key) {
        assertNotClosed();
        assertNotNull(key, "key");
        final long getStart = Times.now(false);
        final V v = doGetControllingExpiry(getStart, key, false, false, true, false, loader);
        remove(key);
        return v;
    }

    private V doGetControllingExpiry(final long getStart, final K key, final boolean updateAcess, final boolean forceDoLoad,
            final boolean skipLoad, final boolean propagateLoadException, final CacheLoader<K, V> loader) {
        final boolean statisticsEnabled = config.isStatisticsEnabled();
        final SimpleKey<K> simpleKey = new SimpleKey<>(key);
        final SimpleElement<V> elt = delegate.get(simpleKey);
        V v = elt != null ? elt.getElement() : null;
        if (v == null && (config.isReadThrough() || forceDoLoad)) {
            if (!skipLoad) {
                v = doLoad(key, false, propagateLoadException, loader);
            }
        } else if (statisticsEnabled) {
            if (v != null) {
                statistics.increaseHits(1);
            } else {
                statistics.increaseMisses(1);
            }
        }

        if (updateAcess && elt != null) {
            final Duration expiryForAccess = expiryPolicy.getExpiryForAccess();
            if (!isNotZero(expiryForAccess)) {
                expires(simpleKey);
            }
        }
        if (statisticsEnabled && v != null) {
            statistics.addGetTime(Times.now(false) - getStart);
        }
        return v;
    }

    @Override
    public boolean replace(final K key, final V oldValue, final V newValue) {
        assertNotClosed();
        assertNotNull(key, "key");
        assertNotNull(oldValue, "oldValue");
        assertNotNull(newValue, "newValue");
        final V value = doGetControllingExpiry(Times.now(config.isStatisticsEnabled()), key, false, config.isReadThrough(), false,
                true, loader);
        if (value != null && value.equals(oldValue)) {
            put(key, newValue);
            return true;
        } else if (value != null) {
            expiryPolicy.getExpiryForAccess();
        }
        return false;
    }

    @Override
    public boolean replace(final K key, final V value) {
        assertNotClosed();
        assertNotNull(key, "key");
        assertNotNull(value, "value");
        boolean statisticsEnabled = config.isStatisticsEnabled();
        if (containsKey(key)) {
            if (statisticsEnabled) {
                statistics.increaseHits(1);
            }
            put(key, value);
            return true;
        } else if (statisticsEnabled) {
            statistics.increaseMisses(1);
        }
        return false;
    }

    @Override
    public V getAndReplace(final K key, final V value) {
        assertNotClosed();
        assertNotNull(key, "key");
        assertNotNull(value, "value");

        final boolean statisticsEnabled = config.isStatisticsEnabled();

        final SimpleElement<V> elt = delegate.get(new SimpleKey<>(key));
        if (elt != null) {
            V oldValue = elt.getElement();
            if (oldValue == null && config.isReadThrough()) {
                oldValue = doLoad(key, false, false, loader);
            } else if (statisticsEnabled) {
                statistics.increaseHits(1);
            }
            put(key, value);
            return oldValue;
        } else if (statisticsEnabled) {
            statistics.increaseMisses(1);
        }
        return null;
    }

    @Override
    public void removeAll(final Set<? extends K> keys) {
        assertNotClosed();
        assertNotNull(keys, "keys");
        for (final K k : keys) {
            remove(k);
        }
    }

    @Override
    public void removeAll() {
        assertNotClosed();
        for (final SimpleKey<K> k : delegate.keySet()) {
            remove(k.getKey());
        }
    }

    @Override
    public void clear() {
        assertNotClosed();
        delegate.clear();
    }

    @Override
    public <C2 extends Configuration<K, V>> C2 getConfiguration(final Class<C2> clazz) {
        assertNotClosed();
        return clazz.cast(config);
    }

    @Override
    public void loadAll(final Set<? extends K> keys, final boolean replaceExistingValues,
            final CompletionListener completionListener) {
        assertNotClosed();
        assertNotNull(keys, "keys");
        if (loader == null) { // quick exit path
            if (completionListener != null) {
                completionListener.onCompletion();
            }
            return;
        }
        for (final K k : keys) {
            assertNotNull(k, "a key");
        }
        addPoolTask(new Runnable() {

            @Override
            public void run() {
                doLoadAll(keys, replaceExistingValues, completionListener);
            }
        });
    }

    private void addPoolTask(final Runnable runnable) {
        final AtomicReference<Future<?>> ref = new AtomicReference<>();
        final CountDownLatch refIsSet = new CountDownLatch(1);
        ref.set(pool.submit(new Runnable() {
            @Override
            public void run() {
                try {
                    runnable.run();
                } finally {
                    try {
                        refIsSet.await();
                    } catch (final InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                    poolTasks.remove(ref.get());
                }
            }
        }));
        refIsSet.countDown();
        poolTasks.add(ref.get());
    }

    private void doLoadAll(final Set<? extends K> keys, final boolean replaceExistingValues,
            final CompletionListener completionListener) {
        try {
            final long now = Times.now(false);
            final Map<K, V> kvMap = loader.loadAll(keys);
            if (kvMap == null) {
                return;
            }
            final CacheLoader<K, V> preloaded = new MapLoader<>(kvMap);
            for (final K k : keys) {
                if (replaceExistingValues) {
                    doLoad(k, containsKey(k), completionListener != null, preloaded);
                } else if (!containsKey(k)) {
                    doGetControllingExpiry(now, k, true, true, false, completionListener != null, preloaded);
                }
            }
        } catch (final RuntimeException e) {
            if (completionListener != null) {
                completionListener.onException(e);
                return;
            }
        }
        if (completionListener != null) {
            completionListener.onCompletion();
        }
    }

    @Override
    public <T> T invoke(final K key, final EntryProcessor<K, V, T> entryProcessor, final Object... arguments)
            throws EntryProcessorException {
        final TempStateCacheView<K, V> view = new TempStateCacheView<K, V>(this);
        final T t = doInvoke(view, key, entryProcessor, arguments);
        view.merge();
        return t;
    }

    private <T> T doInvoke(final TempStateCacheView<K, V> view, final K key, final EntryProcessor<K, V, T> entryProcessor,
            final Object... arguments) {
        assertNotClosed();
        assertNotNull(entryProcessor, "entryProcessor");
        assertNotNull(key, "key");
        try {
            if (config.isStatisticsEnabled()) {
                if (containsKey(key)) {
                    statistics.increaseHits(1);
                } else {
                    statistics.increaseMisses(1);
                }
            }
            return entryProcessor.process(new SimpleMutableEntry<>(view, key), arguments);
        } catch (final Exception ex) {
            return throwEntryProcessorException(ex);
        }
    }

    @Override
    public <T> Map<K, EntryProcessorResult<T>> invokeAll(final Set<? extends K> keys,
            final EntryProcessor<K, V, T> entryProcessor, final Object... arguments) {
        assertNotClosed();
        assertNotNull(entryProcessor, "entryProcessor");
        final Map<K, EntryProcessorResult<T>> results = new HashMap<>();
        for (final K k : keys) {
            try {
                final T invoke = invoke(k, entryProcessor, arguments);
                if (invoke != null) {
                    results.put(k, new EntryProcessorResult<T>() {

                        @Override
                        public T get() throws EntryProcessorException {
                            return invoke;
                        }
                    });
                }
            } catch (final Exception e) {
                results.put(k, new EntryProcessorResult<T>() {

                    @Override
                    public T get() throws EntryProcessorException {
                        return throwEntryProcessorException(e);
                    }
                });
            }
        }
        return results;
    }

    @Override
    public void registerCacheEntryListener(final CacheEntryListenerConfiguration<K, V> cacheEntryListenerConfiguration) {
        assertNotClosed();
        if (listeners.containsKey(cacheEntryListenerConfiguration)) {
            throw new IllegalArgumentException(cacheEntryListenerConfiguration + " already registered");
        }
        listeners.put(cacheEntryListenerConfiguration, new SimpleListener<>(cacheEntryListenerConfiguration));
        config.addListener(cacheEntryListenerConfiguration);
    }

    @Override
    public void deregisterCacheEntryListener(final CacheEntryListenerConfiguration<K, V> cacheEntryListenerConfiguration) {
        assertNotClosed();
        listeners.remove(cacheEntryListenerConfiguration);
        config.removeListener(cacheEntryListenerConfiguration);
    }

    @Override
    public Iterator<Entry<K, V>> iterator() {
        assertNotClosed();
        final Iterator<SimpleKey<K>> keys = new HashSet<>(delegate.keySet()).iterator();
        return new Iterator<Entry<K, V>>() {

            private K lastKey = null;

            @Override
            public boolean hasNext() {
                return keys.hasNext();
            }

            @Override
            public Entry<K, V> next() {
                lastKey = keys.next().getKey();
                return new SimpleEntry<>(lastKey, get(lastKey));
            }

            @Override
            public void remove() {
                if (isClosed() || lastKey == null) {
                    throw new IllegalStateException(isClosed() ? "cache closed" : "call next() before remove()");
                }
                SimpleCache.this.remove(lastKey);
            }
        };
    }

    @Override
    public String getName() {
        assertNotClosed();
        return name;
    }

    @Override
    public CacheManager getCacheManager() {
        assertNotClosed();
        return manager;
    }

    @Override
    public synchronized void close() {
        if (isClosed()) {
            return;
        }

        for (final Future<?> task : poolTasks) {
            task.cancel(true);
        }

        final CacheException ce = new CacheException();
        manager.release(getName());
        closed = true;
        close(loader, ce);
        close(writer, ce);
        close(expiryPolicy, ce);
        for (final SimpleListener<K, V> listener : listeners.values()) {
            try {
                listener.close();
            } catch (final Exception e) {
                ce.addSuppressed(e);
            }
        }
        listeners.clear();
        JMXs.unregister(cacheConfigObjectName);
        JMXs.unregister(cacheStatsObjectName);
        delegate.clear();
        if (ce.getSuppressed().length > 0) {
            throw ce;
        }
    }

    @Override
    public boolean isClosed() {
        return closed;
    }

    @Override
    public <T> T unwrap(final Class<T> clazz) {
        assertNotClosed();
        if (clazz.isInstance(this)) {
            return clazz.cast(this);
        }
        if (clazz.isAssignableFrom(Map.class) || clazz.isAssignableFrom(ConcurrentMap.class)) {
            return clazz.cast(delegate);
        }
        throw new IllegalArgumentException(clazz.getName() + " not supported in unwrap");
    }

    public Statistics getStatistics() {
        return statistics;
    }

    public void enableManagement() {
        config.managementEnabled();
        JMXs.register(cacheConfigObjectName, new SimpleCacheMXBean<K, V>(this));
    }

    public void disableManagement() {
        config.managementDisabled();
        JMXs.unregister(cacheConfigObjectName);
    }

    public void enableStatistics() {
        config.statisticsEnabled();
        statistics.setActive(true);
        JMXs.register(cacheStatsObjectName, new SimpleCacheStatisticsMXBean(statistics));
    }

    public void disableStatistics() {
        config.statisticsDisabled();
        statistics.setActive(false);
        JMXs.unregister(cacheStatsObjectName);
    }

    private static String property(final Properties properties, final String cacheName, final String name,
            final String defaultValue) {
        return properties.getProperty(cacheName + "." + name, properties.getProperty(name, defaultValue));
    }

    private static boolean isNotZero(final Duration duration) {
        return duration == null || !duration.isZero();
    }

    private static <T> T throwEntryProcessorException(final Exception ex) {
        if (EntryProcessorException.class.isInstance(ex)) {
            throw EntryProcessorException.class.cast(ex);
        }
        throw new EntryProcessorException(ex);
    }

    private static void close(final Object potentiallyCloseable, final CacheException wrapper) {
        if (AutoCloseable.class.isInstance(potentiallyCloseable)) {
            try {
                AutoCloseable.class.cast(potentiallyCloseable).close();
            } catch (final Exception re) {
                wrapper.addSuppressed(re);
            }
        }
    }

    private class EvictionThread implements Runnable {

        private final long pause;

        private final long maxDelete;

        private EvictionThread(final long evictionPause, final long maxDelete) {
            this.pause = evictionPause;
            this.maxDelete = maxDelete;
        }

        @Override
        public void run() {
            while (!isClosed()) {
                try {
                    Thread.sleep(pause);
                } catch (final InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
                if (delegate.isEmpty()) {
                    continue;
                }

                try {
                    final List<SimpleKey<K>> keys = new ArrayList<>(delegate.keySet());
                    Collections.sort(keys, new Comparator<SimpleKey<K>>() {

                        @Override
                        public int compare(final SimpleKey<K> o1, final SimpleKey<K> o2) {
                            final long l = o1.lastAccess() - o2.lastAccess();
                            if (l == 0) {
                                return keys.indexOf(o1) - keys.indexOf(o2);
                            }
                            return (int) l;
                        }
                    });

                    int delete = 0;
                    for (final SimpleKey<K> key : keys) {
                        final SimpleElement<V> elt = delegate.get(key);
                        if (elt != null && elt.isExpired()) {
                            delegate.remove(key);
                            statistics.increaseEvictions(1);
                            onExpired(key, elt);
                            delete++;
                            if (delete >= maxDelete) {
                                break;
                            }
                        }
                    }
                } catch (final Exception e) {
                    // no-op
                }
            }
        }
    }

    private static class MapLoader<K, V> implements CacheLoader<K, V> {

        private final Map<K, V> loaded;

        private MapLoader(final Map<K, V> loaded) {
            this.loaded = loaded;
        }

        @Override
        public V load(final K key) throws CacheLoaderException {
            return loaded.get(key);
        }

        @Override
        public Map<K, V> loadAll(final Iterable<? extends K> keys) throws CacheLoaderException {
            throw new UnsupportedOperationException();
        }
    }
}
