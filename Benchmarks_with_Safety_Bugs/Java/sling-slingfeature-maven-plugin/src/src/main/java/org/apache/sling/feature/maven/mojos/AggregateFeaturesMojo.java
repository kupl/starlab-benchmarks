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
    public void execute() throws MojoExecutionException {
        checkPreconditions();
        for (final Aggregate aggregate : aggregates) {
            // check classifier
            ProjectHelper.validateFeatureClassifiers(this.project, aggregate.classifier, aggregate.attach);

            final Map<String, Feature> selection = this.getSelectedFeatures(aggregate);
            if (selection.isEmpty()) {
                throw new MojoExecutionException(
                        "No features found for aggregate with classifier " + aggregate.classifier);
            }

            final Map<String,String> variablesOverwrites = new HashMap<>();
            if (aggregate.variablesOverrides != null)
                variablesOverwrites.putAll(aggregate.variablesOverrides);
            final Map<String,String> frameworkPropertiesOverwrites = new HashMap<>();
            if (aggregate.frameworkPropertiesOverrides != null)
                frameworkPropertiesOverwrites.putAll(aggregate.frameworkPropertiesOverrides);

            final BuilderContext builderContext = new BuilderContext(new BaseFeatureProvider() {
                @Override
                public Feature provide(ArtifactId id) {
                    // check in selection
                    for (final Feature feat : selection.values()) {
                        if (feat.getId().equals(id)) {
                            return feat;
                        }
                    }
                    return super.provide(id);
                }
            }).setArtifactProvider(new BaseArtifactProvider())
                .addVariablesOverrides(variablesOverwrites)
                .addFrameworkPropertiesOverrides(frameworkPropertiesOverwrites)
                .addMergeExtensions(StreamSupport.stream(Spliterators.spliteratorUnknownSize(
                    ServiceLoader.load(MergeHandler.class).iterator(), Spliterator.ORDERED),
                    false).toArray(MergeHandler[]::new))
                .addPostProcessExtensions(StreamSupport.stream(Spliterators.spliteratorUnknownSize(
                    ServiceLoader.load(PostProcessHandler.class).iterator(), Spliterator.ORDERED),
                    false).toArray(PostProcessHandler[]::new));
            for (final ArtifactId rule : aggregate.getArtifactOverrideRules()) {
                builderContext.addArtifactsOverride(rule);
            }

            builderContext.addConfigsOverrides(aggregate.getConfigurationOverrideRules());

            boolean wildcardSet = false;
            for (final Map.Entry<String, Properties> entry : handlerConfiguration.entrySet()) {
                String key = entry.getKey();
                Properties props = entry.getValue();

                if (HANDLER_CONFIG_WILDCARD.equals(key)) {
                    if (!props.containsKey(FILE_STORAGE_CONFIG_KEY)) {
                        props.put(FILE_STORAGE_CONFIG_KEY, getFeatureModelStorage());
                    }
                    wildcardSet = true;
                }

                builderContext.setHandlerConfiguration(key, ProjectHelper.propertiesToMap(props));
            }

            if (!wildcardSet) {
                builderContext.setHandlerConfiguration(HANDLER_CONFIG_WILDCARD,
                        Collections.singletonMap(FILE_STORAGE_CONFIG_KEY, getFeatureModelStorage()));
            }

            final ArtifactId newFeatureID = new ArtifactId(project.getGroupId(), project.getArtifactId(),
                    project.getVersion(), aggregate.classifier, FeatureConstants.PACKAGING_FEATURE);
            final Feature result = assembleFeature(newFeatureID, builderContext, selection);

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

            ProjectHelper.createTmpFeatureFile(project, result);
            ProjectHelper.setFeatureInfo(project, result);

            // Add feature to map of features
            final String key = ProjectHelper.generateAggregateFeatureKey(aggregate.classifier, aggregate.attach);
            ProjectHelper.getAssembledFeatures(project).put(key, result);
            ProjectHelper.getFeatures(this.project).put(key, result);
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
