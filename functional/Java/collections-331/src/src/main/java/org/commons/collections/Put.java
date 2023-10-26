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
package org.apache.commons.collections;

import java.util.Map;

/**
 * The "write" subset of the {@link Map} interface.
 * @since Commons Collections 5
 * @TODO fix version
 * @version $Revision: 815093 $ $Date: 2009-09-15 07:56:32 +0200 (mar., 15 sept. 2009) $
 * @see Get
 * @author Matt Benson
 */
public interface Put<K, V> {

    /**
     * @see Map#clear()
     */
    public void clear();

    /**
     * @see Map#put(Object, Object)
     */
    public Object put(K key, V value);

    /**
     * @see Map#putAll(Map)
     */
    public void putAll(Map<? extends K, ? extends V> t);

}
