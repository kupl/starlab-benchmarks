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
package org.apache.felix.dm.lambda.itest;

import static org.apache.felix.dm.lambda.DependencyManagerActivator.adapter;
import static org.apache.felix.dm.lambda.DependencyManagerActivator.component;

import java.util.Dictionary;
import java.util.Hashtable;

import org.apache.felix.dm.Component;
import org.apache.felix.dm.DependencyManager;
import org.junit.Assert;
import org.osgi.framework.ServiceRegistration;

/**
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
public class AdapterWithoutPropagationTest extends TestBase {
    
    public void testAdapterNoPropagate() {
        DependencyManager m = getDM();
        // helper class that ensures certain steps get executed in sequence
        Ensure e = new Ensure();

        // The provider has a "foo=bar" property
        ServiceProvider serviceProvider = new ServiceProvider(e);
        Component provider = component(m).provides(OriginalService.class).properties(foo -> "bar").impl(serviceProvider).build();

        // The Adapter will see the "foo=bar" property from the adaptee
        Component adapter = adapter(m, OriginalService.class)
            .propagate(false).add("set").change("change").provides(AdaptedService.class).impl(new ServiceAdapter(e)).build();

        // The consumer depends on the AdaptedService, but won't see foo=bar property from the adaptee 
        Component consumer = component(m) 
            .impl(new ServiceConsumer(e)).withSvc(AdaptedService.class, b -> b.add("set").change("change")).build();
        
        // add the provider and the adapter
        m.add(provider);
        m.add(adapter);
        // Checks if the adapter has been started and has seen the adaptee properties
        e.waitForStep(1, 5000);
        
        // add a consumer that must not see the adaptee service properties
        m.add(consumer);
        e.waitForStep(2, 5000);
        
        // change the service properties of the provider, and check that the adapter callback instance is caled.
        serviceProvider.changeServiceProperties();
        e.waitForStep(3, 5000);
        
        // cleanup
        m.clear();
    }

    public void testAdapterNoPropagateRef() {
        DependencyManager m = getDM();
        // helper class that ensures certain steps get executed in sequence
        Ensure e = new Ensure();

        // The provider has a "foo=bar" property
        ServiceProvider serviceProvider = new ServiceProvider(e);
        Component provider = component(m).provides(OriginalService.class).properties(foo -> "bar").impl(serviceProvider).build();

        // The Adapter will see the "foo=bar" property from the adaptee
        ServiceAdapter saimpl = new ServiceAdapter(e);
        Component adapter = adapter(m, OriginalService.class).propagate(false).impl(saimpl).provides(AdaptedService.class).add(saimpl::set).change(saimpl::change).build();           

        // The consumer depends on the AdaptedService, but won't see foo=bar property from the adaptee 
        ServiceConsumer scimpl = new ServiceConsumer(e);
        Component consumer = component(m).impl(scimpl).withSvc(AdaptedService.class, s -> s.add(scimpl::set).change(scimpl::change)).build();
        
        // add the provider and the adapter
        m.add(provider);
        m.add(adapter);
        // Checks if the adapter has been started and has seen the adaptee properties
        e.waitForStep(1, 5000);
        
        // add a consumer that must not see the adaptee service properties
        m.add(consumer);
        e.waitForStep(2, 5000);
        
        // change the service properties of the provider, and check that the adapter callback instance is caled.
        serviceProvider.changeServiceProperties();
        e.waitForStep(3, 5000);
        
        // cleanup
        m.clear();
    }

    static interface OriginalService {
    }
    
    static interface AdaptedService {
    }
    
    static class ServiceProvider implements OriginalService {
        private volatile ServiceRegistration<?> m_registration; // auto injected when started.
        public ServiceProvider(Ensure e) {
        }
        public void changeServiceProperties() {
            Hashtable<String, String> props = new Hashtable<>();
            props.put("foo", "bar2");
            m_registration.setProperties(props);
        }
    }
    
    public static class ServiceAdapter implements AdaptedService {
        private final Ensure m_ensure;
        
        public ServiceAdapter(Ensure e) {
            m_ensure = e;
        }

        public void set(OriginalService adaptee, Dictionary<String, Object> props) {
            Assert.assertEquals("bar", props.get("foo"));
            m_ensure.step(1);
        }
        
        void change(OriginalService adapted, Dictionary<String, Object> props) {
            Assert.assertEquals("bar2", props.get("foo"));
            m_ensure.step(3);
        }
    }

    static class ServiceConsumer {
        @SuppressWarnings("unused")
		private volatile AdaptedService m_service;
        private final Ensure m_ensure;
        
        public ServiceConsumer(Ensure e) {
            m_ensure = e;
        }
        
        void set(AdaptedService adapted, Dictionary<String, Object> props) {
            Assert.assertNull(props.get("foo"));
            m_ensure.step(2);
        }
        
        void change(AdaptedService adapted, Dictionary<String, Object> props) {
            Assert.assertNull(props.get("foo"));
            Assert.fail("Change callback should not be called");
        }
    }
}


