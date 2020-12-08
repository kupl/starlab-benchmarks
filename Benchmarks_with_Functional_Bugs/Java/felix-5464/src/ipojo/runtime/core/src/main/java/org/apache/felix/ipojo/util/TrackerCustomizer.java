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
package org.apache.felix.ipojo.util;

import org.osgi.framework.ServiceReference;

/**
 * Tracker Customizer.
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
public interface TrackerCustomizer {

    /**
     * A service is being added to the Tracker object.
     * This method is called before a service which matched the search parameters of the Tracker object is added to
     * it. This method should return the service object to be tracked for this ServiceReference object.
     * If this method returns {@code true}, the service object becomes available from the {@link Tracker#getService()} and
     * {@link Tracker#getServices()} methods. However notice that the service is still not accessible at that time.
     * @param reference the Reference to service being added to the Tracker object.
     * @return {@code true} if the service reference must be tracked. {@code false} ff the service reference must be
     * ignored (un-tracked)
     */
    boolean addingService(ServiceReference reference);
    
    /**
     * A service tracked by the Tracker object has been added in the list.
     * This method is called when a service has been added in the managed list (after addingService) and if the
     * service has not disappeared before during the callback.
     * In this method, the service object is accessible from the {@link Tracker#getService()} and
     * {@link Tracker#getServices()} method.
     * @param reference the added reference.
     */
    void addedService(ServiceReference reference);

    /**
     * A service tracked by the Tracker object has been modified.
     * This method is called when a service tracked by the Tracker has its properties modified.
     * @param reference the reference to service that has been modified.
     * @param service The service object for the modified service.
     */
    void modifiedService(ServiceReference reference, Object service);

    /**
     * A service tracked by the Tracker object has been removed.
     * This method is called when a tracked service is no longer being tracked by the Tracker object.
     * Notice that some OSGi implementations does not support getting the service object from the bundle context in
     * this method. The cached (given) object must be used.
     * @param reference the reference to service that has been removed.
     * @param service The service object for the removed service.
     */
    void removedService(ServiceReference reference, Object service);

}
