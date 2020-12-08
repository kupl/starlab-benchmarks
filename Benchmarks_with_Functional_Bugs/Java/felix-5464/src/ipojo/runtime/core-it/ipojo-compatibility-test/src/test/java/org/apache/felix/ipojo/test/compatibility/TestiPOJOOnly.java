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

package org.apache.felix.ipojo.test.compatibility;

import org.apache.felix.ipojo.test.compatibility.service.CheckService;
import org.junit.Test;
import org.ops4j.pax.exam.Option;

import static org.fest.assertions.Assertions.assertThat;

/**
 * Check a configuration with iPOJO used for the provider and consumer.
 *
 * This test is intended to pass on Felix, KF and Equinox
 */
public class TestiPOJOOnly extends Common {

    @Override
    public Option[] bundles() {
        return new Option[] {
                iPOJOHelloConsumer(),
                iPOJOHelloProvider(),
        };
    }

    @Test
    public void test() {
        CheckService checker = osgiHelper.getServiceObject(CheckService.class);
        assertThat(checker).isNotNull();
        assertThat(checker.data().get("result")).isEqualTo("hello john doe");
    }
}
