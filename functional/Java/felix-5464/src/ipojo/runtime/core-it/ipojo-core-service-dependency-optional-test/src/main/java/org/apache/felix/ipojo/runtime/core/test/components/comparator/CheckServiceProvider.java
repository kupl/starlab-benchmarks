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
package org.apache.felix.ipojo.runtime.core.test.components.comparator;

import org.apache.felix.ipojo.runtime.core.test.services.CheckService;
import org.apache.felix.ipojo.runtime.core.test.services.FooService;

import java.util.Properties;

public class CheckServiceProvider implements CheckService {

    FooService fs;

    FooService fs2;

    FooService[] fss;


    public boolean check() {
        return fs.foo();
    }

    public Properties getProps() {
        Properties props = new Properties();
        props.put("fs", new Integer(fs.getInt()));
        props.put("fs2", new Integer(fs2.getInt()));

        int[] grades = new int[fss.length];

        for (int i = 0; i < grades.length; i++) {
            grades[i] = fss[i].getInt();
        }

        props.put("fss", grades);

        return props;
    }

    void bind(FooService svc) {
        fs2 = svc;
    }

    void unbind(FooService svc) {
        fs2 = null;
    }


}
