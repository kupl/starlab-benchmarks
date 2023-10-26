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
package org.apache.felix.ipojo;

import java.util.ArrayList;
import java.util.Dictionary;
import java.util.List;

import org.apache.felix.ipojo.metadata.Element;
import org.osgi.framework.BundleContext;

/**
 * The handler manager manages an handler instance.
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
public class HandlerManager extends InstanceManager {

    /**
     * The internal handler object.
     * Immutable once set.
     */
    private Handler m_handler;

    /**
     * Creates a handler manager.
     * @param factory the handler factory
     * @param context the bundle context
     * @param handlers the handler array
     */
    public HandlerManager(ComponentFactory factory, BundleContext context, HandlerManager[] handlers) {
        super(factory, context, handlers);
    }

    /**
     * Gets the contained handler object.
     * If not already created it creates the object.
     * @return the handler object.
     */
    public Handler getHandler() {
        if (m_handler == null) {
            createHandlerObject();
        }
        return m_handler;
    }

    /**
     * Creates and initializes the handler object.
     * @param instance the component instance on which the handler will be attached.
     * @param metadata the component metadata.
     * @param configuration the instance configuration.
     * @throws ConfigurationException if the handler configuration failed.
     */
    public void init(ComponentInstance instance, Element metadata, Dictionary configuration) throws ConfigurationException {
        createHandlerObject();
        m_handler.setFactory(instance.getFactory());
        m_handler.attach(instance);
        m_handler.configure(metadata, configuration);
    }

    /**
     * Creates the handler object.
     * This method does nothing if the object is already created.
     * This method does not need locking protocol as only one thread (the creator thread) can create an instance.
     */
    private void createHandlerObject() {
        if (m_handler != null) { return; }
        m_handler = (Handler) createPojoObject();
    }

    /**
     * Creates an instance of the content.
     * This method needs to be called once only for singleton provided service.
     * This methods call the {@link InstanceManager#createObject()} method, and adds
     * the created object to the {@link InstanceManager#m_pojoObjects} list. Then,
     * it calls the {@link PrimitiveHandler#onCreation(Object)} methods on attached
     * handlers.
     * @return a new instance or <code>null</code> if an error occurs during the
     * creation.
     */
    public Object createPojoObject() {
        Object instance = createObject();

        // Add the new instance in the instance list.
        synchronized (this) {
            if (m_pojoObjects == null) {
                m_pojoObjects = new ArrayList(1);
            }
            m_pojoObjects.add(instance);
        }

        //Do not call onCreation, this will be done in the start method.

        return instance;
    }

    /**
     * Starts the instance manager.
     */
    public void start() {
        synchronized (this) {
            if (m_state != STOPPED) {
                return; // Instance already started
            } else {
                m_state = -2; // Temporary starting state, avoiding concurrent starts.
            }
        }

        // Start attached handler.
        for (int i = 0; i < m_handlers.length; i++) {
            m_handlers[i].addInstanceStateListener(this);
            m_handlers[i].start();
        }

        // Call the onCreation method.
        for (int i = 0; i < m_handlers.length; i++) {
            ((PrimitiveHandler) m_handlers[i].getHandler()).onCreation(m_handler);
        }


        m_handler.start(); // Call the handler start method, the instance might be invalid.


        for (int i = 0; i < m_handlers.length; i++) {
            if (!m_handlers[i].getHandler().isValid()) {
                setState(INVALID);
                return;
            }
        }
        if (m_handler.getValidity()) {
            setState(VALID);
        } else {
            setState(INVALID);
        }

        // Now, the state is necessary different from the temporary state.
    }

    /**
     * Stops the instance manager.
     */
    public void stop() {
        synchronized (this) {
            if (m_state == STOPPED) {
                return; // Instance already stopped
            } else {
                m_state = -2; // Temporary state avoiding concurrent stopping.
            }
        }

        setState(INVALID);

        if (m_handler != null) {
            m_handler.stop();
        }

        // Stop all the handlers
        for (int i = m_handlers.length - 1; i > -1; i--) {
            m_handlers[i].removeInstanceStateListener(this);
            m_handlers[i].stop();
        }

        List listeners = null;
        synchronized (this) {
            m_state = STOPPED;
            if (m_listeners != null) {
                listeners = new ArrayList(m_listeners); // Stack confinement.
            }
        }

        if (listeners != null) {
            for (int i = 0; i < listeners.size(); i++) {
                ((InstanceStateListener) listeners.get(i)).stateChanged(this, STOPPED);
            }
        }
    }

    /**
     * Disposes the instance.
     * @see org.apache.felix.ipojo.ComponentInstance#dispose()
     */
    public void dispose() {
        super.dispose();
        m_handler = null;
    }

    /**
     * Kills the current instance.
     * Only the factory of this instance can call this method.
     */
    protected void kill() {
        super.dispose();
        m_handler = null;
    }

    /**
     * State Change listener callback.
     * This method is notified at each time a plugged handler becomes invalid.
     * @param instance the changing instance
     * @param newState the new state
     * @see org.apache.felix.ipojo.InstanceStateListener#stateChanged(org.apache.felix.ipojo.ComponentInstance, int)
     */
    public void stateChanged(ComponentInstance instance, int newState) {
        int state;
        synchronized (this) {
            if (m_state <= STOPPED) {
                return;
            } else {
                state = m_state; // Stack confinement
            }
        }
        // Update the component state if necessary
        if (newState == INVALID && state == VALID) {
            // Need to update the state to UNRESOLVED
            setState(INVALID);
            return;
        }
        if (newState == VALID && state == INVALID) {
            // An handler becomes valid => check if all handlers are valid
            if (!m_handler.getValidity()) { return; }
            for (int i = 0; i < m_handlers.length; i++) {
                if (m_handlers[i].getState() != VALID) { return; }
            }
            setState(VALID);
            return;
        }
    }

}
