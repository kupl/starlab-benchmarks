/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.commons.collections.bidimap;

import org.apache.commons.collections.OrderedBidiMap;
import org.apache.commons.collections.OrderedMapIterator;

/** 
 * Provides a base decorator that enables additional functionality to be added
 * to an OrderedBidiMap via decoration.
 * <p>
 * Methods are forwarded directly to the decorated map.
 * <p>
 * This implementation does not perform any special processing with the map views.
 * Instead it simply returns the inverse from the wrapped map. This may be
 * undesirable, for example if you are trying to write a validating implementation
 * it would provide a loophole around the validation.
 * But, you might want that loophole, so this class is kept simple.
 *
 * @since Commons Collections 3.0
 * @version $Revision: 966315 $ $Date: 2010-07-21 19:03:58 +0200 (mer., 21 juil. 2010) $
 *
 * @author Stephen Colebourne
 */
public abstract class AbstractOrderedBidiMapDecorator<K, V>
        extends AbstractBidiMapDecorator<K, V>
        implements OrderedBidiMap<K, V> {

    /**
     * Constructor that wraps (not copies).
     *
     * @param map  the map to decorate, must not be null
     * @throws IllegalArgumentException if the collection is null
     */
    protected AbstractOrderedBidiMapDecorator(OrderedBidiMap<K, V> map) {
        super(map);
    }

    /**
     * Gets the map being decorated.
     * 
     * @return the decorated map
     */
    @Override
    protected OrderedBidiMap<K, V> decorated() {
        return (OrderedBidiMap<K, V>) super.decorated();
    }

    //-----------------------------------------------------------------------
    @Override
    public OrderedMapIterator<K, V> mapIterator() {
        return decorated().mapIterator();
    }

    public K firstKey() {
        return decorated().firstKey();
    }

    public K lastKey() {
        return decorated().lastKey();
    }

    public K nextKey(K key) {
        return decorated().nextKey(key);
    }

    public K previousKey(K key) {
        return decorated().previousKey(key);
    }

    @Override
    public OrderedBidiMap<V, K> inverseBidiMap() {
        return decorated().inverseBidiMap();
    }

}
