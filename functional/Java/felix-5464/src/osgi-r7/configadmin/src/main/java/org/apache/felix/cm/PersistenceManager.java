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
package org.apache.felix.cm;


import java.io.IOException;
import java.util.Dictionary;
import java.util.Enumeration;

import org.osgi.annotation.versioning.ConsumerType;


/**
 * The <code>PersistenceManager</code> interface defines the API to be
 * implemented to support persisting configuration data. This interface may
 * be implemented by bundles, which support storing configuration data in
 * different locations.
 * <p>
 * The Apache Felix Configuration Admin Service bundles provides an
 * implementation of this interface using the platform file system to store
 * configuration data.
 * <p>
 * Implementations of this interface must support loading and storing
 * <code>java.util.Dictionary</code> objects as defined in section 104.4.2,
 * Configuration Properties, of the Configuration Admin Service Specification
 * Version 1.2.
 * <p>
 * To make implementations of this interface available to the Configuration
 * Admin Service they must be registered as service for this interface. The
 * Configuration Admin Service will consider all registered services plus the
 * default platform file system based implementation to load configuration data.
 * To store new configuration data, the persistence manager service with the
 * highest rank value - the <code>service.ranking</code> service property - is
 * used. If no persistence manager service has been registered, the platform
 * file system based implementation is used.
 */
@ConsumerType
public interface PersistenceManager
{
    /**
     * Service registration property to define a unique name for the persistence manager.
     * @since 1.2
     */
    String PROPERTY_NAME = "name";

    /**
     * Returns <code>true</code> if a persisted <code>Dictionary</code> exists
     * for the given <code>pid</code>.
     *
     * @param pid The identifier for the dictionary to test.
     */
    boolean exists( String pid );


    /**
     * Returns the <code>Dictionary</code> for the given <code>pid</code>.
     * <p>
     * Implementations are expected to return dictionary instances which may be
     * modified by the caller without affecting any underlying data or affecting
     * future calls to this method with the same PID. In other words the
     * reference equation <code>load(pid) != load(pid)</code> must hold
     * <code>true</code>.
     *
     * @param pid The identifier for the dictionary to load.
     *
     * @return The dictionary for the identifier. This must not be
     *      <code>null</code> but may be empty.
     *
     * @throws IOException If an error occurs loading the dictionary. An
     *      <code>IOException</code> must also be thrown if no dictionary
     *      exists for the given identifier.
     */
    Dictionary load( String pid ) throws IOException;


    /**
     * Returns an enumeration of all <code>Dictionary</code> objects known to
     * this persistence manager.
     * <p>
     * Implementations of this method are allowed to return lazy enumerations.
     * That is, it is allowable for the enumeration to not return a dictionary
     * if loading it results in an error.
     * <p>
     * Implementations are expected to return dictionary instances which may be
     * modified by the caller without affecting any underlying data or affecting
     * future calls to this method.
     * <p>
     * The <code>Enumeration</code> returned from this method must be stable
     * against concurrent calls to either of the {@link #load(String)},
     * {@link #store(String, Dictionary)}, and {@link #delete(String)} methods.
     *
     * @return A possibly empty Enumeration of all dictionaries.
     *
     * @throws IOException If an error occurs getting the dictionaries.
     */
    Enumeration getDictionaries() throws IOException;


    /**
     * Stores the <code>Dictionary</code> under the given <code>pid</code>.
     * <p>
     * The dictionary provided to this method must be considered private to the
     * caller. Any modification by the caller after this method finishes must
     * not influence any internal storage of the provided data. Implementations
     * must not modify the dictionary.
     *
     * @param pid The identifier of the dictionary.
     * @param properties The <code>Dictionary</code> to store.
     *
     * @throws IOException If an error occurs storing the dictionary. If this
     *      exception is thrown, it is expected, that
     *      {@link #exists(String) exists(pid} returns <code>false</code>.
     */
    void store( String pid, Dictionary properties ) throws IOException;


    /**
     * Removes the <code>Dictionary</code> for the given <code>pid</code>. If
     * such a dictionary does not exist, this method has no effect.
     *
     * @param pid The identifier of the dictionary to delete.
     *
     * @throws IOException If an error occurs deleting the dictionary. This
     *      exception must not be thrown if no dictionary with the given
     *      identifier exists.
     */
    void delete( String pid ) throws IOException;

}