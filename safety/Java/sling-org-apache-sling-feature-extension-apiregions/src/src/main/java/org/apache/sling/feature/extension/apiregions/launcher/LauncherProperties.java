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
package org.apache.sling.feature.extension.apiregions.launcher;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.UncheckedIOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.jar.Attributes;
import java.util.jar.JarFile;
import java.util.stream.Collectors;

import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.builder.ArtifactProvider;
import org.apache.sling.feature.extension.apiregions.api.ApiExport;
import org.apache.sling.feature.extension.apiregions.api.ApiRegion;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.apache.sling.feature.io.IOUtils;
import org.osgi.framework.Constants;
import org.osgi.framework.Version;

public class LauncherProperties
{
    public static Properties getBundleIDtoBSNandVersionMap(Feature app, ArtifactProvider artifactProvider) {
        Map<ArtifactId, String> map = new HashMap<>();

        for (Artifact bundle : app.getBundles())
        {
            map.computeIfAbsent(bundle.getId(), id ->
            {
                try(JarFile jarFile = IOUtils.getJarFileFromURL(artifactProvider.provide(id), true, null)) {
                    Attributes manifest = jarFile.getManifest().getMainAttributes();
                    String bsn = manifest.getValue(Constants.BUNDLE_SYMBOLICNAME);
                    if (bsn != null && !bsn.trim().isEmpty())
                    {
                        return bsn.trim() + "~" + Version.parseVersion(manifest.getValue(Constants.BUNDLE_VERSION));
                    }
                    else {
                        return null;
                    }
                } catch (IOException ex) {
                    throw new UncheckedIOException(ex);
                }
            });
        }

        Properties result = new Properties();

        for (Map.Entry<ArtifactId, String> entry : map.entrySet()) {
            result.setProperty(entry.getKey().toMvnId(), entry.getValue());
        }

        return result;
    }

    public static Properties getBundleIDtoFeaturesMap(Feature app) {
        Map<ArtifactId, Set<ArtifactId>> map = new HashMap<>();

        for (Artifact bundle : app.getBundles())
        {
            map.compute(bundle.getId(), (id, features) ->
            {
                if (features == null)
                {
                    features = new HashSet<>();
                }
                features.addAll(Arrays.asList(bundle.getFeatureOrigins()));
                return features;
            });
        }

        Properties result = new Properties();

        for (Map.Entry<ArtifactId, Set<ArtifactId>> entry : map.entrySet()) {
            result.setProperty(entry.getKey().toMvnId(), entry.getValue().stream().map(ArtifactId::toMvnId).collect(Collectors.joining(",")));
        }

        return result;
    }

    public static Properties getFeatureIDtoRegionsMap(ApiRegions regions) {
        Map<ArtifactId, Set<String>> map = new HashMap<>();

        for (ApiRegion region : regions.listRegions())
        {
            for (ArtifactId featureId : region.getFeatureOrigins()) {
                map.compute(featureId, (id, regionNames) -> {
                    if (regionNames == null) {
                        regionNames = new HashSet<>();
                    }
                    regionNames.add(region.getName());
                    for (ApiRegion parent = region.getParent(); parent != null; parent = parent.getParent()) {
                        regionNames.add(parent.getName());
                    }
                    return regionNames;
                });
            }
        }

        Properties result = new Properties();

        for (Map.Entry<ArtifactId, Set<String>> entry : map.entrySet()) {
            result.setProperty(entry.getKey().toMvnId(), String.join(",", entry.getValue()));
        }

        return result;
    }

    public static Properties getRegionNametoPackagesMap(ApiRegions regions) {
        Map<String, Set<String>> map = new HashMap<>();

        for (ApiRegion region : regions.listRegions())
        {
            for (ApiExport export : region.listExports()) {
                map.compute(region.getName(), (name, exports) -> {
                    if (exports == null) {
                        exports = new HashSet<>();
                    }
                    exports.add(export.getName());
                    return exports;
                });
            }
        }

        Properties result = new Properties();

        for (Map.Entry<String, Set<String>> entry : map.entrySet()) {
            result.setProperty(entry.getKey(), String.join(",", entry.getValue()));
        }

        return result;
    }


    public static void save(Properties properties, File file) throws IOException {
        try (FileOutputStream output = new FileOutputStream(file))
        {
            properties.store(output, "");
        }
    }
}
