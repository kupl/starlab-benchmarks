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

package org.apache.sling.serviceusermapping.impl;


import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.sling.serviceusermapping.Mapping;
import org.junit.Test;
import org.osgi.framework.Bundle;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.framework.ServiceEvent;
import org.osgi.framework.ServiceReference;
import org.osgi.framework.hooks.service.ListenerHook;

import junit.framework.TestCase;

/**
 * Test reference and bundle filtering based on <code>Mapping.SERVICENAME</code>
 */
public class ServiceUserMappedBundleFilterTest {

    final static String BUNDLE1 = "bundle1";
    final static String BUNDLE2 = "bundle2";


    final static BundleContext bundleContext1;
    final static BundleContext bundleContext2;
    final static BundleContext mapperContext;

    static  {
        Bundle mapper = mock(Bundle.class);
        when(mapper.getSymbolicName()).thenReturn("mapper");
        mapperContext = mock(BundleContext.class);
        when(mapperContext.getBundle()).thenReturn(mapper);

        bundleContext1 = mock(BundleContext.class);
        Bundle bundle1 = mock(Bundle.class);
        when(bundleContext1.getBundle()).thenReturn(bundle1);
        when(bundle1.getSymbolicName()).thenReturn(BUNDLE1);



        bundleContext2 = mock(BundleContext.class);
        Bundle bundle2 = mock(Bundle.class);
        when(bundleContext2.getBundle()).thenReturn(bundle2);
        when(bundle2.getSymbolicName()).thenReturn(BUNDLE2);

    }




    @Test
    public void testEvent() {
        Map<BundleContext, Collection<ListenerHook.ListenerInfo>> map = new HashMap<BundleContext, Collection<ListenerHook.ListenerInfo>>();

        map.put(bundleContext1, new ArrayList<ListenerHook.ListenerInfo>());
        map.put(bundleContext2, new ArrayList<ListenerHook.ListenerInfo>());

        ServiceEvent serviceEvent = mock(ServiceEvent.class);
        ServiceReference serviceReference = mock(ServiceReference.class);
        when(serviceEvent.getServiceReference()).thenReturn(serviceReference);
        when(serviceReference.getProperty(Constants.OBJECTCLASS)).thenReturn(new String[]{ServiceUserMappedImpl.SERVICEUSERMAPPED});
        when(serviceReference.getProperty(Mapping.SERVICENAME)).thenReturn(BUNDLE1);
        when(serviceEvent.getType()).thenReturn(ServiceEvent.REGISTERED);


        ServiceUserMappedBundleFilter eventListenerHook = new ServiceUserMappedBundleFilter();

        ServiceUserMapperImpl.Config config = mock(ServiceUserMapperImpl.Config.class);
        eventListenerHook.mapper = new ServiceUserMapperImpl(null, config);
        eventListenerHook.start(mapperContext);

        Map<BundleContext, Collection<ListenerHook.ListenerInfo>> map1 = new HashMap<>(map);
        eventListenerHook.event(serviceEvent, map1);

        TestCase.assertEquals(0, map1.size());

        when(config.user_enable_default_mapping()).thenReturn(true);
        eventListenerHook.mapper.configure(config);
        Map<BundleContext, Collection<ListenerHook.ListenerInfo>> map2 = new HashMap<>(map);
        eventListenerHook.event(serviceEvent, map2);

        TestCase.assertEquals(1, map2.size());
        TestCase.assertTrue(map.containsKey(bundleContext1));

    }

    @Test
    public void testFind() {
        List<ServiceReference> collection = new ArrayList<>();

        ServiceReference serviceReference1 = mock(ServiceReference.class);
        ServiceReference serviceReference2 = mock(ServiceReference.class);
        collection.add(serviceReference1);
        collection.add(serviceReference2);

        when(serviceReference1.getProperty(Mapping.SERVICENAME)).thenReturn(BUNDLE1);
        when(serviceReference1.getProperty(Constants.OBJECTCLASS)).thenReturn(new String[]{ServiceUserMappedImpl.SERVICEUSERMAPPED});

        when(serviceReference2.getProperty(Mapping.SERVICENAME)).thenReturn(BUNDLE2);
        when(serviceReference2.getProperty(Constants.OBJECTCLASS)).thenReturn(new String[]{ServiceUserMappedImpl.SERVICEUSERMAPPED});

        ServiceUserMappedBundleFilter findHook = new ServiceUserMappedBundleFilter();

        ServiceUserMapperImpl.Config config = mock(ServiceUserMapperImpl.Config.class);
        findHook.mapper = new ServiceUserMapperImpl(null, config);
        findHook.start(mapperContext);

        List<ServiceReference> collection1 = new ArrayList<>(collection);
        findHook.find(bundleContext1, null, null, false, collection1);

        TestCase.assertEquals(0, collection1.size());

        when(config.user_enable_default_mapping()).thenReturn(true);
        findHook.mapper.configure(config);

        List<ServiceReference> collection2 = new ArrayList<>(collection);
        findHook.find(bundleContext1, null, null, false, collection2);

        TestCase.assertEquals(1, collection2.size());
        TestCase.assertTrue(collection.contains(serviceReference1));


    }
}
