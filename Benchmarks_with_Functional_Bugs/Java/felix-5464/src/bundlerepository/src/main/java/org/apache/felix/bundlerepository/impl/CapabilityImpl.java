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
package org.apache.felix.bundlerepository.impl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.felix.bundlerepository.Capability;
import org.apache.felix.bundlerepository.Property;

public class CapabilityImpl implements Capability
{
    private String m_name = null;
    private final Map<String, Object> m_attributes = new HashMap<String, Object>();
    private final Map<String, String> m_directives = new HashMap<String, String>();
    private final List<Property> m_propList = new ArrayList<Property>();

    public CapabilityImpl()
    {
    }

    public CapabilityImpl(String name)
    {
        setName(name);
    }

    public CapabilityImpl(String name, PropertyImpl[] properties)
    {
        setName(name);
        for (int i = 0; properties != null && i < properties.length; i++)
        {
            addProperty(properties[i]);
        }
    }

    public String getName()
    {
        return m_name;
    }

    public void setName(String name)
    {
        m_name = name.intern();
    }

    public Map<String, Object> getPropertiesAsMap()
    {
        return m_attributes;
    }

    public Property[] getProperties()
    {
        return m_propList.toArray(new Property[m_propList.size()]);
    }

    public void addProperty(Property prop)
    {
        // m_map.put(prop.getName().toLowerCase(), prop.getConvertedValue()); // TODO is toLowerCase() on the key the right thing to do?
        // However if we definitely need to re-enable the to-lowercasing, the Felix Util FilterImpl supports treating filters
        // case-insensitively
        m_attributes.put(prop.getName(), prop.getConvertedValue());
        m_propList.add(prop);
    }

    public void addProperty(String name, String value)
    {
        addProperty(name, null, value);
    }

    public void addProperty(String name, String type, String value)
    {
        addProperty(new PropertyImpl(name, type, value));
    }

    public String toString()
    {
        return m_name  + ":" + m_attributes.toString();
    }

    public void addDirective(String key, String value) {
        m_directives.put(key, value);
    }

    public Map<String, String> getDirectives() {
        return Collections.unmodifiableMap(m_directives);
    }
}