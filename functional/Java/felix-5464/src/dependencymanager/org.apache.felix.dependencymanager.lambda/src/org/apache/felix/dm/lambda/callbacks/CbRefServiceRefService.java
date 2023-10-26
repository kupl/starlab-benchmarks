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
package org.apache.felix.dm.lambda.callbacks;

import java.util.Objects;

import org.osgi.framework.ServiceReference;

/**
 * Represents a callback(ServiceReference, Service, ServiceReference, Service) that is invoked on a Component implementation class. 
 * The type of the class on which the callback is invoked on is represented by the T generic parameter.
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@FunctionalInterface
public interface CbRefServiceRefService<T, S> extends SerializableLambda {
    /**
     * Handles the given arguments.
     * @param instance the Component implementation instance on which the callback is invoked on. 
     * @param oldRef first callback param
     * @param old second callback param
     * @param replaceRef third callback param
     * @param replace fourth callback param
     */
    void accept(T instance, ServiceReference<S> oldRef, S old, ServiceReference<S> replaceRef, S replace);

    default CbRefServiceRefService<T, S> andThen(CbRefServiceRefService<? super T, S> after) {
        Objects.requireNonNull(after);
        return (T instance, ServiceReference<S> oldRef, S old, ServiceReference<S> replaceRef, S replace) -> {
            accept(instance, oldRef, old, replaceRef, replace);
            after.accept(instance, oldRef, old, replaceRef, replace);
        };
    }
}
