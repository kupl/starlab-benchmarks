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

import org.apache.commons.collections.BidiMap;
import org.apache.commons.collections.MapIterator;
import org.apache.commons.collections.map.AbstractMapDecorator;

/** 
 * Provides a base decorator that enables additional functionality to be added
 * to a BidiMap via decoration.
 * <p>
 * Methods are forwarded directly to the decorated map.
 * <p>
 * This implementation does not perform any special processing with the map views.
 * Instead it simply returns the set/collection from the wrapped map. This may be
 * undesirable, for example if you are trying to write a validating implementation
 * it would provide a loophole around the validation.
 * But, you might want that loophole, so this class is kept simple.
 *
 * @since Commons Collections 3.0
 * @version $Revision: 966327 $ $Date: 2010-07-21 19:39:49 +0200 (mer., 21 juil. 2010) $
 *
 * @author Stephen Colebourne
 */
public abstract class AbstractBidiMapDecorator<K, V> extends AbstractMapDecorator<K, V> implements
        BidiMap<K, V> {

    /**
     * Constructor that wraps (not copies).
     *
     * @param map  the map to decorate, must not be null
     * @throws IllegalArgumentException if the collection is null
     */
    protected AbstractBidiMapDecorator(BidiMap<K, V> map) {
        super(map);
    }

    /**
     * Gets the map being decorated.
     * 
     * @return the decorated map
     */
    @Override
    protected BidiMap<K, V> decorated() {
        return (BidiMap<K, V>) super.decorated();
    }

    //-----------------------------------------------------------------------
    @Override
    public MapIterator<K, V> mapIterator() {
        return decorated().mapIterator();
    }

    public K getKey(Object value) {
        return decorated().getKey(value);
    }

    public K removeValue(Object value) {
        return decorated().removeValue(value);
    }

    public BidiMap<V, K> inverseBidiMap() {
        return decorated().inverseBidiMap();
    }

}
