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
package org.apache.felix.dm.runtime.itest.components;

import java.util.HashMap;
import java.util.Map;

import org.apache.felix.dm.annotation.api.Component;
import org.apache.felix.dm.annotation.api.Property;
import org.apache.felix.dm.annotation.api.ServiceDependency;
import org.apache.felix.dm.annotation.api.Start;
import org.apache.felix.dm.itest.util.Ensure;

/**
 * This test validates that a basic Service may specify some extra service properties
 * from it's start callback.
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@SuppressWarnings({"rawtypes", "serial"})
public class ExtraServiceProperties {
    public final static String ENSURE = "ExtraServiceProperties";
    
    public interface Provider {
    }

    @Component(properties = {@Property(name = "foo", value = "bar")})
    public static class ProviderImpl implements Provider {
        @Start
        Map<String, String> start() {
            return new HashMap<String, String>() {
                {
                    put("foo2", "bar2");
                }
            };
        }
    }

    @Component
    public static class Consumer {
        @ServiceDependency(filter = "(name=" + ENSURE + ")")
        volatile Ensure m_sequencer;

        private volatile Map m_properties;

        @ServiceDependency
        void bindProvider(Map properties, Provider m_provider) {
            m_properties = properties;
        }

        @Start
        void start() {
            System.out.println("provider service properties: " + m_properties);
            if ("bar".equals(m_properties.get("foo"))) {
                m_sequencer.step(1);
            }

            if ("bar2".equals(m_properties.get("foo2"))) {
                m_sequencer.step(2);
            }
        }
    }
}
