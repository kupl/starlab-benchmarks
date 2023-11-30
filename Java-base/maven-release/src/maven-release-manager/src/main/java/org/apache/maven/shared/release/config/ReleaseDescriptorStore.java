package org.apache.maven.shared.release.config;

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

/**
 * Storage for reading and writing release configuration.
 *
 * @author <a href="mailto:brett@apache.org">Brett Porter</a>
 */
public interface ReleaseDescriptorStore
{
    /**
     * Read a configuration.
     *
     * @param mergeDescriptor configuration to merge with the loaded configuration. Some values are used as defaults,
     *                        while others are used to override
     * @return the configuration
     */
    ReleaseDescriptorBuilder read( ReleaseDescriptorBuilder mergeDescriptor )
        throws ReleaseDescriptorStoreException;

    /**
     * Save a configuration.
     *
     * @param config the configuration
     */
    void write( ReleaseDescriptor config )
        throws ReleaseDescriptorStoreException;

    /**
     * Remove a configuration.
     *
     * @param config the location of the configuration
     */
    void delete( ReleaseDescriptor config );
}
