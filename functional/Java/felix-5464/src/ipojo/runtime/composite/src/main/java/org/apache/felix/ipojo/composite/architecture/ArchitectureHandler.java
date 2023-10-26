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
package org.apache.felix.ipojo.composite.architecture;

import java.util.Dictionary;
import java.util.Hashtable;

import org.apache.felix.ipojo.ComponentInstance;
import org.apache.felix.ipojo.Factory;
import org.apache.felix.ipojo.InstanceStateListener;
import org.apache.felix.ipojo.architecture.Architecture;
import org.apache.felix.ipojo.architecture.InstanceDescription;
import org.apache.felix.ipojo.composite.CompositeHandler;
import org.apache.felix.ipojo.metadata.Element;
import org.osgi.framework.ServiceRegistration;

/**
 * Composite Architecture Handler.
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
public class ArchitectureHandler extends CompositeHandler implements Architecture, InstanceStateListener {

    /**
     * Name of the component.
     */
    private String m_name;

    /**
     * The Architecture service registration.
     */
    private ServiceRegistration m_serviceRegistration;

    /**
     * Configure the handler.
     *
     * @param metadata      : the metadata of the component
     * @param configuration : the instance configuration
     * @see org.apache.felix.ipojo.Handler#configure(org.apache.felix.ipojo.metadata.Element, java.util.Dictionary)
     */
    public void configure(Element metadata, Dictionary configuration) {
        m_name = (String) configuration.get(Factory.INSTANCE_NAME_PROPERTY);
        Dictionary<String, String> dict = new Hashtable<String, String>();
        dict.put(ARCHITECTURE_INSTANCE, m_name);

        debug("Registering architecture service for " + m_name);
        m_serviceRegistration = getCompositeManager().getContext().registerService(Architecture.class.getName(), this, dict);

        // We can't use the regular handler stateChanged method as this method is not called when the instance is
        // disposed. This handler stays actives until the instance disposal.
        getCompositeManager().addInstanceStateListener(this);
    }

    /**
     * Stop method.
     *
     * @see org.apache.felix.ipojo.Handler#stop()
     */
    public void stop() {
        // Nothing do do when stopping.
    }

    /**
     * Start method.
     *
     * @see org.apache.felix.ipojo.Handler#start()
     */
    public void start() {
        // Nothing to do.
    }

    /**
     * The instance lifecycle listener callback.
     * When we receive the DISPOSED state, the architecture is unregistered from the service registry.
     * @param instance the changing instance the instance, meaningless in our case.
     * @param newState the new instance state the new instance state.
     */
    public void stateChanged(ComponentInstance instance, int newState) {
        if (newState == ComponentInstance.DISPOSED && m_serviceRegistration != null) {
            debug("Withdrawing the architecture service of " + m_name + " due to instance disposal");
            m_serviceRegistration.unregister();
            m_serviceRegistration = null;
        }
    }

    /**
     * Get the instance description.
     * @return the instance description
     * @see org.apache.felix.ipojo.architecture.Architecture#getInstanceDescription() ()
     */
    public InstanceDescription getInstanceDescription() {
        return getCompositeManager().getInstanceDescription();
    }

}
