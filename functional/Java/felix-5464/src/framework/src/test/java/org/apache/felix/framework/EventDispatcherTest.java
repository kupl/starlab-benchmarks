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
package org.apache.felix.framework;

import java.util.*;

import junit.framework.TestCase;

import org.easymock.EasyMock;
import org.osgi.framework.Bundle;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.framework.ServiceEvent;
import org.osgi.framework.ServiceListener;
import org.osgi.framework.ServiceReference;
import org.osgi.framework.hooks.service.EventHook;

public class EventDispatcherTest extends TestCase
{
    public void testFireServiceEvent()
    {
        final Bundle b1 = getMockBundle();
        final Bundle b2 = getMockBundle();
        final Bundle b3 = getMockBundle();
        final Bundle b4 = getMockBundle();

        final Set calledHooks = new HashSet();
        final EventHook eh1 = new EventHook()
        {
            public void event(ServiceEvent event, Collection contexts)
            {
                calledHooks.add(this);
            }
        };
        final EventHook eh2 = new EventHook()
        {
            public void event(ServiceEvent event, Collection contexts)
            {
                calledHooks.add(this);
                for (Iterator it = contexts.iterator(); it.hasNext();)
                {
                    BundleContext bc = (BundleContext) it.next();
                    if (bc.getBundle() == b1)
                    {
                        it.remove();
                    }
                    if (bc.getBundle() == b2)
                    {
                        it.remove();
                    }
                }
            }
        };

        Logger logger = new Logger();
        ServiceRegistry registry = new ServiceRegistry(logger, null);
        registry.registerService(b4, new String [] {EventHook.class.getName()}, eh1, new Hashtable());
        registry.registerService(b4, new String [] {EventHook.class.getName()}, eh2, new Hashtable());

        // -- Set up event dispatcher
        EventDispatcher ed = new EventDispatcher(logger, registry);

        // -- Register some listeners
        final List fired = Collections.synchronizedList(new ArrayList());
        ServiceListener sl1 = new ServiceListener()
        {
            public void serviceChanged(ServiceEvent arg0)
            {
                fired.add(this);
            }
        };
        ed.addListener(b1.getBundleContext(), ServiceListener.class, sl1, null);

        ServiceListener sl2 = new ServiceListener()
        {
            public void serviceChanged(ServiceEvent arg0)
            {
                fired.add(this);
            }
        };
        ed.addListener(b2.getBundleContext(), ServiceListener.class, sl2, null);

        ServiceListener sl3 = new ServiceListener()
        {
            public void serviceChanged(ServiceEvent arg0)
            {
                fired.add(this);
            }
        };
        ed.addListener(b3.getBundleContext(), ServiceListener.class, sl3, null);

        // --- make the invocation
        ServiceReference sr = EasyMock.createNiceMock(ServiceReference.class);
        EasyMock.expect(sr.getProperty(Constants.OBJECTCLASS)).andReturn(new String[]
            {
                "java.lang.String"
            }).anyTimes();
        sr.isAssignableTo(b1, String.class.getName());
        EasyMock.expectLastCall().andReturn(Boolean.TRUE).anyTimes();
        sr.isAssignableTo(b2, String.class.getName());
        EasyMock.expectLastCall().andReturn(Boolean.TRUE).anyTimes();
        sr.isAssignableTo(b3, String.class.getName());
        EasyMock.expectLastCall().andReturn(Boolean.TRUE).anyTimes();
        EasyMock.replay(new Object[]
            {
                sr
            });

        ServiceEvent event = new ServiceEvent(ServiceEvent.REGISTERED, sr);

        assertEquals("Precondition failed", 0, fired.size());

        Felix framework = new Felix(new HashMap());

        ed.fireServiceEvent(event, null, framework);
        assertEquals(1, fired.size());
        assertSame(sl3, fired.iterator().next());

        assertEquals(2, calledHooks.size());
        assertTrue(calledHooks.contains(eh1));
        assertTrue(calledHooks.contains(eh2));
    }

    private Bundle getMockBundle()
    {
        BundleContext bc = EasyMock.createNiceMock(BundleContext.class);
        Bundle b = EasyMock.createNiceMock(Bundle.class);
        EasyMock.expect(b.getBundleContext()).andReturn(bc).anyTimes();
        b.getState();
        EasyMock.expectLastCall().andReturn(Integer.valueOf(Bundle.ACTIVE)).anyTimes();

        EasyMock.expect(bc.getBundle()).andReturn(b).anyTimes();

        EasyMock.replay(new Object[]
            {
                bc, b
            });

        return b;
    }
}
