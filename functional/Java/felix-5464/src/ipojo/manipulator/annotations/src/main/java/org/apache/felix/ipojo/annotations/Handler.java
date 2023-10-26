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
import java.lang.annotation.Target;

/**
 * This annotation declares a handler.
 * This annotation is mandatory to declares an iPOJO handler.
 * <pre>
 *     {@code @Handler}(
 *         name = "my-handler",
 *         namespace = "com.acme.foo"
 *     )
 *     public class MyHandler extends PrimitiveHandler {
 *         // ...
 *     }
 * </pre>

 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@Target({ElementType.TYPE, ElementType.ANNOTATION_TYPE})
public @interface Handler {

    /**
     * Name of the handler (required).
     */
    String name();

    /**
     * Namespace of the handler (required).
     */
    String namespace();

    /**
     * Start-level of the handler.
     * Default: 1.
     */
    int level() default 1;

    /**
     * Enable / Disable the architecture exposition.
     * Default : {@literal false}
     */
    boolean architecture() default false;
}
