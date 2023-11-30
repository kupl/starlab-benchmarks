/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.sling.feature.extension.apiregions.api;

import java.util.HashMap;
import java.util.Map;

import org.apache.sling.feature.ArtifactId;

/**
 * Describes an exported package.
 */
public class ApiExport implements Comparable<ApiExport> {

    private final String name;

    private volatile String toggle;

    private volatile ArtifactId previous;

    private final Map<String, String> properties = new HashMap<>();

    /**
     * Create a new export
     *
     * @param name Package name for the export
     */
    public ApiExport(final String name) {
        this.name = name;
    }

    /**
     * Get the package name
     *
     * @return The package name
     */
    public String getName() {
        return name;
    }

    /**
     * Get the optional toggle information
     *
     * @return The toggle info or {@code null}
     */
    public String getToggle() {
        return toggle;
    }

    /**
     * Set the toggle info.
     *
     * @param toggle The toggle info
     */
    public void setToggle(String toggle) {
        this.toggle = toggle;
    }

    /**
     * Get the previous version of this api
     *
     * @return The previous version or {@code null}
     */
    public ArtifactId getPrevious() {
        return previous;
    }

    /**
     * Set the previous version
     *
     * @param previous Previus version
     */
    public void setPrevious(ArtifactId previous) {
        this.previous = previous;
    }

    /**
     * Get additional properties
     *
     * @return Modifiable map of properties
     */
    public Map<String, String> getProperties() {
        return this.properties;
    }

    @Override
    public int compareTo(final ApiExport o) {
        return this.name.compareTo(o.name);
    }

    @Override
    public String toString() {
        return "ApiExport [name=" + name + ", toggle=" + toggle + ", previous=" + previous + ", properties="
                + properties + "]";
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((name == null) ? 0 : name.hashCode());
        result = prime * result + ((previous == null) ? 0 : previous.hashCode());
        result = prime * result + ((properties == null) ? 0 : properties.hashCode());
        result = prime * result + ((toggle == null) ? 0 : toggle.hashCode());
        return result;
    }

    @Override
public boolean equals(java.lang.Object obj) {
    if (this == obj) {
        return true;
    }
    if (obj == null) {
        return false;
    }
    if (getClass() != obj.getClass()) {
        return false;
    }
    org.apache.sling.feature.extension.apiregions.api.ApiExport other = ((org.apache.sling.feature.extension.apiregions.api.ApiExport) (obj));
    if (name == null) {
        if (other.name != null) {
            return false;
        }
    } else if (!name.equals(other.name)) {
        return false;
    }
    if (previous == null) {
        if (other.previous != null) {
            return false;
        }
    } else if (!previous.equals(other.previous)) {
        return false;
    }
    if (properties == null) {
        if (other.properties != null) {
            return false;
        }
    } else if (!properties.equals(other.properties)) {
        return false;
    }
    {
        if (!/* NPEX_NULL_EXP */
        toggle.equals(other.toggle)) {
            return false;
        }
    }
    return true;
}
}
