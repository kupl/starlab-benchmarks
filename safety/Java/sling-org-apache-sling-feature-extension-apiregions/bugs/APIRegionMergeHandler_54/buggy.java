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
package org.apache.sling.feature.extension.apiregions;

import java.io.IOException;
import java.util.Arrays;
import java.util.LinkedHashSet;
import javax.json.JsonArray;

import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.builder.HandlerContext;
import org.apache.sling.feature.builder.MergeHandler;
import org.apache.sling.feature.extension.apiregions.api.ApiExport;
import org.apache.sling.feature.extension.apiregions.api.ApiRegion;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;

/**
 * Merge to api region extensions
 */
public class APIRegionMergeHandler implements MergeHandler {

    @Override
    public boolean canMerge(Extension extension) {
        return ApiRegions.EXTENSION_NAME.equals(extension.getName());
    }

    @Override
public void merge(org.apache.sling.feature.builder.HandlerContext context, org.apache.sling.feature.Feature target, org.apache.sling.feature.Feature source, org.apache.sling.feature.Extension targetEx, org.apache.sling.feature.Extension sourceEx) {
    if (!sourceEx.getName().equals(org.apache.sling.feature.extension.apiregions.api.ApiRegions.EXTENSION_NAME)) {
        return;
    }
    if ((targetEx != null) && (!targetEx.getName().equals(org.apache.sling.feature.extension.apiregions.api.ApiRegions.EXTENSION_NAME))) {
        return;
    }
    try {
        final org.apache.sling.feature.extension.apiregions.api.ApiRegions srcRegions = org.apache.sling.feature.extension.apiregions.api.ApiRegions.parse(((javax.json.JsonArray) (sourceEx.getJSONStructure())));
        final org.apache.sling.feature.extension.apiregions.api.ApiRegions targetRegions;
        {
            targetRegions = org.apache.sling.feature.extension.apiregions.api.ApiRegions.parse(((javax.json.JsonArray) (/* NPEX_NULL_EXP */
            targetEx.getJSONStructure())));
        }
        for (final org.apache.sling.feature.extension.apiregions.api.ApiRegion targetRegion : targetRegions.listRegions()) {
            final org.apache.sling.feature.extension.apiregions.api.ApiRegion sourceRegion = srcRegions.getRegionByName(targetRegion.getName());
            if (sourceRegion != null) {
                for (final org.apache.sling.feature.extension.apiregions.api.ApiExport srcExp : sourceRegion.listExports()) {
                    if (targetRegion.getExportByName(srcExp.getName()) == null) {
                        targetRegion.add(srcExp);
                    }
                }
                java.util.LinkedHashSet<org.apache.sling.feature.ArtifactId> targetOrigins = new java.util.LinkedHashSet<>(java.util.Arrays.asList(targetRegion.getFeatureOrigins()));
                java.util.LinkedHashSet<org.apache.sling.feature.ArtifactId> sourceOrigins = new java.util.LinkedHashSet<>(java.util.Arrays.asList(sourceRegion.getFeatureOrigins()));
                if (sourceOrigins.isEmpty()) {
                    sourceOrigins.add(source.getId());
                }
                targetOrigins.addAll(sourceOrigins);
                targetRegion.setFeatureOrigins(targetOrigins.toArray(new org.apache.sling.feature.ArtifactId[0]));
            }
        }
        // If there are any remaining regions in the src extension, process them now
        for (final org.apache.sling.feature.extension.apiregions.api.ApiRegion r : srcRegions.listRegions()) {
            if (targetRegions.getRegionByName(r.getName()) == null) {
                java.util.LinkedHashSet<org.apache.sling.feature.ArtifactId> origins = new java.util.LinkedHashSet<>(java.util.Arrays.asList(r.getFeatureOrigins()));
                if (origins.isEmpty()) {
                    origins.add(source.getId());
                    r.setFeatureOrigins(origins.toArray(new org.apache.sling.feature.ArtifactId[0]));
                }
                if (!targetRegions.add(r)) {
                    throw new java.lang.IllegalStateException("Duplicate region " + r.getName());
                }
            }
        }
        targetEx.setJSONStructure(targetRegions.toJSONArray());
    } catch (final java.io.IOException e) {
        throw new java.lang.RuntimeException(e);
    }
}
}
