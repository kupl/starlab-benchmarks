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
package org.apache.sling.feature.extension.apiregions.analyser;

import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.analyser.task.AnalyserTaskContext;
import org.apache.sling.feature.extension.apiregions.api.ApiExport;
import org.apache.sling.feature.extension.apiregions.api.ApiRegion;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.apache.sling.feature.scanner.BundleDescriptor;
import org.apache.sling.feature.scanner.FeatureDescriptor;
import org.apache.sling.feature.scanner.PackageInfo;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

public class CheckApiRegionsCrossFeatureDups extends AbstractApiRegionsAnalyserTask {

    @Override
    public String getId() {
        return ApiRegions.EXTENSION_NAME + "-crossfeature-dups";
    }

    @Override
    public String getName() {
        return "Api Regions cross-feature duplicate export task";
    }

    @Override
    protected void execute(ApiRegions apiRegions, AnalyserTaskContext ctx) throws Exception {
        Set<String> checkedRegions = splitListConfig(ctx.getConfiguration().get("regions"));
        Set<String> ignoredPackages = splitListConfig(ctx.getConfiguration().get("ignoredPackages"));
        Set<String> warningPackages = splitListConfig(ctx.getConfiguration().get("warningPackages"));

        Map<String, Set<String>> regionExports = new HashMap<>();

        List<ArtifactId> apiRegionsFeatures = new ArrayList<>();
        for (ApiRegion r : apiRegions.listRegions()) {
            apiRegionsFeatures.addAll(Arrays.asList(r.getFeatureOrigins()));
            if (checkedRegions.isEmpty() || checkedRegions.contains(r.getName())) {
                Set<String> exports = regionExports.get(r.getName());
                if (exports == null) {
                    exports = new HashSet<>();
                    regionExports.put(r.getName(), exports);
                }
                exports.addAll(r.listExports().stream().map(ApiExport::getName).collect(Collectors.toSet()));
            }
        }

        FeatureDescriptor f = ctx.getFeatureDescriptor();
        for (BundleDescriptor bd : f.getBundleDescriptors()) {
            List<ArtifactId> borgs = new ArrayList<>(Arrays.asList(bd.getArtifact().getFeatureOrigins()));
            borgs.removeAll(apiRegionsFeatures);

            if (!borgs.isEmpty()) {
                // This bundle was contributed by a feature that did not opt-in to the API Regions
                Set<String> reportedPackages = new HashSet<>();
                for (PackageInfo pi : bd.getExportedPackages()) {
                    String pkgName = pi.getName();
                    for (Map.Entry<String, Set<String>> entry : regionExports.entrySet()) {
                        if (entry.getValue().contains(pkgName) && !reportedPackages.contains(pkgName)) {
                            if (matchesSet(pkgName, ignoredPackages)) {
                                continue;
                            }

                            reportedPackages.add(pi.getName());

                            String msg = "Package overlap found between region " + entry.getKey()
                                + " and bundle " + bd.getBundleSymbolicName() + " " + bd.getBundleVersion()
                                + " which comes from a feature without API Regions: " + borgs
                                + ". Both export package: " + pi.getName();
                            if (matchesSet(pkgName, warningPackages)) {
                                ctx.reportWarning(msg);
                            } else {
                                ctx.reportError(msg);
                            }
                        }
                    }
                }
            }
        }
    }

    private boolean matchesSet(String pkg, Set<String> set) {
        for (String e : set) {
            if (e.endsWith("*")) {
                if (pkg.startsWith(e.substring(0, e.length() - 1)) ) {
                    return true;
                }
            } else {
                if (pkg.equals(e)) {
                    return true;
                }
            }
        }
        return false;
    }

private java.util.Set<java.lang.String> splitListConfig(java.lang.String value) {
    {
        return java.util.Arrays.asList(/* NPEX_NULL_EXP */
        value.split(",")).stream().map(java.lang.String::trim).collect(java.util.stream.Collectors.toSet());
    }
}
}
