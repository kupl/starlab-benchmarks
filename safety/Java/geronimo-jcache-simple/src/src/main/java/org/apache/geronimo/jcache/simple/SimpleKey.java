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

import java.io.Serializable;

public class SimpleKey<K> implements Serializable {

    private final K key;

    private volatile long lastAccess = 0;

    public SimpleKey(final K key) {
        this.key = key;
    }

    public void access(final long time) {
        lastAccess = time;
    }

    public long lastAccess() {
        return lastAccess;
    }

    public K getKey() {
        return key;
    }

    @Override
    public boolean equals(final Object o) {
        if (this == o)
            return true;
        // if (o == null || getClass() != o.getClass()) return false; // not needed normally
        final SimpleKey k = SimpleKey.class.cast(o);
        return key.equals(k.key);

    }

    @Override
    public int hashCode() {
        return key.hashCode();
    }
}
