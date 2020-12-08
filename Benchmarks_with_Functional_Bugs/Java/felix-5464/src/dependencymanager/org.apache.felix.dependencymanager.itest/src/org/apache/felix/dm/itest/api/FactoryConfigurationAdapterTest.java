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
package org.apache.felix.dm.itest.api;

import java.util.Dictionary;
import java.util.Hashtable;
import java.util.Map;

import org.apache.felix.dm.Component;
import org.apache.felix.dm.DependencyManager;
import org.apache.felix.dm.itest.util.Ensure;
import org.apache.felix.dm.itest.util.TestBase;
import org.junit.Assert;
import org.osgi.service.cm.ConfigurationAdmin;

/**
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@SuppressWarnings({"unchecked", "rawtypes", "serial"})
public class FactoryConfigurationAdapterTest extends TestBase
{
    private static Ensure m_ensure;
    
    public void testFactoryConfigurationAdapter() {
    	testFactoryConfigurationAdapter(Adapter.class, "updated");
    }
    
    public void testFactoryConfigurationAdapterWithUpdatedCallbackThatTakesComponentAsParameter() {
    	testFactoryConfigurationAdapter(AdapterWithUpdateMethodThatTakesComponentAsParameter.class, "updatedWithComponent");
    }
    
    public void testFactoryConfigurationAdapter(Class<?> adapterImplClass, String adapterUpdate) {
        DependencyManager m = getDM();
        // helper class that ensures certain steps get executed in sequence
        m_ensure = new Ensure();
        
        // Create a Configuration instance, which will create/update/remove a configuration for factoryPid "MyFactoryPid"
        FactoryConfigurationCreator configurator = new FactoryConfigurationCreator(m_ensure, "MyFactoryPid", 1, "key", "value1");
        Component s1 = m.createComponent()
            .setImplementation(configurator)
            .add(m.createServiceDependency()
                .setService(ConfigurationAdmin.class)
                .setRequired(true));

        // Create an Adapter that will be instantiated, once the configuration is created.
        // This Adapter provides an AdapterService, and depends on an AdapterExtraDependency service.
        Component s2 = m.createFactoryConfigurationAdapterService("MyFactoryPid", adapterUpdate, true /* propagate CM settings */)
                      .setInterface(AdapterService.class.getName(), new Hashtable() {{ put("foo", "bar"); }})
                      .setImplementation(adapterImplClass);

        s2.add(m.createServiceDependency()
            .setService(AdapterExtraDependency.class)
            .setRequired(true)
            .setAutoConfig(true));
        
        // Create extra adapter service dependency upon which our adapter depends on.
        Component s3 = m.createComponent()
            .setImplementation(new AdapterExtraDependency())
            .setInterface(AdapterExtraDependency.class.getName(), null);
        
        // Create an AdapterService Consumer
        Component s4 = m.createComponent()
            .setImplementation(AdapterServiceConsumer.class)
            .add(m.createServiceDependency()
                .setService(AdapterService.class)
                .setRequired(true)
                .setCallbacks("bind", "change", "remove"));
        
        // Start services
        m.add(s1);
        m.add(s2);
        m.add(s3);
        m.add(s4);
        
        // Wait for step 8: the AdapterService consumer has been injected with the AdapterService, and has called the doService method.
        m_ensure.waitForStep(8, 10000);
        
        // Modify configuration.
        configurator.update("key", "value2");
        
        // Wait for step 13: the AdapterService has been updated, and the AdapterService consumer has seen the change
        m_ensure.waitForStep(13, 10000);
        
        // Remove the configuration
        m.remove(s1); // The stop method will remove the configuration
        m_ensure.waitForStep(16, 10000);
        m.clear();
    }

    public interface AdapterService {
        public void doService();
    }
    
    public static class AdapterExtraDependency {
    }

    public static class Adapter implements AdapterService {
        volatile AdapterExtraDependency m_extraDependency; // extra dependency.
        private int updateCount;
        
        void updated(Dictionary settings) {
            updateCount ++;
            if (updateCount == 1) {
                m_ensure.step(2);
                Assert.assertEquals(true, "value1".equals(settings.get("key")));
                m_ensure.step(3);
            } else if (updateCount == 2) {
                m_ensure.step(9);
                Assert.assertEquals(true, "value2".equals(settings.get("key")));
                m_ensure.step(10);
            } else {
                Assert.fail("wrong call to updated method: count=" + updateCount);
            }
        }

        public void doService() {   
            m_ensure.step(8);
        }
        
        public void start() {
            m_ensure.step(4);
            Assert.assertNotNull(m_extraDependency);
            m_ensure.step(5);
        }
        
        public void stop() {
            m_ensure.step(16);
        }
    }
    
    public static class AdapterWithUpdateMethodThatTakesComponentAsParameter extends Adapter {
        void updatedWithComponent(Component component, Dictionary settings) {
        	Assert.assertNotNull(component);
        	Assert.assertEquals(this, component.getInstance());
        	super.updated(settings);
        }
    }

    public static class AdapterServiceConsumer {
        private AdapterService m_adapterService;
        private Map m_adapterServiceProperties;
        
        void bind(Map serviceProperties, AdapterService adapterService) {
            m_ensure.step(6);
            m_adapterService = adapterService;
            m_adapterServiceProperties = serviceProperties;
        }
        
        void change(Map serviceProperties, AdapterService adapterService) {
            m_ensure.step(11);
            Assert.assertEquals(true, "value2".equals(m_adapterServiceProperties.get("key")));
            m_ensure.step(12);
            Assert.assertEquals(true, "bar".equals(m_adapterServiceProperties.get("foo")));
            m_ensure.step(13);
        }
        
        public void start() {
            m_ensure.step(7);
            Assert.assertNotNull(m_adapterService);
            Assert.assertEquals(true, "value1".equals(m_adapterServiceProperties.get("key")));
            Assert.assertEquals(true, "bar".equals(m_adapterServiceProperties.get("foo")));
            m_adapterService.doService();
        }
        
        public void stop() {
            m_ensure.step(14);
        }
        
        void remove(AdapterService adapterService) {
            m_ensure.step(15);
        }
    }
}
