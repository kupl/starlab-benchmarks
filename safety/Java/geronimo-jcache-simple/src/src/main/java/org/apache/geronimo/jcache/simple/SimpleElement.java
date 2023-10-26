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

import javax.cache.expiry.Duration;

public class SimpleElement<V> {

    private final V element;

    private final long end;

    public SimpleElement(final V element, final Duration duration) {
        this.element = element;
        this.end = duration == null || duration.isEternal() ? Long.MAX_VALUE
                        : ((System.nanoTime() + duration.getTimeUnit().toNanos(duration.getDurationAmount())) / 1000);
    }

    public V getElement() {
        return element;
    }

    public boolean isExpired() {
        return end != -1 && (end == 0 || Times.now(false) > end);
    }
}
