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

package org.apache.felix.ipojo.runtime.core.api;

import org.apache.felix.ipojo.ComponentInstance;
import org.apache.felix.ipojo.ServiceContext;
import org.apache.felix.ipojo.composite.CompositeManager;
import org.ops4j.pax.exam.Option;
import org.ops4j.pax.exam.spi.reactors.ExamReactorStrategy;
import org.ops4j.pax.exam.spi.reactors.PerMethod;
import org.ow2.chameleon.testing.helpers.BaseTest;

import java.util.Arrays;
import java.util.List;

import static org.ops4j.pax.exam.CoreOptions.mavenBundle;

/**
 * Bootstrap the test from this project
 */
@ExamReactorStrategy(PerMethod.class)
public class Common extends BaseTest {

    public static ServiceContext getServiceContext(ComponentInstance ci) {
        if (ci instanceof CompositeManager) {
            return ((CompositeManager) ci).getServiceContext();
        } else {
            throw new RuntimeException("Cannot get the service context from a non composite instance");
        }
    }

    @Override
    public boolean deployiPOJOComposite() {
        return true;
    }

    @Override
    public boolean deployConfigAdmin() {
        return true;
    }

    @Override
    protected Option[] getCustomOptions() {
        return new Option[]{
                mavenBundle("org.apache.felix", "org.apache.felix.ipojo.handler.whiteboard").versionAsInProject(),
                mavenBundle("org.apache.felix", "org.apache.felix.ipojo.api").versionAsInProject()
        };
    }

    @Override
    protected List<String> getExtraExports() {
        return Arrays.asList(
                "org.apache.felix.ipojo.runtime.core.api.components"
        );
    }
}
