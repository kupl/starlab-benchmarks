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

import java.util.ArrayList;
import java.util.List;

import javax.cache.configuration.CacheEntryListenerConfiguration;
import javax.cache.configuration.Factory;
import javax.cache.event.CacheEntryCreatedListener;
import javax.cache.event.CacheEntryEvent;
import javax.cache.event.CacheEntryEventFilter;
import javax.cache.event.CacheEntryExpiredListener;
import javax.cache.event.CacheEntryListener;
import javax.cache.event.CacheEntryListenerException;
import javax.cache.event.CacheEntryRemovedListener;
import javax.cache.event.CacheEntryUpdatedListener;

public class SimpleListener<K, V> implements AutoCloseable {

    private final CacheEntryEventFilter<? super K, ? super V> filter;

    private final CacheEntryListener<? super K, ? super V> delegate;

    private final boolean remove;

    private final boolean expire;

    private final boolean update;

    private final boolean create;

    public SimpleListener(final CacheEntryListenerConfiguration<K, V> cacheEntryListenerConfiguration) {
        final Factory<CacheEntryEventFilter<? super K, ? super V>> filterFactory = cacheEntryListenerConfiguration
                .getCacheEntryEventFilterFactory();
        if (filterFactory == null) {
            this.filter = NoFilter.INSTANCE;
        } else {
            final CacheEntryEventFilter<? super K, ? super V> filter = filterFactory.create();
            this.filter = (CacheEntryEventFilter<? super K, ? super V>) (filter == null ? NoFilter.INSTANCE : filter);
        }

        delegate = cacheEntryListenerConfiguration.getCacheEntryListenerFactory().create();
        remove = CacheEntryRemovedListener.class.isInstance(delegate);
        expire = CacheEntryExpiredListener.class.isInstance(delegate);
        update = CacheEntryUpdatedListener.class.isInstance(delegate);
        create = CacheEntryCreatedListener.class.isInstance(delegate);
    }

    public void onRemoved(final List<CacheEntryEvent<? extends K, ? extends V>> events) throws CacheEntryListenerException {
        if (remove) {
            CacheEntryRemovedListener.class.cast(delegate).onRemoved(filter(events));
        }
    }

    public void onExpired(final List<CacheEntryEvent<? extends K, ? extends V>> events) throws CacheEntryListenerException {
        if (expire) {
            CacheEntryExpiredListener.class.cast(delegate).onExpired(filter(events));
        }
    }

    public void onUpdated(final List<CacheEntryEvent<? extends K, ? extends V>> events) throws CacheEntryListenerException {
        if (update) {
            CacheEntryUpdatedListener.class.cast(delegate).onUpdated(filter(events));
        }
    }

    public void onCreated(final List<CacheEntryEvent<? extends K, ? extends V>> events) throws CacheEntryListenerException {
        if (create) {
            CacheEntryCreatedListener.class.cast(delegate).onCreated(filter(events));
        }
    }

    private Iterable<CacheEntryEvent<? extends K, ? extends V>> filter(
            final List<CacheEntryEvent<? extends K, ? extends V>> events) {
        if (filter == NoFilter.INSTANCE) {
            return events;
        }

        final List<CacheEntryEvent<? extends K, ? extends V>> filtered = new ArrayList<CacheEntryEvent<? extends K, ? extends V>>(
                events.size());
        for (final CacheEntryEvent<? extends K, ? extends V> event : events) {
            if (filter.evaluate(event)) {
                filtered.add(event);
            }
        }
        return filtered;
    }

    @Override
    public void close() throws Exception {
        if (AutoCloseable.class.isInstance(delegate)) {
            AutoCloseable.class.cast(delegate).close();
        }
    }

    public static class NoFilter implements CacheEntryEventFilter<Object, Object> {

        public static final CacheEntryEventFilter<Object, Object> INSTANCE = new NoFilter();

        private NoFilter() {
            // no-op
        }

        @Override
        public boolean evaluate(final CacheEntryEvent<?, ?> event) throws CacheEntryListenerException {
            return true;
        }
    }
}