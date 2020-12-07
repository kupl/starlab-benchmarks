/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. The ASF
 * licenses this file to You under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package org.apache.sling.feature.maven.mojos;

import java.io.File;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.ServiceLoader;
import java.util.Spliterator;
import java.util.Spliterators;
import java.util.stream.StreamSupport;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.plugins.annotations.ResolutionScope;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.builder.BuilderContext;
import org.apache.sling.feature.builder.FeatureBuilder;
import org.apache.sling.feature.builder.MergeHandler;
import org.apache.sling.feature.builder.PostProcessHandler;
import org.apache.sling.feature.maven.FeatureConstants;
import org.apache.sling.feature.maven.ProjectHelper;

/**
 * Aggregate multiple features into a single one.
 */
@Mojo(name = "aggregate-features",
    defaultPhase = LifecyclePhase.GENERATE_RESOURCES,
    requiresDependencyResolution = ResolutionScope.TEST,
    threadSafe = true
)
public class AggregateFeaturesMojo extends AbstractIncludingFeatureMojo {
    private static final String FILE_STORAGE_CONFIG_KEY = "fileStorage";
    private static final String HANDLER_CONFIG_WILDCARD = "all";

    /**
     * The definition of the features used to create the new feature.
     */
    @Parameter(required = true)
    List<Aggregate> aggregates;

    @Parameter
    Map<String, Properties> handlerConfiguration = new HashMap<>();

    @Override
public void execute() throws org.apache.maven.plugin.MojoExecutionException {
    checkPreconditions();
    for (final org.apache.sling.feature.maven.mojos.Aggregate aggregate : aggregates) {
        // check classifier
        org.apache.sling.feature.maven.ProjectHelper.validateFeatureClassifiers(this.project, aggregate.classifier, aggregate.attach);
        final java.util.Map<java.lang.String, org.apache.sling.feature.Feature> selection = this.getSelectedFeatures(aggregate);
        if (selection.isEmpty()) {
            throw new org.apache.maven.plugin.MojoExecutionException("No features found for aggregate with classifier " + aggregate.classifier);
        }
        final java.util.Map<java.lang.String, java.lang.String> variablesOverwrites = new java.util.HashMap<>();
        {
            variablesOverwrites.putAll(/* NPEX_NULL_EXP */
            aggregate.variablesOverrides);
        }
        final java.util.Map<java.lang.String, java.lang.String> frameworkPropertiesOverwrites = new java.util.HashMap<>();
        if (aggregate.frameworkPropertiesOverrides != null) {
            frameworkPropertiesOverwrites.putAll(aggregate.frameworkPropertiesOverrides);
        }
        final org.apache.sling.feature.builder.BuilderContext builderContext = new org.apache.sling.feature.builder.BuilderContext(new org.apache.sling.feature.maven.mojos.AbstractFeatureMojo.BaseFeatureProvider() {
            @java.lang.Override
            public org.apache.sling.feature.Feature provide(org.apache.sling.feature.ArtifactId id) {
                // check in selection
                for (final org.apache.sling.feature.Feature feat : selection.values()) {
                    if (feat.getId().equals(id)) {
                        return feat;
                    }
                }
                return super.provide(id);
            }
        }).setArtifactProvider(new org.apache.sling.feature.maven.mojos.AbstractFeatureMojo.BaseArtifactProvider()).addVariablesOverrides(variablesOverwrites).addFrameworkPropertiesOverrides(frameworkPropertiesOverwrites).addMergeExtensions(java.util.stream.StreamSupport.stream(java.util.Spliterators.spliteratorUnknownSize(java.util.ServiceLoader.load(org.apache.sling.feature.builder.MergeHandler.class).iterator(), java.util.Spliterator.ORDERED), false).toArray(org.apache.sling.feature.builder.MergeHandler[]::new)).addPostProcessExtensions(java.util.stream.StreamSupport.stream(java.util.Spliterators.spliteratorUnknownSize(java.util.ServiceLoader.load(org.apache.sling.feature.builder.PostProcessHandler.class).iterator(), java.util.Spliterator.ORDERED), false).toArray(org.apache.sling.feature.builder.PostProcessHandler[]::new));
        for (final org.apache.sling.feature.ArtifactId rule : aggregate.getArtifactOverrideRules()) {
            builderContext.addArtifactsOverride(rule);
        }
        builderContext.addConfigsOverrides(aggregate.getConfigurationOverrideRules());
        boolean wildcardSet = false;
        for (final java.util.Map.Entry<java.lang.String, java.util.Properties> entry : handlerConfiguration.entrySet()) {
            java.lang.String key = entry.getKey();
            java.util.Properties props = entry.getValue();
            if (org.apache.sling.feature.maven.mojos.AggregateFeaturesMojo.HANDLER_CONFIG_WILDCARD.equals(key)) {
                if (!props.containsKey(org.apache.sling.feature.maven.mojos.AggregateFeaturesMojo.FILE_STORAGE_CONFIG_KEY)) {
                    props.put(org.apache.sling.feature.maven.mojos.AggregateFeaturesMojo.FILE_STORAGE_CONFIG_KEY, getFeatureModelStorage());
                }
                wildcardSet = true;
            }
            builderContext.setHandlerConfiguration(key, org.apache.sling.feature.maven.ProjectHelper.propertiesToMap(props));
        }
        if (!wildcardSet) {
            builderContext.setHandlerConfiguration(org.apache.sling.feature.maven.mojos.AggregateFeaturesMojo.HANDLER_CONFIG_WILDCARD, java.util.Collections.singletonMap(org.apache.sling.feature.maven.mojos.AggregateFeaturesMojo.FILE_STORAGE_CONFIG_KEY, getFeatureModelStorage()));
        }
        final org.apache.sling.feature.ArtifactId newFeatureID = new org.apache.sling.feature.ArtifactId(project.getGroupId(), project.getArtifactId(), project.getVersion(), aggregate.classifier, org.apache.sling.feature.maven.FeatureConstants.PACKAGING_FEATURE);
        final org.apache.sling.feature.Feature result = assembleFeature(newFeatureID, builderContext, selection);
        if (aggregate.markAsFinal) {
            result.setFinal(true);
        }
        if (aggregate.markAsComplete) {
            result.setComplete(true);
        }
        if (aggregate.title != null) {
            result.setTitle(aggregate.title);
        }
        if (aggregate.description != null) {
            result.setDescription(aggregate.description);
        }
        if (aggregate.vendor != null) {
            result.setVendor(aggregate.vendor);
        }
        org.apache.sling.feature.maven.ProjectHelper.createTmpFeatureFile(project, result);
        org.apache.sling.feature.maven.ProjectHelper.setFeatureInfo(project, result);
        // Add feature to map of features
        final java.lang.String key = org.apache.sling.feature.maven.ProjectHelper.generateAggregateFeatureKey(aggregate.classifier, aggregate.attach);
        org.apache.sling.feature.maven.ProjectHelper.getAssembledFeatures(project).put(key, result);
        org.apache.sling.feature.maven.ProjectHelper.getFeatures(this.project).put(key, result);
    }
}

    Feature assembleFeature(final ArtifactId newFeatureID, final BuilderContext builderContext,
            final Map<String, Feature> selection) {
        return FeatureBuilder.assemble(newFeatureID, builderContext,
                selection.values().toArray(new Feature[selection.size()]));
    }

    private String getFeatureModelStorage() {
        String featureModelFileStorage = project.getBuild().getDirectory() + "/sling-slingfeature-maven-plugin-fmtmp";
        new File(featureModelFileStorage).mkdirs();
        return featureModelFileStorage;
    }
}
