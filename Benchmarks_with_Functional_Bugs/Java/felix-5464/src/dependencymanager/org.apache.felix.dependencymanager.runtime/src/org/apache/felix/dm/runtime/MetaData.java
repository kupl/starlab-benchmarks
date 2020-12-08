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

import java.util.Dictionary;

/**
 * This class represents the meta data parsed from a descriptor entry (json) line.
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
public interface MetaData extends Cloneable
{
    /**
     * Returns a String descriptor entry parameter value.
     */
    String getString(Params key);

    /**
     * Returns a String descriptor entry parameter value.
     */
    String getString(Params key, String def);

    /**
     * Returns a String descriptor entry parameter value.
     */
    int getInt(Params key);

    /**
     * Returns a String descriptor entry parameter value.
     */
    int getInt(Params key, int def);
    
    /**
     * Returns a String descriptor entry parameter value.
     */
    long getLong(Params key);

    /**
     * Returns a String descriptor entry parameter value.
     */
    long getLong(Params key, long def);

    /**
     * Returns a String array descriptor entry parameter value.
     */
    String[] getStrings(Params key);

    /**
     * Returns a String array descriptor entry parameter value.
     */
    String[] getStrings(Params key, String[] def);

    /**
     * Returns a descriptor entry value which is a complex value.
     */
    Dictionary<String, Object> getDictionary(Params key, Dictionary<String, Object> def);
    
    /**
     * Modifies a key Sring value
     */
    void setString(Params key, String value);
    
    /**
     * Modifies a String[] value.
     */
    void setStrings(Params key, String[] values);
    
    /**
     * Modifies a String[] value.
     */
    void setDictionary(Params key, Dictionary<String, Object> dictionary);
    
    /**
     * Clone this MetaData object.
     */
    Object clone() throws CloneNotSupportedException;
}
