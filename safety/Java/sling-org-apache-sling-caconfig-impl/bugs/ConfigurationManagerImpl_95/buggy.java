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
package org.apache.sling.caconfig.management.impl;

import static org.apache.sling.caconfig.impl.ConfigurationNameConstants.CONFIGS_BUCKET_NAME;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.SortedSet;

import org.apache.commons.collections4.IteratorUtils;
import org.apache.commons.collections4.ResettableIterator;
import org.apache.commons.collections4.Transformer;
import org.apache.commons.collections4.iterators.ListIteratorWrapper;
import org.apache.commons.lang3.StringUtils;
import org.apache.sling.api.resource.Resource;
import org.apache.sling.api.resource.ResourceResolver;
import org.apache.sling.caconfig.management.ConfigurationCollectionData;
import org.apache.sling.caconfig.management.ConfigurationData;
import org.apache.sling.caconfig.management.ConfigurationManagementSettings;
import org.apache.sling.caconfig.management.ConfigurationManager;
import org.apache.sling.caconfig.management.ConfigurationResourceResolverConfig;
import org.apache.sling.caconfig.management.multiplexer.ConfigurationInheritanceStrategyMultiplexer;
import org.apache.sling.caconfig.management.multiplexer.ConfigurationMetadataProviderMultiplexer;
import org.apache.sling.caconfig.management.multiplexer.ConfigurationOverrideMultiplexer;
import org.apache.sling.caconfig.management.multiplexer.ConfigurationPersistenceStrategyMultiplexer;
import org.apache.sling.caconfig.management.multiplexer.ConfigurationResourceResolvingStrategyMultiplexer;
import org.apache.sling.caconfig.resource.impl.util.ConfigNameUtil;
import org.apache.sling.caconfig.resource.impl.util.MapUtil;
import org.apache.sling.caconfig.spi.ConfigurationCollectionPersistData;
import org.apache.sling.caconfig.spi.ConfigurationPersistData;
import org.apache.sling.caconfig.spi.ConfigurationPersistenceException;
import org.apache.sling.caconfig.spi.metadata.ConfigurationMetadata;
import org.apache.sling.caconfig.spi.metadata.PropertyMetadata;
import org.jetbrains.annotations.NotNull;
import org.osgi.service.component.annotations.Component;
import org.osgi.service.component.annotations.Reference;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Component(service = ConfigurationManager.class)
public class ConfigurationManagerImpl implements ConfigurationManager {
    
    @Reference
    private ConfigurationResourceResolvingStrategyMultiplexer configurationResourceResolvingStrategy;
    @Reference
    private ConfigurationMetadataProviderMultiplexer configurationMetadataProvider;
    @Reference
    private ConfigurationPersistenceStrategyMultiplexer configurationPersistenceStrategy;
    @Reference
    private ConfigurationInheritanceStrategyMultiplexer configurationInheritanceStrategy;
    @Reference
    private ConfigurationOverrideMultiplexer configurationOverrideMultiplexer;
    @Reference
    private ConfigurationResourceResolverConfig configurationResourceResolverConfig;
    @Reference
    private ConfigurationManagementSettings configurationManagementSettings;

    private static final Logger log = LoggerFactory.getLogger(ConfigurationManagerImpl.class);
    
    @SuppressWarnings("unchecked")
    @Override
public org.apache.sling.caconfig.management.ConfigurationData getConfiguration(@org.jetbrains.annotations.NotNull
org.apache.sling.api.resource.Resource resource, @org.jetbrains.annotations.NotNull
java.lang.String configName) {
    org.apache.sling.caconfig.resource.impl.util.ConfigNameUtil.ensureValidConfigName(configName);
    if (org.apache.sling.caconfig.management.impl.ConfigurationManagerImpl.log.isDebugEnabled()) {
        org.apache.sling.caconfig.management.impl.ConfigurationManagerImpl.log.debug("Get configuration for context path {}, name '{}'", resource.getPath(), configName);
    }
    org.apache.sling.caconfig.spi.metadata.ConfigurationMetadata configMetadata = getConfigurationMetadata(configName);
    org.apache.sling.api.resource.Resource configResource = null;
    java.util.Iterator<org.apache.sling.api.resource.Resource> configResourceInheritanceChain = configurationResourceResolvingStrategy.getResourceInheritanceChain(resource, configurationResourceResolverConfig.configBucketNames(), configName);
    {
        org.apache.commons.collections4.ResettableIterator resettableConfigResourceInheritanceChain = new org.apache.commons.collections4.iterators.ListIteratorWrapper(/* NPEX_NULL_EXP */
        configResourceInheritanceChain);
        configResource = applyPersistenceAndInheritance(resource.getPath(), configName, resettableConfigResourceInheritanceChain, false, resource.getResourceResolver());
        if (configResource != null) {
            // get writeback resource for "reverse inheritance detection"
            org.apache.sling.api.resource.Resource writebackConfigResource = null;
            java.lang.String writebackConfigResourcePath = null;
            for (java.lang.String configBucketName : configurationResourceResolverConfig.configBucketNames()) {
                writebackConfigResourcePath = configurationResourceResolvingStrategy.getResourcePath(resource, configBucketName, configName);
                if (writebackConfigResourcePath != null) {
                    writebackConfigResource = resource.getResourceResolver().getResource(writebackConfigResourcePath);
                    if (writebackConfigResource != null) {
                        writebackConfigResource = configurationPersistenceStrategy.getResource(writebackConfigResource);
                        break;
                    }
                }
            }
            if (org.apache.sling.caconfig.management.impl.ConfigurationManagerImpl.log.isTraceEnabled()) {
                org.apache.sling.caconfig.management.impl.ConfigurationManagerImpl.log.trace(((((((("+ Found config resource for context path " + resource.getPath()) + ": ") + configResource.getPath()) + " ") + org.apache.sling.caconfig.resource.impl.util.MapUtil.traceOutput(configResource.getValueMap())) + ", ") + "writeback config resource: ") + writebackConfigResourcePath);
            }
            resettableConfigResourceInheritanceChain.reset();
            return new org.apache.sling.caconfig.management.impl.ConfigurationDataImpl(configMetadata, configResource, writebackConfigResource, applyPersistence(resettableConfigResourceInheritanceChain, false), resource, configName, this, configurationManagementSettings, configurationOverrideMultiplexer, configurationPersistenceStrategy, false, null);
        }
    }
    if (configMetadata != null) {
        // if no config resource found still check for overrides
        configResource = configurationOverrideMultiplexer.overrideProperties(resource.getPath(), configName, ((org.apache.sling.api.resource.Resource) (null)), resource.getResourceResolver());
        if (configResource != null) {
            return new org.apache.sling.caconfig.management.impl.ConfigurationDataImpl(configMetadata, configResource, null, null, resource, configName, this, configurationManagementSettings, configurationOverrideMultiplexer, configurationPersistenceStrategy, false, null);
        }
        // if no config resource found but config metadata exist return empty config data with default values
        return new org.apache.sling.caconfig.management.impl.ConfigurationDataImpl(configMetadata, resource, configName, this, configurationManagementSettings, configurationOverrideMultiplexer, configurationPersistenceStrategy, false);
    }
    return null;
}

    @SuppressWarnings("unchecked")
    @Override
    public@NotNull  ConfigurationCollectionData getConfigurationCollection(@NotNull Resource resource, @NotNull String configName) {
        ConfigNameUtil.ensureValidConfigName(configName);
        if (log.isDebugEnabled()) {
            log.debug("Get configuration collection for context path {}, name '{}'", resource.getPath(), configName);
        }
        ConfigurationMetadata configMetadata = getConfigurationMetadata(configName);
        List<ConfigurationData> configData = new ArrayList<>();

        // get all possible colection parent config names
        Collection<String> collectionParentConfigNames = configurationPersistenceStrategy.getAllCollectionParentConfigNames(configName);

        // get configuration resource items
        List<Iterator<Resource>> configResourceInheritanceChains = new ArrayList<>();
        for (String collectionParentConfigName : collectionParentConfigNames) {
            Collection<Iterator<Resource>> result = configurationResourceResolvingStrategy
                    .getResourceCollectionInheritanceChain(resource, configurationResourceResolverConfig.configBucketNames(), collectionParentConfigName);
            if (result != null) {
                configResourceInheritanceChains.addAll(result);
            }
        }

        String writebackConfigResourceCollectionParentPath = null;
        for (Iterator<Resource> configResourceInheritanceChain : configResourceInheritanceChains) {
            ResettableIterator resettableConfigResourceInheritanceChain = new ListIteratorWrapper(configResourceInheritanceChain);
            Resource configResource = applyPersistenceAndInheritance(resource.getPath(), configName, resettableConfigResourceInheritanceChain, true, resource.getResourceResolver());
            resettableConfigResourceInheritanceChain.reset();
            Resource untransformedConfigResource = (Resource)resettableConfigResourceInheritanceChain.next();
            if (configResource != null) {
                // get writeback resource for "reverse inheritance detection"
                Resource writebackConfigResource = null;
                
                String writebackConfigResourcePath = null;
                for (String configBucketName : configurationResourceResolverConfig.configBucketNames()) {
                    writebackConfigResourceCollectionParentPath = configurationResourceResolvingStrategy.getResourceCollectionParentPath(resource, configBucketName, configName);
                    if (writebackConfigResourceCollectionParentPath != null) {
                        writebackConfigResourceCollectionParentPath = configurationPersistenceStrategy.getCollectionParentResourcePath(writebackConfigResourceCollectionParentPath);
                        writebackConfigResourcePath = writebackConfigResourceCollectionParentPath + "/" + untransformedConfigResource.getName();
                        writebackConfigResource = configResource.getResourceResolver().getResource(writebackConfigResourcePath);
                        if (writebackConfigResource != null) {
                            writebackConfigResource = configurationPersistenceStrategy.getCollectionItemResource(writebackConfigResource);
                            break;
                        }
                    }
                }
                
                if (log.isTraceEnabled()) {
                    log.trace("+ Found config resource for context path " + resource.getPath() + ": " + configResource.getPath() + " "
                            + MapUtil.traceOutput(configResource.getValueMap()) + ", "
                            + "writeback config resource: " + writebackConfigResourcePath);
                }
                resettableConfigResourceInheritanceChain.reset();
                configData.add(new ConfigurationDataImpl(configMetadata, configResource, writebackConfigResource,
                        applyPersistence(resettableConfigResourceInheritanceChain, true),
                        resource, configName, this, configurationManagementSettings,
                        configurationOverrideMultiplexer, configurationPersistenceStrategy,
                        true, untransformedConfigResource.getName()));
            }
        }
        // fallback for writeback path detection when no configuration resources does exist yet
        if (writebackConfigResourceCollectionParentPath == null) {
            for (String configBucketName : configurationResourceResolverConfig.configBucketNames()) {
                writebackConfigResourceCollectionParentPath = configurationResourceResolvingStrategy.getResourceCollectionParentPath(resource, configBucketName, configName);
                if (writebackConfigResourceCollectionParentPath != null) {
                    break;
                }
            }
        }
        
        // get properties of parent resource of the current level
        Map<String,Object> resourceCollectionParentProps = null;
        if (writebackConfigResourceCollectionParentPath != null) {
            Resource writebackConfigResourceCollectionParent = resource.getResourceResolver().getResource(writebackConfigResourceCollectionParentPath);
            if (writebackConfigResourceCollectionParent != null) {
                for (String resourceName : configurationManagementSettings.getConfigCollectionPropertiesResourceNames()) {
                    Resource propsResource = writebackConfigResourceCollectionParent.getChild(resourceName);
                    if (propsResource != null) {
                        resourceCollectionParentProps = propsResource.getValueMap();
                        break;
                    }
                }
            }
        }
        
        return new ConfigurationCollectionDataImpl(
                configName,
                configData,
                writebackConfigResourceCollectionParentPath,
                resourceCollectionParentProps,
                configurationManagementSettings
                );
    }
    
    @SuppressWarnings("unchecked")
    private Iterator<Resource> applyPersistence(final Iterator<Resource> configResourceInheritanceChain, final boolean isCollection) {
        if (configResourceInheritanceChain == null) {
            return null;
        }
        return IteratorUtils.transformedIterator(configResourceInheritanceChain,
                new Transformer() {
                    @Override
                    public Object transform(Object input) {
                        if (isCollection) {
                            return configurationPersistenceStrategy.getCollectionItemResource((Resource)input);
                        }
                        else {
                            return configurationPersistenceStrategy.getResource((Resource)input);
                        }
                    }
                });
    }

    private Resource applyPersistenceAndInheritance(String contextPath, String configName, Iterator<Resource> configResourceInheritanceChain,
            boolean isCollection, ResourceResolver resourceResolver) {
        if (configResourceInheritanceChain == null) {
            return null;
        }
        
        // apply configuration persistence transformation
        Iterator<Resource> transformedConfigResources = applyPersistence(configResourceInheritanceChain, isCollection);
        
        // apply resource inheritance
        Resource configResource = configurationInheritanceStrategy.getResource(transformedConfigResources);
        
        // apply overrides
        return configurationOverrideMultiplexer.overrideProperties(contextPath, configName, configResource, resourceResolver);
    }

    @Override
    public void persistConfiguration(@NotNull Resource resource, @NotNull String configName, @NotNull ConfigurationPersistData data) {
        ConfigNameUtil.ensureValidConfigName(configName);
        String configResourcePath = configurationResourceResolvingStrategy.getResourcePath(resource, CONFIGS_BUCKET_NAME, configName);
        if (configResourcePath == null) {
            throw new ConfigurationPersistenceException("Unable to persist configuration: Configuration resolving strategy returned no path.");
        }
        if (log.isDebugEnabled()) {
            log.debug("Persist configuration for context path {}, name '{}' to {}", resource.getPath(), configName, configResourcePath);
        }
        if (!configurationPersistenceStrategy.persistConfiguration(resource.getResourceResolver(), configResourcePath, data)) {
            throw new ConfigurationPersistenceException("Unable to persist configuration: No persistence strategy found.");
        }
    }

    @Override
    public void persistConfigurationCollection(@NotNull Resource resource, @NotNull String configName, @NotNull ConfigurationCollectionPersistData data) {
        ConfigNameUtil.ensureValidConfigName(configName);
        String configResourceParentPath = configurationResourceResolvingStrategy.getResourceCollectionParentPath(resource, CONFIGS_BUCKET_NAME, configName);
        if (configResourceParentPath == null) {
            throw new ConfigurationPersistenceException("Unable to persist configuration collection: Configuration resolving strategy returned no parent path.");
        }
        if (log.isDebugEnabled()) {
            log.debug("Persist configuration collection for context path {}, name '{}' to {}", resource.getPath(), configName, configResourceParentPath);
        }
        if (!configurationPersistenceStrategy.persistConfigurationCollection(resource.getResourceResolver(), configResourceParentPath, data)) {
            throw new ConfigurationPersistenceException("Unable to persist configuration: No persistence strategy found.");
        }
    }

    @Override
    public ConfigurationData newCollectionItem(@NotNull Resource resource, @NotNull String configName) {
        ConfigNameUtil.ensureValidConfigName(configName);
        ConfigurationMetadata configMetadata = getConfigurationMetadata(configName);
        if (configMetadata != null) {
            return new ConfigurationDataImpl(configMetadata,
                    resource, configName, this, configurationManagementSettings,
                    configurationOverrideMultiplexer, configurationPersistenceStrategy, true);
        }
        return null;
    }

    @Override
    public void deleteConfiguration(@NotNull Resource resource, @NotNull String configName) {
        ConfigNameUtil.ensureValidConfigName(configName);
        
        // try to delete from all config bucket names
        boolean foundAnyPath = false;
        for (String configBucketName : configurationResourceResolverConfig.configBucketNames()) {
            String configResourcePath = configurationResourceResolvingStrategy.getResourcePath(resource, configBucketName, configName);
            if (configResourcePath != null) {
                foundAnyPath = true;
                if (log.isDebugEnabled()) {
                    log.debug("Delete configuration for context path {}, name '{}' from {}", resource.getPath(), configName, configResourcePath);
                }
                if (!configurationPersistenceStrategy.deleteConfiguration(resource.getResourceResolver(), configResourcePath)) {
                    throw new ConfigurationPersistenceException("Unable to delete configuration: No persistence strategy found.");
                }
            }
        }
        if (!foundAnyPath) {
            throw new ConfigurationPersistenceException("Unable to delete configuration: Configuration resolving strategy returned no path.");
        }
    }
    
    @Override
    public @NotNull SortedSet<String> getConfigurationNames() {
        return configurationMetadataProvider.getConfigurationNames();
    }

    @Override
    public ConfigurationMetadata getConfigurationMetadata(@NotNull String configName) {
        ConfigNameUtil.ensureValidConfigName(configName);
        ConfigurationMetadata metadata = configurationMetadataProvider.getConfigurationMetadata(configName);
        if (metadata != null) {
            log.trace("+ Configuration metadata found for: {}", configName);
            return metadata;
        }
        
        // if no metadata found with direct match try to resolve nested configuration metadata references
        for (String partialConfigName : ConfigNameUtil.getAllPartialConfigNameVariations(configName)) {
            ConfigurationMetadata partialConfigMetadata = getConfigurationMetadata(partialConfigName);
            if (partialConfigMetadata != null) {
                ConfigurationMetadata nestedConfigMetadata = getNestedConfigurationMetadata(partialConfigMetadata, configName, partialConfigName);
                if (nestedConfigMetadata != null) {
                    log.trace("+ Nested configuration metadata found for: {}", configName);
                    return nestedConfigMetadata;
                }
            }
        }

        log.trace("- No configuration metadata found for: {}", configName);
        return null;
    }
    
    private ConfigurationMetadata getNestedConfigurationMetadata(ConfigurationMetadata configMetadata, String configName, String partialConfigName) {
        if (StringUtils.startsWith(configName, partialConfigName + "/")) {
            
            // depending on different persistence strategies config names can be transformed differently - try all combinations here
            Set<String> prefixesToRemove = new LinkedHashSet<>();
            if (configMetadata.isCollection()) {
                String collectionItemName = StringUtils.substringBefore(StringUtils.substringAfter(configName, partialConfigName + "/"), "/");
                for (String collectionParentConfigName : configurationPersistenceStrategy.getAllCollectionParentConfigNames(partialConfigName)) {
                    for (String collectionItemConfigName : configurationPersistenceStrategy.getAllCollectionItemConfigNames(collectionItemName)) {
                        prefixesToRemove.add(collectionParentConfigName  + "/" + collectionItemConfigName + "/");
                    }
                }
            }
            else {
                for (String configNameItem : configurationPersistenceStrategy.getAllConfigNames(partialConfigName)) {
                    prefixesToRemove.add(configNameItem + "/");
                }
            }
            
            for (String prefixToRemove : prefixesToRemove) {
                String remainingConfigName = StringUtils.substringAfter(configName, prefixToRemove);
                // try direct match
                ConfigurationMetadata nestedConfigMetadata = getNestedConfigurationMetadataFromProperty(configMetadata, remainingConfigName);
                if (nestedConfigMetadata != null) {
                    return nestedConfigMetadata;
                }
                // try to find partial match for deeper nestings
                for (String partialRemainingConfigName : ConfigNameUtil.getAllPartialConfigNameVariations(remainingConfigName)) {
                    ConfigurationMetadata partialConfigMetadata = getNestedConfigurationMetadataFromProperty(configMetadata, partialRemainingConfigName);
                    if (partialConfigMetadata != null) {
                        nestedConfigMetadata = getNestedConfigurationMetadata(partialConfigMetadata, remainingConfigName, partialRemainingConfigName);
                        if (nestedConfigMetadata != null) {
                            return nestedConfigMetadata;
                        }
                    }
                }
            }
        }
        return null;
    }
    
    private ConfigurationMetadata getNestedConfigurationMetadataFromProperty(ConfigurationMetadata partialConfigMetadata, String configName) {
        for (PropertyMetadata<?> propertyMetadata : partialConfigMetadata.getPropertyMetadata().values()) {
            if (propertyMetadata.isNestedConfiguration()) {
                ConfigurationMetadata nestedConfigMetadata = propertyMetadata.getConfigurationMetadata();
                if (StringUtils.equals(configName, nestedConfigMetadata.getName())) {
                    return nestedConfigMetadata;
                }
            }
        }
        return null;
    }

    @Override
    public String getPersistenceResourcePath(@NotNull String configResourcePath) {
        return configurationPersistenceStrategy.getResourcePath(configResourcePath);
    }

}
