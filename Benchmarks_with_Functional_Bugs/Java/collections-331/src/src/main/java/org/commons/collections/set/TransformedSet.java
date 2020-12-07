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

import java.util.Set;

import org.apache.commons.collections.Transformer;
import org.apache.commons.collections.collection.TransformedCollection;

/**
 * Decorates another <code>Set</code> to transform objects that are added.
 * <p>
 * The add methods are affected by this class.
 * Thus objects must be removed or searched for using their transformed form.
 * For example, if the transformation converts Strings to Integers, you must
 * use the Integer form to remove objects.
 * <p>
 * This class is Serializable from Commons Collections 3.1.
 *
 * @since Commons Collections 3.0
 * @version $Revision: 1024102 $ $Date: 2010-10-19 05:28:51 +0200 (mar., 19 oct. 2010) $
 *
 * @author Stephen Colebourne
 */
public class TransformedSet<E> extends TransformedCollection<E> implements Set<E> {

    /** Serialization version */
    private static final long serialVersionUID = 306127383500410386L;

    /**
     * Factory method to create a transforming set.
     * <p>
     * If there are any elements already in the set being decorated, they
     * are NOT transformed.
     * Constrast this with {@link #decorateTransform}.
     * 
     * @param set  the set to decorate, must not be null
     * @param transformer  the transformer to use for conversion, must not be null
     * @throws IllegalArgumentException if set or transformer is null
     */
    public static <E> Set<E> decorate(Set<E> set, Transformer<? super E, ? extends E> transformer) {
        return new TransformedSet<E>(set, transformer);
    }
    
    /**
     * Factory method to create a transforming set that will transform
     * existing contents of the specified set.
     * <p>
     * If there are any elements already in the set being decorated, they
     * will be transformed by this method.
     * Constrast this with {@link #decorate}.
     * 
     * @param set  the set to decorate, must not be null
     * @param transformer  the transformer to use for conversion, must not be null
     * @return a new transformed Set
     * @throws IllegalArgumentException if set or transformer is null
     * @since Commons Collections 3.3
     */
    public static <E> Set<E> decorateTransform(Set<E> set, Transformer<? super E, ? extends E> transformer) {
        TransformedSet<E> decorated = new TransformedSet<E>(set, transformer);
        if (transformer != null && set != null && set.size() > 0) {
            @SuppressWarnings("unchecked") // set is type E
            E[] values = (E[]) set.toArray();
            set.clear();
            for(int i=0; i<values.length; i++) {
                decorated.decorated().add(transformer.transform(values[i]));
            }
        }
        return decorated;
    }

    //-----------------------------------------------------------------------
    /**
     * Constructor that wraps (not copies).
     * <p>
     * If there are any elements already in the set being decorated, they
     * are NOT transformed.
     * 
     * @param set  the set to decorate, must not be null
     * @param transformer  the transformer to use for conversion, must not be null
     * @throws IllegalArgumentException if set or transformer is null
     */
    protected TransformedSet(Set<E> set, Transformer<? super E, ? extends E> transformer) {
        super(set, transformer);
    }

}
