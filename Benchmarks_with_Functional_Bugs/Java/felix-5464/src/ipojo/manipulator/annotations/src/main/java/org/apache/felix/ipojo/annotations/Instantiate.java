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
 * This annotation is used to create an 'empty' instance of the
 * current component type.
 *
 * Notice that all mandatory properties should have a default value, otherwise
 * configuration is declared unacceptable and the instance creation fails.
 *
 * <pre>
 *     {@linkplain org.apache.felix.ipojo.annotations.Component @Component}
 *     {@code @Instantiate}
 *     public class MyComponent {
 *         // ...
 *     }
 * </pre>
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@Target({ElementType.TYPE, ElementType.ANNOTATION_TYPE})
public @interface Instantiate {
    
    /**
     * Optional attribute to set the instance name.
     * Default: unique generated name
     * <pre>
     *     {@linkplain org.apache.felix.ipojo.annotations.Component @Component}
     *     {@code @Instantiate("my-default-component")}
     *     public class MyComponent {
     *         // ...
     *     }
     * </pre>
     */
    String name() default ""; 
}
