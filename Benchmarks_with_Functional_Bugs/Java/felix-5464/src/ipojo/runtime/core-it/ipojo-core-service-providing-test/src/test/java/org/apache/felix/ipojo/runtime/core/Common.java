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

package org.apache.felix.ipojo.runtime.core;

import org.ow2.chameleon.testing.helpers.BaseTest;

import java.util.Arrays;
import java.util.List;

import static junit.framework.Assert.fail;

/**
 * Bootstrap the test from this project
 */
public class Common extends BaseTest {

    @Override
    public boolean deployConfigAdmin() {
        return true;
    }

    @Override
    protected List<String> getExtraExports() {
        return Arrays.asList(
                "org.apache.felix.ipojo.runtime.core.components",
                "org.apache.felix.ipojo.runtime.core.components.inherited",
                "org.apache.felix.ipojo.runtime.core.components.strategies",
                "org.apache.felix.ipojo.runtime.core.components.inheritance.a",
                "org.apache.felix.ipojo.runtime.core.components.inheritance.b"
        );
    }

    public void assertContains(String s, String[] arrays, String object) {
        for (String suspect : arrays) {
            if (object.equals(suspect)) {
                return;
            }
        }
        fail("Assertion failed : " + s);
    }


}
