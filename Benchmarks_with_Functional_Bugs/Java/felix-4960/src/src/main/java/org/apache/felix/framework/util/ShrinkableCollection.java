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
package org.apache.felix.framework.util;

import java.util.Collection;
import java.util.Iterator;

/**
 * A collection wrapper that only permits clients to shrink the collection.
**/
public class ShrinkableCollection<T> implements Collection<T>
{
    private final Collection<T> m_delegate;

    public ShrinkableCollection(Collection<T> delegate)
    {
        m_delegate = delegate;
    }

    public boolean add(T o)
    {
        throw new UnsupportedOperationException();
    }

    public boolean addAll(Collection<? extends T> c)
    {
        throw new UnsupportedOperationException();
    }

    public void clear()
    {
        m_delegate.clear();
    }

    public boolean contains(Object o)
    {
        return m_delegate.contains(o);
    }

    public boolean containsAll(Collection<?> c)
    {
        return m_delegate.containsAll(c);
    }

    @Override
    public boolean equals(Object o)
    {
        return m_delegate.equals(o);
    }

    @Override
    public int hashCode()
    {
        return m_delegate.hashCode();
    }

    public boolean isEmpty()
    {
        return m_delegate.isEmpty();
    }

    public Iterator iterator()
    {
        return m_delegate.iterator();
    }

    public boolean remove(Object o)
    {
        return m_delegate.remove(o);
    }

    public boolean removeAll(Collection<?> c)
    {
        return m_delegate.removeAll(c);
    }

    public boolean retainAll(Collection<?> c)
    {
        return m_delegate.retainAll(c);
    }

    public int size()
    {
        return m_delegate.size();
    }

    public Object[] toArray()
    {
        return m_delegate.toArray();
    }

    public <A> A[] toArray(A[] a)
    {
        return m_delegate.toArray(a);
    }
}