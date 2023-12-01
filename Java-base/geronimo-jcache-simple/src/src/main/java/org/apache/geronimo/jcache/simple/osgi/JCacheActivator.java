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
package org.apache.geronimo.jcache.simple.osgi;

import java.util.Dictionary;
import java.util.Hashtable;
import java.util.Objects;
import java.util.stream.Stream;

import javax.cache.spi.CachingProvider;
import javax.enterprise.inject.spi.Extension;

import org.apache.geronimo.jcache.simple.SimpleProvider;
import org.apache.geronimo.jcache.simple.cdi.MakeJCacheCDIInterceptorFriendly;
import org.osgi.framework.Bundle;
import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;
import org.osgi.framework.PrototypeServiceFactory;
import org.osgi.framework.ServiceRegistration;

public class JCacheActivator implements BundleActivator {
    private ServiceRegistration<?> cacheProviderRegistration;
    private ServiceRegistration<?> jcacheExtensionRegistration;

    @Override
    public void start(final BundleContext context) {
        final Dictionary<String, Object> cachingProvider = new Hashtable<>();
        cachingProvider.put("javax.cache.provider", CachingProvider.class.getName());
        cacheProviderRegistration = context.registerService(
                CachingProvider.class, new SimpleProvider(), cachingProvider);

        final Dictionary<String, Object> jcacheExtension = new Hashtable<>();
        jcacheExtension.put("osgi.cdi.extension", "geronimo-jcache-simple");
        jcacheExtension.put("aries.cdi.extension.mode", "implicit"); // always enable/-able since it just enable interceptors
        jcacheExtensionRegistration = context.registerService(
                Extension.class, new PrototypeServiceFactory<Extension>() {
                    @Override
                    public Extension getService(final Bundle bundle, final ServiceRegistration<Extension> registration) {
                        return new MakeJCacheCDIInterceptorFriendly();
                    }

                    @Override
                    public void ungetService(final Bundle bundle, final ServiceRegistration<Extension> registration,
                                             final Extension service) {
                        // no-op
                    }
                }, jcacheExtension);
    }

    @Override
    public void stop(final BundleContext context) {
        Stream.of(cacheProviderRegistration, jcacheExtensionRegistration)
                .filter(Objects::nonNull)
                .forEach(it -> {
                    try {
                        it.unregister();
                    } catch (final IllegalStateException ise) {
                        // no-op
                    }
                });
    }
}
