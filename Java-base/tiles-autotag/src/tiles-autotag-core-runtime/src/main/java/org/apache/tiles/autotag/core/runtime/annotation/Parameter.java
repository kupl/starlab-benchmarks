/*
 * $Id$
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
package org.apache.tiles.autotag.core.runtime.annotation;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Specifies behaviour for a parameter of the "execute" method of a template class.
 *
 * @version $Rev$ $Date$
 */
@Retention(RetentionPolicy.SOURCE)
@Target(ElementType.PARAMETER)
public @interface Parameter {

    /**
     * Indicates to use the parameter name itself for the exported name.
     */
    String SAME_NAME = "USE THE SAME NAME";

    /**
     * Returns the name of the exported property name.
     */
    String name() default SAME_NAME;

    /**
     * Indicates that this parameter is required.
     */
    boolean required() default false;

    /**
     * Indicates the default value, as it will be written in Java code.
     */
    String defaultValue() default "null";
}
