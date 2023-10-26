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
package org.apache.geronimo.jcache.simple.tck;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotSame;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CompletionStage;
import java.util.concurrent.ExecutionException;

import javax.cache.annotation.BeanProvider;
import javax.cache.annotation.CacheResult;
import javax.enterprise.context.Dependent;

import org.apache.geronimo.jcache.simple.cdi.CacheResolverFactoryImpl;
import org.junit.Test;

public class CacheResultCompletionStageTest {

    @Test
    public void run() throws ExecutionException, InterruptedException {
        final BeanProvider ioc = new OWBBeanProvider();
        final MyService myService = ioc.getBeanByType(MyService.class);
        final Resolver resolver = ioc.getBeanByType(Resolver.class);

        {   // cache is active
            final CompletionStage<String> result = myService.get();
            assertSame(result, myService.get());

            // success
            result.toCompletableFuture().complete("ok");
            assertEquals("ok", result.toCompletableFuture().get());
        }

        {   // evict then fail
            resolver.evict();
            assertFalse(myService.get().toCompletableFuture().isDone());
            final CompletionStage<String> beforeFailure = myService.get();
            beforeFailure.toCompletableFuture().completeExceptionally(new IllegalArgumentException("failed"));

            final CompletionStage<String> newInstanceSinceNotCached = myService.get();
            assertNotSame(beforeFailure, newInstanceSinceNotCached);
            assertFalse(newInstanceSinceNotCached.toCompletableFuture().isCompletedExceptionally());
            assertFalse(newInstanceSinceNotCached.toCompletableFuture().isDone());
        }

        {   // evict then fail a not configured exception
            resolver.evict();
            assertFalse(myService.get().toCompletableFuture().isDone());
            final CompletionStage<String> beforeFailure = myService.get();
            beforeFailure.toCompletableFuture().completeExceptionally(new IllegalStateException("failed"));

            final CompletionStage<String> newInstanceSinceNotCached = myService.get();
            assertNotSame(beforeFailure, newInstanceSinceNotCached);
            assertFalse(newInstanceSinceNotCached.toCompletableFuture().isCompletedExceptionally());
            assertFalse(newInstanceSinceNotCached.toCompletableFuture().isDone());
        }

        {   // evict then fail a cached exception - so cached
            resolver.evict();
            assertFalse(myService.get().toCompletableFuture().isDone());
            final CompletionStage<String> beforeFailure = myService.get();
            beforeFailure.toCompletableFuture().completeExceptionally(new NullPointerException("failed"));

            final CompletionStage<String> newInstanceSinceNotCached = myService.get();
            assertSame(beforeFailure, newInstanceSinceNotCached);
            assertTrue(newInstanceSinceNotCached.toCompletableFuture().isCompletedExceptionally());
            try {
                newInstanceSinceNotCached.toCompletableFuture().get();
                fail("npe expected");
            } catch (final ExecutionException npe) {
                assertTrue(NullPointerException.class.isInstance(npe.getCause()));
                assertEquals("failed", NullPointerException.class.cast(npe.getCause()).getMessage());
            }
        }
    }

    @Dependent
    public static class Resolver extends CacheResolverFactoryImpl {
        public void evict() {
            getCacheManager()
                    .getCache("org.apache.geronimo.jcache.simple.tck.CacheResultCompletionStageTest$MyService.get()")
                    .clear();
        }
    }

    @Dependent
    public static class MyService {

        @CacheResult(nonCachedExceptions = IllegalArgumentException.class, cachedExceptions = NullPointerException.class,
                exceptionCacheName = "org.apache.geronimo.jcache.simple.tck.CacheResultCompletionStageTest$MyService.get()_exception",
                cacheResolverFactory = Resolver.class)
        public CompletionStage<String> get() {
            return new CompletableFuture<>();
        }
    }
}
