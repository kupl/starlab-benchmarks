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
package org.apache.felix.prefs.impl;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.apache.felix.prefs.BackingStore;
import org.apache.felix.prefs.BackingStoreManager;
import org.apache.felix.prefs.PreferencesImpl;
import org.osgi.framework.Bundle;
import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;
import org.osgi.framework.BundleEvent;
import org.osgi.framework.BundleListener;
import org.osgi.framework.ServiceFactory;
import org.osgi.framework.ServiceRegistration;
import org.osgi.service.prefs.BackingStoreException;
import org.osgi.service.prefs.PreferencesService;
import org.osgi.util.tracker.ServiceTracker;

/**
 * This activator registers itself as a service factory for the
 * preferences service.
 */
public class PreferencesManager
    implements BundleActivator,
    BundleListener,
    ServiceFactory<PreferencesService>,
    BackingStoreManager {

    /**
     * The map of already created services. For each client bundle
     * a new service is created.
     */
    protected final Map<Long, PreferencesServiceImpl> services = new HashMap<Long, PreferencesServiceImpl>();

    /** The bundle context. */
    protected BundleContext context;

    /** The backing store service tracker. */
    protected ServiceTracker<BackingStore, BackingStore> storeTracker;

    /** The default store which is used if no service can be found. */
    protected BackingStore defaultStore;

    /** Tracking count for the store tracker to detect changes. */
    protected int storeTrackingCount = -1;

    /**
     * @see org.osgi.framework.BundleListener#bundleChanged(org.osgi.framework.BundleEvent)
     */
    public void bundleChanged(final BundleEvent event) {
        if (event.getType() == BundleEvent.UNINSTALLED) {
            final Long bundleId = new Long(event.getBundle().getBundleId());
            synchronized (this.services) {
                try {
                    final BackingStore store = this.getStore();
                    // Only in the case we're already being stopped, this situation could occur, see FELIX-3334
                    if (store != null) {
                        store.remove(bundleId);
                    }
                }
                catch (final BackingStoreException ignore) {
                    // we ignore this for now
                }
                this.services.remove(bundleId);
            }
        }
    }

    /**
     * @see org.osgi.framework.BundleActivator#start(org.osgi.framework.BundleContext)
     */
    public void start(final BundleContext context) throws Exception {
        this.context = context;

        // create the tracker for our backing store
        this.storeTracker = new ServiceTracker<BackingStore, BackingStore>(context, BackingStore.class, null);
        this.storeTracker.open();

        // register this activator as a bundle lister
        context.addBundleListener(this);

        // finally register the service factory for the preferences service
        context.registerService(PreferencesService.class.getName(), this, null);
    }

    /**
     * @see org.osgi.framework.BundleActivator#stop(org.osgi.framework.BundleContext)
     */
    public void stop(final BundleContext context) throws Exception {
        // if we get stopped, we should save all in memory representations
        synchronized (this.services) {
            final Iterator<PreferencesServiceImpl> i = this.services.values().iterator();
            while (i.hasNext()) {
                final PreferencesServiceImpl service = i.next();
                this.save(service);
            }
            this.services.clear();
        }
        // stop tracking store service
        if (this.storeTracker != null) {
            this.storeTracker.close();
            this.storeTracker = null;
        }
        this.defaultStore = null;


        this.context = null;
    }

    /**
     * @see org.osgi.framework.ServiceFactory#getService(org.osgi.framework.Bundle, org.osgi.framework.ServiceRegistration)
     */
    public PreferencesService getService(final Bundle bundle,
            final ServiceRegistration<PreferencesService> reg) {
        final Long bundleId = new Long(bundle.getBundleId());

        synchronized (this.services) {
            PreferencesServiceImpl service;
            // do we already have created a service for this bundle?
            service = this.services.get(bundleId);

            if (service == null) {
                // create a new service instance
                service = new PreferencesServiceImpl(bundleId, this);
                this.services.put(bundleId, service);
            }
            return service;
        }
    }

    /**
     * @see org.osgi.framework.ServiceFactory#ungetService(org.osgi.framework.Bundle, org.osgi.framework.ServiceRegistration, java.lang.Object)
     */
    public void ungetService(final Bundle bundle,
            final ServiceRegistration<PreferencesService> reg,
            final PreferencesService s) {
        final Long bundleId = new Long(bundle.getBundleId());
        // we save all the preferences
        synchronized (this.services) {
            final PreferencesServiceImpl service = this.services.get(bundleId);
            if (service != null) {
                this.save(service);
            }
        }
    }

    /**
     * Save all preferences for this service.
     *
     * @param service
     */
    protected void save(final PreferencesServiceImpl service) {
        final Iterator<PreferencesImpl> i = service.getAllPreferences().iterator();
        while (i.hasNext()) {
            final PreferencesImpl prefs = i.next();
            try {
                this.getStore().store(prefs);
            }
            catch (final BackingStoreException ignore) {
                // we ignore this
            }
        }
    }

    /**
     * @see org.apache.felix.prefs.BackingStoreManager#getStore()
     */
    public BackingStore getStore() throws BackingStoreException {
        BackingStore service = null;
        ServiceTracker<BackingStore, BackingStore> storeTracker = this.storeTracker;

        // Only possible if we're not stopped already...
        if (storeTracker != null) {
	        // has the service changed?
	        int currentCount = storeTracker.getTrackingCount();
	        service = storeTracker.getService();
	        if (service != null && this.storeTrackingCount < currentCount) {
	            this.storeTrackingCount = currentCount;
	            this.cleanupStore(service);
	        }
	        if (service == null) {
	            // no service available use default store
	            if (this.defaultStore == null) {
	                synchronized (this) {
	                    if (this.defaultStore == null) {
	                        this.defaultStore = new DataFileBackingStoreImpl(this.context);
	                        this.cleanupStore(this.defaultStore);
	                    }
	                }
	            }
	            service = this.defaultStore;
	        }
        }

        if (service == null) {
            // (still) no service available; cannot fulfill this request...
            throw new BackingStoreException("No backing store!");
        }

        return service;
    }

    /**
     * Clean up the store and remove preferences for deleted bundles.
     *
     * @param store
     */
    protected void cleanupStore(final BackingStore store) {
        // check which bundles are available
        final Long[] availableBundleIds = store.availableBundles();

        // now check the bundles, for which we have preferences, if they are still
        // in service and delete the preferences where the bundles are out of service.
        // we synchronize on services in order to get not disturbed by a bundle event
        synchronized (this.services) {
            for (int i = 0; i < availableBundleIds.length; i++) {
                final Long bundleId = availableBundleIds[i];
                final Bundle bundle = this.context.getBundle(bundleId.longValue());
                if (bundle == null || bundle.getState() == Bundle.UNINSTALLED) {
                    try {
                        store.remove(bundleId);
                    }
                    catch (final BackingStoreException ignore) {
                        // we ignore this for now
                    }
                }
            }
        }
    }
}
