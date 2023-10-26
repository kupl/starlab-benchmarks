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

import java.util.Collection;
import java.util.Formatter;

import org.apache.sling.feature.analyser.task.AnalyserTaskContext;
import org.apache.sling.feature.extension.apiregions.api.ApiExport;
import org.apache.sling.feature.extension.apiregions.api.ApiRegion;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.apache.sling.feature.scanner.BundleDescriptor;
import org.apache.sling.feature.scanner.FeatureDescriptor;
import org.apache.sling.feature.scanner.PackageInfo;

public class CheckApiRegions extends AbstractApiRegionsAnalyserTask {

    @Override
    public String getId() {
        return ApiRegions.EXTENSION_NAME;
    }

    @Override
    public String getName() {
        return "Api Regions analyser task";
    }

    @Override
    protected void execute(ApiRegions apiRegions, AnalyserTaskContext ctx) throws Exception {
        // for each bundle, get the Export-Package and process the packages

        FeatureDescriptor featureDescriptor = ctx.getFeatureDescriptor();
        for (BundleDescriptor bundleDescriptor : featureDescriptor.getBundleDescriptors()) {
            for (PackageInfo packageInfo : bundleDescriptor.getExportedPackages()) {
                String exportedPackage = packageInfo.getName();
                // use the Sieve technique: remove bundle exported packages from the api-regions
                for (final ApiRegion region : apiRegions.listRegions()) {
                    ApiExport export = region.getExportByName(exportedPackage);
                    if (export != null) {
                        region.remove(export);
                    }
                }
            }
        }

        boolean isEmpty = true;
        for (final ApiRegion region : apiRegions.listRegions()) {
            if (!region.listExports().isEmpty()) {
                isEmpty = false;
                break;
            }
        }
        // final evaluation: if the Sieve is not empty, not all declared packages are exported by bundles of the same feature
        if (!isEmpty) {
            // track a single error for each region
            for (ApiRegion region : apiRegions.listRegions()) {
                if (!region.listExports().isEmpty()) {
                    Formatter formatter = new Formatter();
                    formatter.format("Region '%s' defined in feature '%s' declares %s package%s which %s not exported by any bundle:%n",
                            region.getName(),
                                     ctx.getFeature().getId(),
                            region.listExports().size(), getExtension(region.listExports(), "", "s"),
                            getExtension(region.listExports(), "is", "are"));
                    region.listExports().forEach(api -> formatter.format(" * %s%n", api.getName()));

                    ctx.reportError(formatter.toString());

                    formatter.close();
                }
            }
        }
    }

    // utility methods

    private static <T> String getExtension(Collection<T> collection, String singular, String plural) {
        return collection.size() > 1 ? plural : singular;
    }

}
