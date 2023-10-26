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
package org.apache.commons.collections.functors;

import java.io.Serializable;

import org.apache.commons.collections.Closure;
import org.apache.commons.collections.Transformer;

/**
 * Closure implementation that calls a Transformer using the input object
 * and ignore the result.
 *
 * @since Commons Collections 3.0
 * @version $Revision: 814997 $ $Date: 2009-09-15 07:29:56 +0200 (mar., 15 sept. 2009) $
 *
 * @author Stephen Colebourne
 */
public class TransformerClosure<E> implements Closure<E>, Serializable {

    /** Serial version UID */
    private static final long serialVersionUID = -5194992589193388969L;

    /** The transformer to wrap */
    private final Transformer<? super E, ?> iTransformer;

    /**
     * Factory method that performs validation.
     * <p>
     * A null transformer will return the <code>NOPClosure</code>.
     * 
     * @param transformer  the transformer to call, null means nop
     * @return the <code>transformer</code> closure
     */
    public static <E> Closure<E> getInstance(Transformer<? super E, ?> transformer) {
        if (transformer == null) {
            return NOPClosure.<E>getInstance();
        }
        return new TransformerClosure<E>(transformer);
    }

    /**
     * Constructor that performs no validation.
     * Use <code>getInstance</code> if you want that.
     * 
     * @param transformer  the transformer to call, not null
     */
    public TransformerClosure(Transformer<? super E, ?> transformer) {
        super();
        iTransformer = transformer;
    }

    /**
     * Executes the closure by calling the decorated transformer.
     * 
     * @param input  the input object
     */
    public void execute(E input) {
        iTransformer.transform(input);
    }

    /**
     * Gets the transformer.
     * 
     * @return the transformer
     * @since Commons Collections 3.1
     */
    public Transformer<? super E, ?> getTransformer() {
        return iTransformer;
    }

}
