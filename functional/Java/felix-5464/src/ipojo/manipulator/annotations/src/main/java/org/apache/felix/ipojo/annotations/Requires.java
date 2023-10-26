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
package org.apache.felix.ipojo.annotations;

import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Target;
import java.util.Comparator;

/**
 * This annotation declares a service requirement.
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@Target({ElementType.FIELD, ElementType.PARAMETER, ElementType.ANNOTATION_TYPE})
@Inherited
public @interface Requires {

    /**
     * Set the LDAP filter of the dependency.
     * Default : no filter
     */
    String filter() default "";

    /**
     * Set if the dependency is optional.
     * Default : false
     */
    boolean optional() default false;

    /**
     * Set the dependency id.
     * Default : empty
     */
    String id() default "";

    /**
     * Enable / Disable nullable pattern.
     * Default : true
     */
    boolean nullable() default true;

    /**
     * Set the default-implementation to use if the dependency is optional,
     * and no providers are available.
     * The class must implement the required service interface.
     * Default : no default-implementation
     */
    Class defaultimplementation() default Class.class;

    /**
     * Set the exception to throw when the service is not available. This attribute can only be used for optional
     * dependencies. It must be a subclass of {@link RuntimeException}.
     * Default: no exception.
     */
    Class<? extends RuntimeException> exception() default RuntimeException.class;

    /**
     * Set the binding policy.
     * Acceptable policy are dynamic, static and dynamic-priority.
     * Default: dynamic.
     */
    BindingPolicy policy() default BindingPolicy.DYNAMIC;

    /**
     * Set the comparator.
     * The indicated class must implement {@link Comparator}
     */
    Class<?extends Comparator> comparator() default Comparator.class;

    /**
     * Set the from attribute.
     */
    String from() default "";

    /**
     * Set the required service specification.
     * This attribute is required for Collection field.
     */
    Class specification() default Object.class;

    /**
     * Set to true if the service dependency is injected
     * as a proxy.
     * Default: true
     */
    boolean proxy() default true;


    /**
     * Set the time to wait before applying the 'no service available' action.
     * This attribute is only valid for optional dependencies.
     * The time is set in milliseconds.
     * Default: no timeout
     */
    int timeout() default -1;
}
