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
package org.apache.commons.collections.set;

import java.util.Comparator;
import java.util.Set;
import java.util.SortedSet;

/**
 * Decorates another <code>SortedSet</code> to provide additional behaviour.
 * <p>
 * Methods are forwarded directly to the decorated set.
 *
 * @param <E> the type of the elements in the sorted set
 * @since Commons Collections 3.0
 * @version $Revision: 966327 $ $Date: 2010-07-21 19:39:49 +0200 (mer., 21 juil. 2010) $
 *
 * @author Stephen Colebourne
 */
public abstract class AbstractSortedSetDecorator<E>
        extends AbstractSetDecorator<E>
        implements SortedSet<E> {

    /** Serialization version */
    private static final long serialVersionUID = -3462240946294214398L;

    /**
     * Constructor only used in deserialization, do not use otherwise.
     * @since Commons Collections 3.1
     */
    protected AbstractSortedSetDecorator() {
        super();
    }

    /**
     * Constructor that wraps (not copies).
     * 
     * @param set  the set to decorate, must not be null
     * @throws IllegalArgumentException if set is null
     */
    protected AbstractSortedSetDecorator(Set<E> set) {
        super(set);
    }

    /**
     * Gets the set being decorated.
     * 
     * @return the decorated set
     */
    @Override
    protected SortedSet<E> decorated() {
        return (SortedSet<E>) super.decorated();
    }

    //-----------------------------------------------------------------------
    public SortedSet<E> subSet(E fromElement, E toElement) {
        return decorated().subSet(fromElement, toElement);
    }

    public SortedSet<E> headSet(E toElement) {
        return decorated().headSet(toElement);
    }

    public SortedSet<E> tailSet(E fromElement) {
        return decorated().tailSet(fromElement);
    }

    public E first() {
        return decorated().first();
    }

    public E last() {
        return decorated().last();
    }

    public Comparator<? super E> comparator() {
        return decorated().comparator();
    }

}
