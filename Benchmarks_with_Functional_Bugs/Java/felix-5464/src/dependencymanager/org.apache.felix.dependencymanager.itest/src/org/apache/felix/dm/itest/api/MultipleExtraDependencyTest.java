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

import java.util.Hashtable;

import org.apache.felix.dm.Component;
import org.apache.felix.dm.DependencyManager;
import org.apache.felix.dm.itest.util.Ensure;
import org.apache.felix.dm.itest.util.TestBase;

/**
 * Test which validates multi-dependencies combination.
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@SuppressWarnings({"unchecked", "rawtypes", "serial"})
public class MultipleExtraDependencyTest extends TestBase {
    public void testMultipleExtraDependencies()
    {
        DependencyManager m = getDM();
        Ensure e = new Ensure();
        
        Component sp2 = m.createComponent()
              .setImplementation(ServiceProvider2.class).setInterface(ServiceProvider2.class.getName(), null)
              .add(m.createServiceDependency()
                   .setService(Runnable.class, "(foo=bar)")
                   .setRequired(false)
                   .setAutoConfig("m_runnable"))
              .add(m.createServiceDependency()
                   .setService(Sequencer.class)
                   .setRequired(true)
                   .setCallbacks("bind", null))
              .setCallbacks(null, "start", "stop", null)
              .setComposition("getComposition");
        
        Component sp = m.createComponent()
              .setImplementation(ServiceProvider.class)
              .setInterface(ServiceInterface.class.getName(), 
                            new Hashtable() {{ put("foo", "bar"); }})
              .add(m.createServiceDependency()
                   .setService(Sequencer.class)
                   .setRequired(true)
                   .setAutoConfig("m_sequencer"))
               .add(m.createServiceDependency()
                   .setService(ServiceProvider2.class)
                   .setRequired(true)
                   .setCallbacks("bind", "unbind"))
              .setCallbacks(null, "start", "stop", null);
        
        Component sc = m.createComponent()
              .setImplementation(ServiceConsumer.class)
              .add(m.createServiceDependency()
                   .setService(Sequencer.class)
                   .setRequired(true)
                   .setAutoConfig("m_sequencer"))
               .add(m.createServiceDependency()
                   .setService(ServiceInterface.class, "(foo=bar)")
                   .setRequired(true)
                   .setAutoConfig("m_service"))
              .setCallbacks(null, "start", "stop", null);
        
        Component sequencer = 
            m.createComponent().setImplementation(new SequencerImpl(e))
                             .setInterface(Sequencer.class.getName(), null);
        m.add(sp2);
        m.add(sp);
        m.add(sc);
        m.add(sequencer);
        
        // Check if ServiceProvider component have been initialized orderly
        e.waitForStep(7, 5000);
        
        // Stop the test.annotation bundle
        m.remove(sequencer);
        m.remove(sp);
        m.remove(sp2);
        m.remove(sc);
        
        // And check if ServiceProvider2 has been deactivated orderly
        e.waitForStep(11, 5000);
    }
    
    public interface Sequencer
    {
        void step();
        void step(int step);
        void waitForStep(int step, int timeout);
    }
    
    public static class SequencerImpl implements Sequencer {
        Ensure m_ensure;
        
        public SequencerImpl(Ensure e)
        {
            m_ensure = e;
        }
        
        public void step()
        {
            m_ensure.step();
        }

        public void step(int step)
        {
            m_ensure.step(step);
        }

        public void waitForStep(int step, int timeout)
        {
            m_ensure.waitForStep(step, timeout);
        }  
    }
    
    public interface ServiceInterface
    {
        public void doService();
    }
    
    public static class ServiceConsumer
    {
        volatile Sequencer m_sequencer;
        volatile ServiceInterface m_service;

        void start()
        {
            m_sequencer.step(6);
            m_service.doService();
        }

        void stop()
        {
            m_sequencer.step(8);
        }
    }
    
    public static class ServiceProvider implements ServiceInterface
    {
        Sequencer m_sequencer;
        ServiceProvider2 m_serviceProvider2;

        void bind(ServiceProvider2 provider2)
        {
            m_serviceProvider2 = provider2;
        }

        void start()
        {
            m_serviceProvider2.step(4);
            m_sequencer.step(5);
        }

        void stop()
        {
            m_sequencer.step(9);
        }

        void unbind(ServiceProvider2 provider2)
        {
            m_sequencer.step(10);
        }

        public void doService()
        {
            m_sequencer.step(7);
        }
    }

    public static class ServiceProvider2
    {
        Composite m_composite = new Composite();
        Sequencer m_sequencer;
        Runnable m_runnable;

        void bind(Sequencer seq)
        {
            m_sequencer = seq;
            m_sequencer.step(1);
        }

        void start()
        {
            m_sequencer.step(3);
            m_runnable.run(); // NullObject
        }

        public void step(int step) // called by ServiceProvider.start() method 
        { 
            m_sequencer.step(step);
        }
        
        void stop()
        {
            m_sequencer.step(11);
        }

        Object[] getComposition()
        {
            return new Object[] { this, m_composite };
        }
    }
    
    public static class Composite
    {
        void bind(Sequencer seq)
        {
            seq.step(2);
        }
    }
}
