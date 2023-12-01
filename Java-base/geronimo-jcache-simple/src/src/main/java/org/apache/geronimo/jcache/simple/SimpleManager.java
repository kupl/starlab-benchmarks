/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
package org.apache.geronimo.jcache.simple;

import static java.util.Collections.unmodifiableSet;
import static org.apache.geronimo.jcache.simple.Asserts.assertNotNull;

import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.net.URL;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.Properties;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.cache.Cache;
import javax.cache.CacheException;
import javax.cache.CacheManager;
import javax.cache.configuration.Configuration;
import javax.cache.spi.CachingProvider;

public class SimpleManager implements CacheManager {

    private final CachingProvider provider;

    private final URI uri;

    private final ClassLoader loader;

    private final Properties properties;

    private final ConcurrentMap<String, Cache<?, ?>> caches = new ConcurrentHashMap<>();

    private final Properties configProperties;

    private final ExecutorService executorService;

    private volatile boolean closed = false;

    SimpleManager(final CachingProvider provider, final URI uri, final ClassLoader loader, final Properties properties) {
        this.provider = provider;
        this.uri = uri;
        this.loader = loader;
        this.properties = readConfig(uri, loader, properties);
        this.configProperties = properties;

        ExecutorService executorService = rawProperty("geronimo.pool");
        if (executorService == null) {
            final int poolSize = Integer.parseInt(this.properties.getProperty("pool.size", "16"));
            final SimpleThreadFactory threadFactory = new SimpleThreadFactory("geronimo-simple-jcache-[" + uri.toASCIIString() + "]-");
            executorService = poolSize > 0 ? Executors.newFixedThreadPool(poolSize, threadFactory)
                    : Executors.newCachedThreadPool(threadFactory);
        }
        this.executorService = executorService;
    }

    private <T> T rawProperty(final String name) {
        final Object value = this.properties.get(name);
        if (value == null) {
            return (T) this.properties.get(name);
        }
        return (T) value;
    }

    private Properties readConfig(final URI uri, final ClassLoader loader, final Properties properties) {
        final Properties props = new Properties();
        try {
            if (SimpleProvider.DEFAULT_URI.toString().equals(uri.toString()) || uri.getScheme().equals("geronimo")
                    || uri.getScheme().equals("classpath")) {

                final Enumeration<URL> resources = loader.getResources(uri.toASCIIString().substring((uri.getScheme() + "://").length()));
                while (resources.hasMoreElements()) {
                    do {
                        addProperties(resources.nextElement(), props);
                    } while (resources.hasMoreElements());
                }
            } else {
                props.load(uri.toURL().openStream());
            }
        } catch (final IOException e) {
            throw new IllegalStateException(e);
        }
        if (properties != null) {
            props.putAll(properties);
        }
        return props;
    }

    private void addProperties(final URL url, final Properties aggregator) {
        try (final InputStream inputStream = url.openStream()) {
            aggregator.load(inputStream);
        } catch (final IOException e) {
            throw new IllegalArgumentException(e);
        }
    }

    private void assertNotClosed() {
        if (isClosed()) {
            throw new IllegalStateException("cache manager closed");
        }
    }

    @Override
    public <K, V, C extends Configuration<K, V>> Cache<K, V> createCache(final String cacheName, final C configuration)
            throws IllegalArgumentException {
        assertNotClosed();
        assertNotNull(cacheName, "cacheName");
        assertNotNull(configuration, "configuration");
        final Class<K> keyType = configuration.getKeyType();
        final Class<V> valueType = configuration.getValueType();
        if (!caches.containsKey(cacheName)) {
            final Cache<K, V> cache = ClassLoaderAwareCache.wrap(loader, new SimpleCache<K, V>(loader, this, cacheName,
                    new SimpleConfiguration<>(configuration, keyType, valueType), properties, executorService));
            caches.putIfAbsent(cacheName, cache);
        } else {
            throw new CacheException("cache " + cacheName + " already exists");
        }
        return (Cache<K, V>) getCache(cacheName, keyType, valueType);
    }

    @Override
    public void destroyCache(final String cacheName) {
        assertNotClosed();
        assertNotNull(cacheName, "cacheName");
        final Cache<?, ?> cache = caches.remove(cacheName);
        if (cache != null && !cache.isClosed()) {
            cache.clear();
            cache.close();
        }
    }

    @Override
    public void enableManagement(final String cacheName, final boolean enabled) {
        assertNotClosed();
        assertNotNull(cacheName, "cacheName");
        final SimpleCache<?, ?> cache = of(cacheName);
        if (cache != null) {
            if (enabled) {
                cache.enableManagement();
            } else {
                cache.disableManagement();
            }
        }
    }

    private SimpleCache<?, ?> of(final String cacheName) {
        return SimpleCache.class.cast(ClassLoaderAwareCache.getDelegate(caches.get(cacheName)));
    }

    @Override
    public void enableStatistics(final String cacheName, final boolean enabled) {
        assertNotClosed();
        assertNotNull(cacheName, "cacheName");
        final SimpleCache<?, ?> cache = of(cacheName);
        if (cache != null) {
            if (enabled) {
                cache.enableStatistics();
            } else {
                cache.disableStatistics();
            }
        }
    }

    @Override
    public synchronized void close() {
        if (isClosed()) {
            return;
        }

        assertNotClosed();
        for (final Cache<?, ?> c : caches.values()) {
            c.close();
        }
        caches.clear();
        for (final Runnable task : executorService.shutdownNow()) {
            task.run();
        }
        closed = true;
        if (SimpleProvider.class.isInstance(provider)) {
            SimpleProvider.class.cast(provider).remove(this);
        } // else throw?
    }

    @Override
    public <T> T unwrap(final Class<T> clazz) {
        if (clazz.isInstance(this)) {
            return clazz.cast(this);
        }
        throw new IllegalArgumentException(clazz.getName() + " not supported in unwrap");
    }

    @Override
    public boolean isClosed() {
        return closed;
    }

    @Override
    public <K, V> Cache<K, V> getCache(final String cacheName) {
        assertNotClosed();
        assertNotNull(cacheName, "cacheName");
        return (Cache<K, V>) doGetCache(cacheName, null, null);
    }

    @Override
    public Iterable<String> getCacheNames() {
        assertNotClosed();
        return unmodifiableSet(new HashSet<>(caches.keySet()));
    }

    @Override
    public <K, V> Cache<K, V> getCache(final String cacheName, final Class<K> keyType, final Class<V> valueType) {
        assertNotClosed();
        assertNotNull(cacheName, "cacheName");
        assertNotNull(keyType, "keyType");
        assertNotNull(valueType, "valueType");
        try {
            return doGetCache(cacheName, keyType, valueType);
        } catch (final IllegalArgumentException iae) {
            throw new ClassCastException(iae.getMessage());
        }
    }

    private <K, V> Cache<K, V> doGetCache(final String cacheName, final Class<K> keyType, final Class<V> valueType) {
        final Cache<K, V> cache = (Cache<K, V>) caches.get(cacheName);
        if (keyType == null && valueType == null) {
            return cache;
        }
        if (cache == null) {
            return null;
        }

        final Configuration<K, V> config = cache.getConfiguration(Configuration.class);
        if ((keyType != null && !config.getKeyType().isAssignableFrom(keyType))
                || (valueType != null && !config.getValueType().isAssignableFrom(valueType))) {
            throw new IllegalArgumentException(
                    "this cache is <" + config.getKeyType().getName() + ", " + config.getValueType().getName() + "> "
                            + " and not <" + keyType.getName() + ", " + valueType.getName() + ">");
        }
        return cache;
    }

    @Override
    public CachingProvider getCachingProvider() {
        return provider;
    }

    @Override
    public URI getURI() {
        return uri;
    }

    @Override
    public ClassLoader getClassLoader() {
        return loader;
    }

    @Override
    public Properties getProperties() {
        return configProperties;
    }

    public void release(final String name) {
        caches.remove(name);
    }
}
