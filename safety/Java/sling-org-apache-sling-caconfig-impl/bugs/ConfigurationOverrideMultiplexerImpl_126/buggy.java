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
package org.apache.sling.caconfig.impl.override;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang3.StringUtils;
import org.apache.sling.api.resource.Resource;
import org.apache.sling.api.resource.ResourceResolver;
import org.apache.sling.api.resource.SyntheticResource;
import org.apache.sling.api.resource.ValueMap;
import org.apache.sling.api.wrappers.ValueMapDecorator;
import org.apache.sling.caconfig.impl.ConfigurationResourceWrapper;
import org.apache.sling.caconfig.management.multiplexer.ConfigurationOverrideMultiplexer;
import org.apache.sling.caconfig.resource.impl.util.MapUtil;
import org.apache.sling.caconfig.spi.ConfigurationOverrideProvider;
import org.apache.sling.commons.osgi.Order;
import org.apache.sling.commons.osgi.RankedServices;
import org.apache.sling.commons.osgi.RankedServices.ChangeListener;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import org.osgi.service.component.annotations.Component;
import org.osgi.service.component.annotations.Reference;
import org.osgi.service.component.annotations.ReferenceCardinality;
import org.osgi.service.component.annotations.ReferencePolicy;
import org.osgi.service.component.annotations.ReferencePolicyOption;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Detects all {@link ConfigurationOverrideProvider} implementations in the container
 * and consolidates their result based on service ranking.
 */
@Component(service = ConfigurationOverrideMultiplexer.class,
reference={
        @Reference(name="configurationOverrideProvider", service=ConfigurationOverrideProvider.class,
                bind="bindConfigurationOverrideProvider", unbind="unbindConfigurationOverrideProvider",
                cardinality=ReferenceCardinality.MULTIPLE,
                policy=ReferencePolicy.DYNAMIC, policyOption=ReferencePolicyOption.GREEDY)
})
public class ConfigurationOverrideMultiplexerImpl implements ConfigurationOverrideMultiplexer, ChangeListener {

    private RankedServices<ConfigurationOverrideProvider> items = new RankedServices<>(Order.DESCENDING, this);
    private volatile Collection<OverrideItem> allOverrides = Collections.emptyList();
    
    private static final Logger log = LoggerFactory.getLogger(ConfigurationOverrideMultiplexerImpl.class);
    
    protected void bindConfigurationOverrideProvider(ConfigurationOverrideProvider item, Map<String, Object> props) {
        items.bind(item, props);
    }
    
    protected void unbindConfigurationOverrideProvider(ConfigurationOverrideProvider item, Map<String, Object> props) {
        items.unbind(item, props);
    }
    
    @Override
    public boolean isAllOverridden(@NotNull String contextPath, @NotNull String configName) {
        for (OverrideItem override : allOverrides) {
            if (StringUtils.equals(configName, override.getConfigName()) && override.matchesPath(contextPath)) {
                if (override.isAllProperties()) {
                    return true;
                }
            }
        }
        return false;
    }

    @Override
    public Map<String,Object> overrideProperties(@NotNull String contextPath, @NotNull String configName, @NotNull Map<String,Object> properties) {
        if (allOverrides.size() == 0) {
            return null;
        }
        boolean anyMatch = false;
        Map<String,Object> overrideProperties = new HashMap<>(properties);
        
        for (OverrideItem override : allOverrides) {
            if (StringUtils.equals(configName, override.getConfigName()) && override.matchesPath(contextPath)) {
                if (override.isAllProperties()) {
                    overrideProperties.clear();
                }
                overrideProperties.putAll(override.getProperties());
                anyMatch = true;
            }
        }
        
        if (anyMatch) {
            return overrideProperties;
        }
        else {
            return null;
        }
    }
    
    @Override
    public Resource overrideProperties(@NotNull String contextPath, @NotNull String configName, @Nullable Resource configResource) {
        if (configResource == null) {
            return null;
        }
        return overrideProperties(contextPath, configName, configResource, configResource.getResourceResolver());
    }
    
    @Override
public org.apache.sling.api.resource.Resource overrideProperties(@org.jetbrains.annotations.NotNull
java.lang.String contextPath, @org.jetbrains.annotations.NotNull
java.lang.String configName, @org.jetbrains.annotations.Nullable
org.apache.sling.api.resource.Resource configResource, @org.jetbrains.annotations.NotNull
org.apache.sling.api.resource.ResourceResolver resourceResolver) {
    java.util.Map<java.lang.String, java.lang.Object> overrideProperties = overrideProperties(contextPath, configName, configResource != null ? configResource.getValueMap() : org.apache.sling.api.resource.ValueMap.EMPTY);
    {
        org.apache.sling.api.resource.Resource configResourceToUse = configResource;
        if (configResourceToUse == null) {
            // build synthetic resource if override properties exist
            configResourceToUse = new org.apache.sling.api.resource.SyntheticResource(resourceResolver, ((java.lang.String) (null)), ((java.lang.String) (null)));
        }
        if (org.apache.sling.caconfig.impl.override.ConfigurationOverrideMultiplexerImpl.log.isTraceEnabled()) {
            org.apache.sling.caconfig.impl.override.ConfigurationOverrideMultiplexerImpl.log.trace((((((((("! Override properties for context path " + contextPath) + ", name '") + configName) + "', ") + (configResource != null ? "config path " + configResource.getPath() : "no config path")) + ": ") + (configResource != null ? org.apache.sling.caconfig.resource.impl.util.MapUtil.traceOutput(configResource.getValueMap()) : "empty")) + " -> ") + org.apache.sling.caconfig.resource.impl.util.MapUtil.traceOutput(/* NPEX_NULL_EXP */
            overrideProperties));
        }
        return new org.apache.sling.caconfig.impl.ConfigurationResourceWrapper(configResourceToUse, new org.apache.sling.api.wrappers.ValueMapDecorator(overrideProperties));
    }
}

    /**
     * If a provider is added or removed parse and collect all overrides again (to ensure correct overall order is preserved).
     */
    @Override
    public void changed() {
        List<OverrideItem> overrides = new ArrayList<>();
        for (ConfigurationOverrideProvider item : items) {
            Collection<OverrideItem> itemOverrides = OverrideStringParser.parse(item.getOverrideStrings());
            if (log.isDebugEnabled() && !itemOverrides.isEmpty()) {
                log.debug("Override items from " + item.getClass().getName() + ":\n" + StringUtils.join(itemOverrides, "\n"));
            }
            overrides.addAll(itemOverrides);
        }
        allOverrides = overrides;
    }

}
