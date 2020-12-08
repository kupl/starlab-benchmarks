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
package org.apache.felix.ipojo.handlers.event;

import java.lang.annotation.ElementType;
import java.lang.annotation.Target;


/**
 * Event Admin Subscriber handler.
 * Be aware that despite is it provided in the annotations jar,
 * it refers to an external handler.
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@Target({ElementType.METHOD, ElementType.ANNOTATION_TYPE})
public @interface Subscriber {

    /**
     * Sets the subscriber name.
     */
    String name();

    /**
     * Sets topics on which event are received.
     * The topics are separated by a ',' such as in
     * "foo, bar".
     * Default : no topics (configured in the instance configuration)
     */
    String topics() default "";

    /**
     * Sets the data key in which the data is
     * stored.
     * Default : no key
     * @deprecated replaced by dataKey
     */
    String data_key() default "";

    /**
     * Sets the data type (type of the received data).
     * Default : no type.
     * @deprecated replaced by dataType
     */
    String data_type() default "";

    /**
     * Sets the data key in which the data is
     * stored.
     * Default : no key
     */
    String dataKey() default "";

    /**
     * Sets the data type (type of the received data).
     * Default : no type.
     */
    String dataType() default "";


    /**
     * Sets the event filter. Only event matching with the
     * specified LDAP filter are received.
     * default : no filter;
     */
    String filter() default "";

}
