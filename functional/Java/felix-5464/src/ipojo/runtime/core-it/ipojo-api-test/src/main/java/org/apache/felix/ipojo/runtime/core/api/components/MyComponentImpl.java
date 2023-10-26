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

package org.apache.felix.ipojo.runtime.core.api.components;

import org.apache.felix.ipojo.annotations.Component;
import org.apache.felix.ipojo.runtime.core.api.services.Foo;

/**
 * This class is marked as a component to be manipulated.
 */
@Component(name="do-not-use-this-factory", public_factory = false)
public class MyComponentImpl {
    
    private Foo myFoo;
    
    private int anInt;
    
    public MyComponentImpl() {
        anInt = 2;
    }
    
    public MyComponentImpl(int i) {
        anInt = i;
    }

    public void start() {
       myFoo.doSomething();
       if (anInt > 0) {
           System.out.println("Set int to " + anInt);
       }
    }

}
