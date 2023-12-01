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
package org.apache.sling.feature.extension.apiregions.analyser;

import java.io.IOException;
import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import javax.json.JsonArray;

import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.Extensions;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.analyser.task.AnalyserTask;
import org.apache.sling.feature.analyser.task.AnalyserTaskContext;
import org.apache.sling.feature.extension.apiregions.api.ApiRegion;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.apache.sling.feature.scanner.BundleDescriptor;
import org.apache.sling.feature.scanner.PackageInfo;
import org.osgi.framework.Version;

public class CheckApiRegionsBundleExportsImports implements AnalyserTask {

    private static final String IGNORE_API_REGIONS_CONFIG_KEY = "ignoreAPIRegions";
    private static final String GLOBAL_REGION = "global";
    private static final String NO_REGION = " __NO_REGION__ ";
    private static final String OWN_FEATURE = " __OWN_FEATURE__ ";

    @Override
    public String getName() {
        return "Bundle Import/Export Check";
    }

    @Override
    public String getId() {
        return ApiRegions.EXTENSION_NAME + "-exportsimports";
    }

    public static final class Report {

        public List<PackageInfo> exportWithoutVersion = new ArrayList<>();

        public List<PackageInfo> exportMatchingSeveral = new ArrayList<>();

        public List<PackageInfo> importWithoutVersion = new ArrayList<>();

        public List<PackageInfo> missingExports = new ArrayList<>();

        public List<PackageInfo> missingExportsWithVersion = new ArrayList<>();

        public List<PackageInfo> missingExportsForOptional = new ArrayList<>();

        public Map<PackageInfo, Map.Entry<Set<String>, Set<String>>> regionInfo = new HashMap<>();
    }

    private Report getReport(final Map<BundleDescriptor, Report> reports, final BundleDescriptor info) {
        Report report = reports.get(info);
        if ( report == null ) {
            report = new Report();
            reports.put(info, report);
        }
        return report;
    }

    private void checkForVersionOnExportedPackages(final AnalyserTaskContext ctx, final Map<BundleDescriptor, Report> reports) {
        for(final BundleDescriptor info : ctx.getFeatureDescriptor().getBundleDescriptors()) {
            if ( info.getExportedPackages() != null ) {
                for(final PackageInfo i : info.getExportedPackages()) {
                    if ( i.getPackageVersion().compareTo(Version.emptyVersion) == 0 ) {
                        getReport(reports, info).exportWithoutVersion.add(i);
                    }
                }
            }
        }
    }

    private void checkForVersionOnImportingPackages(final AnalyserTaskContext ctx, final Map<BundleDescriptor, Report> reports) {
        for(final BundleDescriptor info : ctx.getFeatureDescriptor().getBundleDescriptors()) {
            if ( info.getImportedPackages() != null ) {
                for(final PackageInfo i : info.getImportedPackages()) {
                    if ( i.getVersion() == null ) {
                        // don't report for javax, org.xml. and org.w3c. packages (TODO)
                        if ( !i.getName().startsWith("javax.")
                                && !i.getName().startsWith("org.w3c.") && !i.getName().startsWith("org.xml.")) {
                            getReport(reports, info).importWithoutVersion.add(i);
                        }
                    }
                }
            }
        }
    }

    @Override
public void execute(final org.apache.sling.feature.analyser.task.AnalyserTaskContext ctx) throws java.lang.Exception {
    boolean ignoreAPIRegions = ctx.getConfiguration().getOrDefault(org.apache.sling.feature.extension.apiregions.analyser.CheckApiRegionsBundleExportsImports.IGNORE_API_REGIONS_CONFIG_KEY, "false").equalsIgnoreCase("true");
    // basic checks
    final java.util.Map<org.apache.sling.feature.scanner.BundleDescriptor, org.apache.sling.feature.extension.apiregions.analyser.CheckApiRegionsBundleExportsImports.Report> reports = new java.util.HashMap<>();
    checkForVersionOnExportedPackages(ctx, reports);
    checkForVersionOnImportingPackages(ctx, reports);
    final java.util.SortedMap<java.lang.Integer, java.util.List<org.apache.sling.feature.scanner.BundleDescriptor>> bundlesMap = new java.util.TreeMap<>();
    for (final org.apache.sling.feature.scanner.BundleDescriptor bi : ctx.getFeatureDescriptor().getBundleDescriptors()) {
        java.util.List<org.apache.sling.feature.scanner.BundleDescriptor> list = bundlesMap.get(bi.getArtifact().getStartOrder());
        if (list == null) {
            list = new java.util.ArrayList<>();
            bundlesMap.put(bi.getArtifact().getStartOrder(), list);
        }
        list.add(bi);
    }
    // add all system packages
    final java.util.List<org.apache.sling.feature.scanner.BundleDescriptor> exportingBundles = new java.util.ArrayList<>();
    {
        exportingBundles.add(/* NPEX_NULL_EXP */
        ctx.getFrameworkDescriptor());
    }
    org.apache.sling.feature.extension.apiregions.api.ApiRegions apiRegions = new org.apache.sling.feature.extension.apiregions.api.ApiRegions();// Empty API Regions;

    org.apache.sling.feature.Feature feature = ctx.getFeature();
    // extract and check the api-regions
    org.apache.sling.feature.Extensions extensions = feature.getExtensions();
    org.apache.sling.feature.Extension apiRegionsExtension = extensions.getByName(org.apache.sling.feature.extension.apiregions.api.ApiRegions.EXTENSION_NAME);
    if ((apiRegionsExtension != null) && (apiRegionsExtension.getJSONStructure() != null)) {
        try {
            apiRegions = org.apache.sling.feature.extension.apiregions.api.ApiRegions.parse(((javax.json.JsonArray) (apiRegionsExtension.getJSONStructure())));
        } catch (java.io.IOException e) {
            ctx.reportError((("API Regions '" + apiRegionsExtension.getJSON()) + "' does not represent a valid JSON 'api-regions': ") + e.getMessage());
            return;
        }
    }
    for (final java.util.Map.Entry<java.lang.Integer, java.util.List<org.apache.sling.feature.scanner.BundleDescriptor>> entry : bundlesMap.entrySet()) {
        // first add all exporting bundles
        for (final org.apache.sling.feature.scanner.BundleDescriptor info : entry.getValue()) {
            if (info.getExportedPackages() != null) {
                exportingBundles.add(info);
            }
        }
        // check importing bundles
        for (final org.apache.sling.feature.scanner.BundleDescriptor info : entry.getValue()) {
            if (info.getImportedPackages() != null) {
                for (final org.apache.sling.feature.scanner.PackageInfo pck : info.getImportedPackages()) {
                    final java.util.Map<org.apache.sling.feature.scanner.BundleDescriptor, java.util.Set<java.lang.String>> candidates = getCandidates(exportingBundles, pck, info, apiRegions, ignoreAPIRegions);
                    if (candidates.isEmpty()) {
                        if (pck.isOptional()) {
                            getReport(reports, info).missingExportsForOptional.add(pck);
                        } else {
                            getReport(reports, info).missingExports.add(pck);
                        }
                    } else {
                        final java.util.List<org.apache.sling.feature.scanner.BundleDescriptor> matchingCandidates = new java.util.ArrayList<>();
                        java.util.Set<java.lang.String> exportingRegions = new java.util.HashSet<>();
                        java.util.Set<java.lang.String> importingRegions = new java.util.HashSet<>();
                        for (final java.util.Map.Entry<org.apache.sling.feature.scanner.BundleDescriptor, java.util.Set<java.lang.String>> candidate : candidates.entrySet()) {
                            org.apache.sling.feature.scanner.BundleDescriptor bd = candidate.getKey();
                            if (bd.isExportingPackage(pck)) {
                                java.util.Set<java.lang.String> exRegions = candidate.getValue();
                                if (exRegions.contains(org.apache.sling.feature.extension.apiregions.analyser.CheckApiRegionsBundleExportsImports.NO_REGION)) {
                                    // If an export is defined outside of a region, it always matches
                                    matchingCandidates.add(bd);
                                    continue;
                                }
                                if (exRegions.contains(org.apache.sling.feature.extension.apiregions.analyser.CheckApiRegionsBundleExportsImports.GLOBAL_REGION)) {
                                    // Everyone can import from the global regin
                                    matchingCandidates.add(bd);
                                    continue;
                                }
                                if (exRegions.contains(org.apache.sling.feature.extension.apiregions.analyser.CheckApiRegionsBundleExportsImports.OWN_FEATURE)) {
                                    // A feature can always import packages from bundles in itself
                                    matchingCandidates.add(bd);
                                    continue;
                                }
                                // Find out what regions the importing bundle is in
                                java.util.Set<java.lang.String> imRegions = getBundleRegions(info, apiRegions, ignoreAPIRegions);
                                // Record the exporting and importing regions for diagnostics
                                exportingRegions.addAll(exRegions);
                                java.util.Set<java.lang.String> regions = new java.util.HashSet<>();
                                for (java.lang.String region : imRegions) {
                                    for (org.apache.sling.feature.extension.apiregions.api.ApiRegion r = apiRegions.getRegionByName(region); r != null; r = r.getParent()) {
                                        regions.add(r.getName());
                                    }
                                }
                                importingRegions.addAll(regions);
                                // Only keep the regions that also export the package
                                regions.retainAll(exRegions);
                                if (!regions.isEmpty()) {
                                    // there is an overlapping region
                                    matchingCandidates.add(bd);
                                }
                            }
                        }
                        if (matchingCandidates.isEmpty()) {
                            if (pck.isOptional()) {
                                getReport(reports, info).missingExportsForOptional.add(pck);
                            } else {
                                getReport(reports, info).missingExportsWithVersion.add(pck);
                                getReport(reports, info).regionInfo.put(pck, new java.util.AbstractMap.SimpleEntry<>(exportingRegions, importingRegions));
                            }
                        } else if (matchingCandidates.size() > 1) {
                            getReport(reports, info).exportMatchingSeveral.add(pck);
                        }
                    }
                }
            }
        }
    }
    boolean errorReported = false;
    for (final java.util.Map.Entry<org.apache.sling.feature.scanner.BundleDescriptor, org.apache.sling.feature.extension.apiregions.analyser.CheckApiRegionsBundleExportsImports.Report> entry : reports.entrySet()) {
        final java.lang.String key = (("Bundle " + entry.getKey().getArtifact().getId().getArtifactId()) + ":") + entry.getKey().getArtifact().getId().getVersion();
        if (!entry.getValue().importWithoutVersion.isEmpty()) {
            ctx.reportWarning(((key + " is importing package(s) ") + getPackageInfo(entry.getValue().importWithoutVersion, false)) + " without specifying a version range.");
        }
        if (!entry.getValue().exportWithoutVersion.isEmpty()) {
            ctx.reportWarning(((key + " is exporting package(s) ") + getPackageInfo(entry.getValue().importWithoutVersion, false)) + " without a version.");
        }
        if (!entry.getValue().missingExports.isEmpty()) {
            ctx.reportError(((((key + " is importing package(s) ") + getPackageInfo(entry.getValue().missingExports, false)) + " in start level ") + java.lang.String.valueOf(entry.getKey().getArtifact().getStartOrder())) + " but no bundle is exporting these for that start level.");
            errorReported = true;
        }
        if (!entry.getValue().missingExportsWithVersion.isEmpty()) {
            java.lang.StringBuilder message = new java.lang.StringBuilder(((((key + " is importing package(s) ") + getPackageInfo(entry.getValue().missingExportsWithVersion, true)) + " in start level ") + java.lang.String.valueOf(entry.getKey().getArtifact().getStartOrder())) + " but no visible bundle is exporting these for that start level in the required version range.");
            for (java.util.Map.Entry<org.apache.sling.feature.scanner.PackageInfo, java.util.Map.Entry<java.util.Set<java.lang.String>, java.util.Set<java.lang.String>>> regionInfoEntry : entry.getValue().regionInfo.entrySet()) {
                org.apache.sling.feature.scanner.PackageInfo pkg = regionInfoEntry.getKey();
                java.util.Map.Entry<java.util.Set<java.lang.String>, java.util.Set<java.lang.String>> regions = regionInfoEntry.getValue();
                if (regions.getKey().size() > 0) {
                    message.append((((("\n" + pkg.getName()) + " is exported in regions ") + regions.getKey()) + " but it is imported in regions ") + regions.getValue());
                }
            }
            ctx.reportError(message.toString());
            errorReported = true;
        }
    }
    if (errorReported && ctx.getFeature().isComplete()) {
        ctx.reportError(ctx.getFeature().getId().toMvnId() + " is marked as 'complete' but has missing imports.");
    }
}

    private Set<String> getBundleRegions(BundleDescriptor info, ApiRegions regions, boolean ignoreAPIRegions) {
        Set<String> result = ignoreAPIRegions ? Collections.emptySet() : Stream.of(info.getArtifact().getFeatureOrigins())
            .map(regions::getRegionsByFeature).flatMap(Stream::of).map(ApiRegion::getName).collect(Collectors.toSet());

        if (result.isEmpty()) {
            result = new HashSet<>();
            result.add(NO_REGION);
        }
        return result;
    }


    private String getPackageInfo(final List<PackageInfo> pcks, final boolean includeVersion) {
        if ( pcks.size() == 1 ) {
            if (includeVersion) {
                return pcks.get(0).toString();
            } else {
                return pcks.get(0).getName();
            }
        }
        final StringBuilder sb = new StringBuilder();
        boolean first = true;
        sb.append('[');
        for(final PackageInfo info : pcks) {
            if ( first ) {
                first = false;
            } else {
                sb.append(", ");
            }
            if (includeVersion) {
                sb.append(info.toString());
            } else {
                sb.append(info.getName());
            }
        }
        sb.append(']');
        return sb.toString();
    }

    private Map<BundleDescriptor, Set<String>> getCandidates(
            final List<BundleDescriptor> exportingBundles,
            final PackageInfo pck,
            final BundleDescriptor requestingBundle,
            final ApiRegions apiRegions, boolean ignoreAPIRegions) {
        Set<String> rf = ignoreAPIRegions ? Collections.emptySet() : Stream.of(requestingBundle.getArtifact().getFeatureOrigins()).map(ArtifactId::toMvnId).collect(Collectors.toSet());

        final Set<String> requestingFeatures = rf;

        final Map<BundleDescriptor, Set<String>> candidates = new HashMap<>();
        for(final BundleDescriptor info : exportingBundles) {
            if ( info.isExportingPackage(pck.getName()) ) {
                Set<String> providingFeatures = ignoreAPIRegions ? Collections.emptySet() : Stream.of(info.getArtifact().getFeatureOrigins()).map(ArtifactId::toMvnId).collect(Collectors.toSet());

                // Compute the intersection without modifying the sets
                Set<String> intersection = providingFeatures.stream().filter(
                        s -> requestingFeatures.contains(s)).collect(Collectors.toSet());
                if (!intersection.isEmpty()) {
                    // A requesting bundle can see all exported packages inside its own feature
                    candidates.put(info, Collections.singleton(OWN_FEATURE));
                    continue;
                }

                for (String region : getBundleRegions(info, apiRegions, ignoreAPIRegions)) {
                    if (!NO_REGION.equals(region) &&
                            (apiRegions.getRegionByName(region) == null
                                    || apiRegions.getRegionByName(region).getAllExportByName(pck.getName()) == null))
                        continue;

                    Set<String> regions = candidates.get(info);
                    if (regions == null) {
                        regions = new HashSet<>();
                        candidates.put(info, regions);
                    }
                    regions.add(region);
                }
            }
        }
        return candidates;
    }
}
