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
package org.apache.felix.dm.runtime;

import java.lang.reflect.Method;
import java.util.AbstractSet;
import java.util.Dictionary;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.felix.dm.Component;
import org.apache.felix.dm.Dependency;
import org.apache.felix.dm.DependencyManager;
import org.osgi.framework.Bundle;

/**
 * This class implements a <code>java.util.Set</code> which acts as a service factory.
 * When a <code>Service</annotation> contains a <code>factory</code> attribute, this class is provided
 * into the OSGi registry with a <code>dm.factory.name</code> service property. So, another factory component
 * may be injected with this Set. And each time a Dictionary configuration is registered in the Set,
 * then a new Service instance will be instantiated, and will be provided with the Dictionary passed to the
 * Service instance.
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@SuppressWarnings( { "unchecked", "rawtypes"})
public class FactorySet extends AbstractSet<Dictionary>
{
    /**
     * When a Dictionary is registered in a factory Set, we use this special 
     * property key, whose value may provide the instance to use when
     * creating a service.
     */
    private final static String DM_FACTORY_INSTANCE = "dm.factory.instance";
    
    /**
     * The actual Service instance that is allocated for each dictionaries added in this Set.
     */
    private volatile Object m_impl;

    /**
     * The Service provided in the OSGi registry.
     */
    private final String[] m_provide;

    /**
     * The properties to be provided by the Service.
     */
    private final Dictionary m_serviceProperties;

    /**
     * The configure Service callback used to pass configuration added in this Set.
     */
    private final String m_configure;

    /**
     * The map between our Dictionaries and corresponding Service instances. 
     * This map is modified from our serial executor, or from the add method.
     */
    private final ConcurrentHashMap<ServiceKey, Object> m_services = new ConcurrentHashMap<ServiceKey, Object>();

    /**
     * The list of Dependencies which are applied in the Service.
     */
    private final MetaData m_srvMeta;

    /**
     * The list of Dependencies which are applied in the Service.
     */
    private final List<MetaData> m_depsMeta;

    /**
     * The DependencyManager which is used to create Service instances.
     */
    private volatile DependencyManager m_dm;

    /**
     * This class is used to serialize concurrent method calls, and allow to leave our methods unsynchronized.
     * This is required because some of our methods may invoke some Service callbacks, which must be called outside 
     * synchronized block (in order to avoid potential dead locks).
     */
    private final SerialExecutor m_serialExecutor = new SerialExecutor();

    /**
     * Flag used to check if our service is Active.
     */
    private volatile boolean m_active;

    /**
     * The bundle containing the Service annotated with the factory attribute.
     */
    private final Bundle m_bundle;

    /**
     * Flag used to check if a service is being created
     */
    private final static Object SERVICE_CREATING = new Object();

    /**
     * This class wraps <tt>Dictionary</tt>, allowing to store the dictionary into a Map, using
     * reference-equality in place of object-equality when getting the Dictionary from the Map.
     */
    private static class ServiceKey
    {
        private Dictionary m_dictionary;

        public ServiceKey(Dictionary dictionary)
        {
            m_dictionary = dictionary;
        }

        Dictionary getDictionary()
        {
            return m_dictionary;
        }

        @Override
        public boolean equals(Object that)
        {
            return that instanceof ServiceKey ? (((ServiceKey) that).getDictionary() == m_dictionary)
                : false;
        }

        @Override
        public int hashCode()
        {
            return System.identityHashCode(m_dictionary);
        }

        @Override
        public String toString()
        {
            return Dictionary.class.getName() + "@" + System.identityHashCode(m_dictionary);
        }
    }

    /**
     * Sole constructor.
     * @param b the bundle containing the Service annotated with the factory attribute
     * @param impl The Service implementation class
     * @param serviceProperties The Service properties
     * @param provides The Services provided by this Service
     * @param factoryConfigure The configure callback invoked in order to pass configurations added in this Set.
     */
    public FactorySet(Bundle b, MetaData srvMeta, List<MetaData> depsMeta)
    {
        m_serviceProperties = srvMeta.getDictionary(Params.properties, null);
        m_provide = srvMeta.getStrings(Params.provides, null);
        m_configure = srvMeta.getString(Params.factoryConfigure, null);
        m_bundle = b;
        m_srvMeta = srvMeta;
        m_depsMeta = depsMeta;
    }

    /**
     * Our Service is starting. 
     */
    public void start(Component c)
    {
        m_active = true;
        m_dm = c.getDependencyManager();
    }

    /**
     * Our Service is stopping: we have to remove all Service instances that we have created.
     */
    public void stop()
    {
        try
        {
            clear();
        }
        finally
        {
            m_active = false;
        }
    }

    /**
     * Create or Update a Service.
     */
    @Override
    @SuppressWarnings({ "synthetic-access" })
    public boolean add(final Dictionary configuration)
    {
        // Check parameter validity
        if (configuration == null)
        {
            throw new NullPointerException("configuration parameter can't be null");
        }

        // Check if our service is running.
        checkServiceAvailable();

        // Check if service is being created
        ServiceKey serviceKey = new ServiceKey(configuration);
        boolean creating = m_services.putIfAbsent(serviceKey, SERVICE_CREATING) == null;
        
        // Create or Update the Service.
        m_serialExecutor.enqueue(new Runnable()
        {
            public void run()
            {
                doAdd(configuration);
            }
        });

        m_serialExecutor.execute();
        return creating;
    }

    /**
     * Another Service wants to remove an existing Service.
     * This method is not synchronized but uses a SerialExecutor for serializing concurrent method call.
     * (This avoid potential dead locks, especially when Service callback methods are invoked).
     */
    @Override
    @SuppressWarnings("synthetic-access")
    public boolean remove(final Object configuration)
    {
        // Check parameter validity.
        if (configuration == null)
        {
            throw new NullPointerException("configuration parameter can't be null");
        }
        if (!(configuration instanceof Dictionary))
        {
            throw new IllegalArgumentException("configuration must be an instance of a Dictionary");
        }

        // Check if our service is active.
        checkServiceAvailable();

        // Check if service is created (or creating)
        boolean found = m_services.containsKey(new ServiceKey((Dictionary) configuration));
        if (found)
        {
            // Create or Update the Service.
            m_serialExecutor.enqueue(new Runnable()
            {
                public void run()
                {
                    doRemove((Dictionary) configuration);
                }
            });
            m_serialExecutor.execute();
        }
        return found;
    }

    /**
     * Another Service wants to remove all existing Services.
     * This method is not synchronized but uses a SerialExecutor for serializing concurrent method call.
     * (This avoid potential dead locks, especially when Service callback methods are invoked).
     */
    @Override
    @SuppressWarnings("synthetic-access")
    public void clear()
    {
        if (!m_active)
        {
            return;
        }

        // Make sure add/update/clear events are handled in FIFO order (serially).
        m_serialExecutor.enqueue(new Runnable()
        {
            public void run()
            {
                doClear();
            }
        });
        m_serialExecutor.execute();
    }

    /**
     * returns the list of active Service instances configurations.
     */
    @Override
    public Iterator<Dictionary> iterator()
    {
        throw new UnsupportedOperationException(
            "iterator method is not supported by DependencyManager Set's service factories");
    }

    @Override
    public String toString()
    {
        return FactorySet.class.getName() + "(" + m_services.size() + " active instances)";
    }

    /**
     * Returns the number of active Service instances.
     */
    @Override
    public int size()
    {
        if (!m_active)
        {
            return 0;
        }
        return m_services.size();
    }

    /**
     * Checks if our Service is available (we are not stopped").
     */
    private void checkServiceAvailable()
    {
        if (!m_active)
        {
            throw new IllegalStateException("Service not available");
        }
    }

    /**
     * Add or create a new Service instance, given its configuration. This method is invoked by the
     * SerialExecutor, hence it's thread safe and we'll invoke Service's callbacks without being
     * synchronized (hence this will avoid potential dead locks).
     */
    private void doAdd(Dictionary configuration)
    {
        // Check if the service exists.
        ServiceKey serviceKey = new ServiceKey(configuration);
        Object service = m_services.get(serviceKey);

        if (service == null || service == SERVICE_CREATING)
        {
            try
            {
                // Create the Service / impl, unless it is explicitly provided from the
                // configuration (using the specific key "dm.factory.instance").
                Component s = m_dm.createComponent();
                Class<?> implClass = m_bundle.loadClass(m_srvMeta.getString(Params.impl));
                Object impl = configuration.get(DM_FACTORY_INSTANCE);
                if (impl == null)
                {
                    String factoryMethod = m_srvMeta.getString(Params.factoryMethod, null);
                    if (factoryMethod == null)
                    {
                        m_impl = implClass.newInstance();
                    }
                    else
                    {
                        Method m = implClass.getDeclaredMethod(factoryMethod);
                        m.setAccessible(true);
                        m_impl = m.invoke(null);
                    }
                }
                else
                {
                    m_impl = impl;
                }

                // Invoke "configure" callback
                if (m_configure != null)
                {
                    invokeConfigure(m_impl, m_configure, configuration);
                }

                // Create Service
                s.setImplementation(m_impl);
                if (m_provide != null)
                {
                     // Merge service properties with the configuration provided by the factory.
                    Dictionary serviceProperties = mergeSettings(m_serviceProperties, configuration);
                    s.setInterface(m_provide, serviceProperties);
                }

                s.setComposition(m_srvMeta.getString(Params.composition, null));
                ServiceLifecycleHandler lfcleHandler = new ServiceLifecycleHandler(s, m_bundle, m_dm,
                    m_srvMeta, m_depsMeta);
                // The dependencies will be plugged by our lifecycle handler.
                s.setCallbacks(lfcleHandler, "init", "start", "stop", "destroy");

                // Adds dependencies (except named dependencies, which are managed by the lifecycle handler).
                for (MetaData dependency: m_depsMeta)
                {
                    String name = dependency.getString(Params.name, null);
                    if (name == null)
                    {
                        DependencyBuilder depBuilder = new DependencyBuilder(dependency);
                        Log.instance().info("ServiceLifecycleHandler.init: adding dependency %s into service %s",
                                            dependency, m_srvMeta);
                        Dependency d = depBuilder.build(m_bundle, m_dm);
                        s.add(d);
                    }
                }

                // Register the Service instance, and keep track of it.
                Log.instance().info("ServiceFactory: created service %s", m_srvMeta);
                m_dm.add(s);
                m_services.put(serviceKey, s);
            }
            catch (Throwable t)
            {
                // Make sure the SERVICE_CREATING flag is also removed
                m_services.remove(serviceKey);
                Log.instance().error("ServiceFactory: could not instantiate service %s",
                                     t, m_srvMeta);
            }
        }
        else
        {
            // Reconfigure an already existing Service.
            if (m_configure != null)
            {
                Log.instance().info("ServiceFactory: updating service %s", m_impl);
                invokeConfigure(m_impl, m_configure, configuration);
            }

            // Update service properties
            if (m_provide != null)
            {
                Dictionary settings = mergeSettings(m_serviceProperties, configuration);
                ((Component) service).setServiceProperties(settings);
            }
        }
    }

    private void doRemove(Dictionary configuraton)
    {
        Log.instance().info("ServiceFactory: removing service %s", m_srvMeta);
        ServiceKey serviceKey = new ServiceKey(configuraton);
        Object service = m_services.remove(serviceKey);
        if (service != null && service != SERVICE_CREATING)
        {
            m_dm.remove((Component) service);
        }
    }

    private void doClear()
    {
        for (Object service : m_services.values())
        {
            if (service instanceof Component)
            {
                m_dm.remove((Component) service);
            }
        }
        m_services.clear();
    }

    /**
     * Merge factory configuration settings with the service properties. The private factory configuration 
     * settings are ignored. A factory configuration property is private if its name starts with a dot (".").
     * 
     * @param serviceProperties
     * @param factoryConfiguration
     * @return
     */
    private Dictionary mergeSettings(Dictionary serviceProperties, Dictionary factoryConfiguration)
    {
        Dictionary props = new Hashtable();

        if (serviceProperties != null)
        {
            Enumeration keys = serviceProperties.keys();
            while (keys.hasMoreElements())
            {
                Object key = keys.nextElement();
                Object val = serviceProperties.get(key);
                props.put(key, val);
            }
        }

        Enumeration keys = factoryConfiguration.keys();
        while (keys.hasMoreElements())
        {
            Object key = keys.nextElement();
            if (!key.toString().startsWith("."))
            {
                // public properties are propagated
                Object val = factoryConfiguration.get(key);
                props.put(key, val);
            }
        }
        return props;
    }

    /**
     * Invokes the configure callback method on the service instance implemenatation.
     * @param impl
     * @param configure
     * @param config
     */
    private void invokeConfigure(Object impl, String configure, Dictionary config)
    {
        try
        {
            InvocationUtil.invokeCallbackMethod(impl, configure,
                                                new Class[][] { { Dictionary.class } },
                                                new Object[][] { { config } });
        }

        catch (Throwable t)
        {
            if (t instanceof RuntimeException)
            {
                throw (RuntimeException) t;
            }
            else
            {
                throw new RuntimeException("Could not invoke method " + configure
                                           + " on object " + impl, t);
            }
        }
    }
}
