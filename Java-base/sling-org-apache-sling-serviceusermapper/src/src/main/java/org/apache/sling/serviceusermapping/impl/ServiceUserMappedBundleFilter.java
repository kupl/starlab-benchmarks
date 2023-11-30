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

import java.util.Collection;
import java.util.Iterator;
import java.util.Map;

import org.apache.sling.serviceusermapping.Mapping;
import org.apache.sling.serviceusermapping.ServiceUserMapped;
import org.osgi.framework.Bundle;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.framework.InvalidSyntaxException;
import org.osgi.framework.ServiceEvent;
import org.osgi.framework.ServiceReference;
import org.osgi.framework.hooks.service.EventListenerHook;
import org.osgi.framework.hooks.service.FindHook;
import org.osgi.framework.hooks.service.ListenerHook;
import org.osgi.service.component.annotations.Activate;
import org.osgi.service.component.annotations.Component;
import org.osgi.service.component.annotations.Reference;

/**
 * The <code>ServiceUserMappingBundleFilter</code> only allows the bundle for which the service mapping is available to see it.
 */
@Component(immediate=true, // framework gets/ungets hooks each time
           service = {EventListenerHook.class, FindHook.class} )
public class ServiceUserMappedBundleFilter implements EventListenerHook, FindHook {

    @Reference
    volatile ServiceUserMapperImpl mapper;

    volatile BundleContext context;

    @Activate
    void start(BundleContext context) {
        this.context = context;
    }

    @Override
    public void event(ServiceEvent serviceEvent, Map map) {

        ServiceReference serviceReference = serviceEvent.getServiceReference();
        if (isServiceMappingReference(serviceReference) && serviceEvent.getType() == ServiceEvent.REGISTERED) {
            Object serviceName = serviceReference.getProperty(Mapping.SERVICENAME);
            String subserviceName = getSubServiceName(serviceReference);
            Iterator<Map.Entry<BundleContext, Collection<ListenerHook.ListenerInfo>>> it = map.entrySet().iterator();
            while (it.hasNext()) {
                Map.Entry<BundleContext, Collection<ListenerHook.ListenerInfo>> entry = it.next();
                BundleContext ctx = entry.getKey();

                if (!ctx.getBundle().equals(context.getBundle()) && !isValid(ctx.getBundle(), serviceName, subserviceName)) {
                    boolean keep = false;
                    Iterator<ListenerHook.ListenerInfo> iter = entry.getValue().iterator();
                    while (iter.hasNext()) {
                        if (!matchDefault(ctx.getBundle(), serviceName, subserviceName, iter.next().getFilter(), serviceReference)) {
                            iter.remove();
                        }
                        else {
                            keep = true;
                        }
                    }
                    if (!keep) {
                        it.remove();
                    }
                }
            }
        }
    }

    @Override
    public void find(BundleContext bundleContext, String name, String filter, boolean allServices,
                     Collection references) {

        Iterator<ServiceReference> it = references.iterator();
        while (it.hasNext()) {
            ServiceReference serviceReference = it.next();
            if (isServiceMappingReference(serviceReference)) {
                Object serviceName = serviceReference.getProperty(Mapping.SERVICENAME);
                String subserviceName = getSubServiceName(serviceReference);
                if (!isValid(bundleContext.getBundle(), serviceName, subserviceName)) {
                    if (!matchDefault(bundleContext.getBundle(), serviceName, subserviceName, filter, serviceReference)) {
                        it.remove();
                    }
                }
            }
        }
    }

    private boolean isValid(Bundle bundle, Object serviceName, String subserviceName) {
        String bundleServiceName = mapper.getServiceName(bundle);
        if (bundleServiceName.equals(serviceName)) {
            Iterable<String> principals = mapper.getServicePrincipalNamesInternal(bundle, subserviceName);
            if (principals != null && principals.iterator().hasNext()) {
                return true;
            }
            String user = mapper.getServiceUserIDInternal(bundle, subserviceName);
            if (user != null && !user.isEmpty()) {
                return true;
            }
        }

        return false;
    }

    private boolean matchDefault(Bundle bundle, Object serviceName, String subserviceName, String filter, ServiceReference ref) {
         if ( mapper.getServiceName(context.getBundle()).equals(serviceName) ) {
             try {
                 if (filter != null && context.createFilter(filter).match(ref)) {
                     for (Mapping mapping : mapper.getActiveMappings()) {
                         if (mapping.getServiceName().equals(mapper.getServiceName(bundle))) {
                             return false;
                         }
                     }
                     Iterable<String> principals = mapper.getServicePrincipalNamesInternal(bundle, subserviceName);
                     if (principals != null && principals.iterator().hasNext()) {
                         return true;
                     }
                     String user = mapper.getServiceUserIDInternal(bundle, subserviceName);
                     if (user != null && !user.isEmpty()) {
                         return true;
                     }
                 }
             } catch (InvalidSyntaxException e) {
                 // this shouldn't happen as the framework will have validated the filter already
                 throw new RuntimeException(e);
             }
         }
         return false;
    }

    private static boolean isServiceMappingReference(ServiceReference serviceReference) {
        Object objectClass = serviceReference.getProperty(Constants.OBJECTCLASS);
        for (Object o :  (Object[]) objectClass) {
            if (ServiceUserMappedImpl.SERVICEUSERMAPPED.equals(o)) {
                return true;
            }
        }
        return false;
    }

    private static String getSubServiceName(ServiceReference ref) {
        Object subserviceName = ref.getProperty(ServiceUserMapped.SUBSERVICENAME);
        if (subserviceName != null && subserviceName instanceof String) {
            return (String) subserviceName;
        }
        return null;
    }
}
