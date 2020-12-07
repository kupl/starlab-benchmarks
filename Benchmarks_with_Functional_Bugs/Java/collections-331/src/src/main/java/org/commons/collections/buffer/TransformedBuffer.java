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
package org.apache.commons.collections.buffer;

import org.apache.commons.collections.Buffer;
import org.apache.commons.collections.Transformer;
import org.apache.commons.collections.collection.TransformedCollection;

/**
 * Decorates another <code>Buffer</code> to transform objects that are added.
 * <p>
 * The add methods are affected by this class.
 * Thus objects must be removed or searched for using their transformed form.
 * For example, if the transformation converts Strings to Integers, you must
 * use the Integer form to remove objects.
 * <p>
 * This class is Serializable from Commons Collections 3.1.
 *
 * @since Commons Collections 3.0
 * @version $Revision: 1023964 $ $Date: 2010-10-18 21:31:58 +0200 (lun., 18 oct. 2010) $
 *
 * @author Stephen Colebourne
 */
public class TransformedBuffer<E> extends TransformedCollection<E> implements Buffer<E> {

    /** Serialization version */
    private static final long serialVersionUID = -7901091318986132033L;

    /**
     * Factory method to create a transforming buffer.
     * <p>
     * If there are any elements already in the buffer being decorated, they
     * are NOT transformed.
     * Constrast this with {@link #decorateTransform}.
     * 
     * @param buffer  the buffer to decorate, must not be null
     * @param transformer  the transformer to use for conversion, must not be null
     * @return a new transformed Buffer
     * @throws IllegalArgumentException if buffer or transformer is null
     */
    public static <E> Buffer<E> decorate(Buffer<E> buffer, Transformer<? super E, ? extends E> transformer) {
        return new TransformedBuffer<E>(buffer, transformer);
    }
    
    /**
     * Factory method to create a transforming buffer that will transform
     * existing contents of the specified buffer.
     * <p>
     * If there are any elements already in the buffer being decorated, they
     * will be transformed by this method.
     * Constrast this with {@link #decorate}.
     * 
     * @param buffer  the buffer to decorate, must not be null
     * @param transformer  the transformer to use for conversion, must not be null
     * @return a new transformed Buffer
     * @throws IllegalArgumentException if buffer or transformer is null
     * @since Commons Collections 3.3
     */
    public static <E> Buffer<E> decorateTransform(Buffer<E> buffer, Transformer<? super E, ? extends E> transformer) {
        TransformedBuffer<E> decorated = new TransformedBuffer<E>(buffer, transformer); // throws IAE if buffer or transformer is null
        if (buffer.size() > 0) {
            @SuppressWarnings("unchecked") // buffer is type <E>
            E[] values = (E[]) buffer.toArray();
            buffer.clear();
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
     * If there are any elements already in the buffer being decorated, they
     * are NOT transformed.
     * 
     * @param buffer  the buffer to decorate, must not be null
     * @param transformer  the transformer to use for conversion, must not be null
     * @throws IllegalArgumentException if buffer or transformer is null
     */
    protected TransformedBuffer(Buffer<E> buffer, Transformer<? super E, ? extends E> transformer) {
        super(buffer, transformer);
    }

    /**
     * Gets the decorated buffer.
     * 
     * @return the decorated buffer
     */
    protected Buffer<E> getBuffer() {
        return (Buffer<E>) collection;
    }

    //-----------------------------------------------------------------------
    public E get() {
        return getBuffer().get();
    }

    public E remove() {
        return getBuffer().remove();
    }

}
