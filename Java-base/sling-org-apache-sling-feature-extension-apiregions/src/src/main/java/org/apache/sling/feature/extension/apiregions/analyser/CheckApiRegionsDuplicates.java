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

import java.util.Formatter;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.sling.feature.analyser.task.AnalyserTaskContext;
import org.apache.sling.feature.extension.apiregions.api.ApiExport;
import org.apache.sling.feature.extension.apiregions.api.ApiRegion;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;

public class CheckApiRegionsDuplicates extends AbstractApiRegionsAnalyserTask {

    @Override
    public String getId() {
        return ApiRegions.EXTENSION_NAME + "-duplicates";
    }

    @Override
    public String getName() {
        return "Api Regions duplicates analyser task";
    }

    @Override
    protected void execute(ApiRegions apiRegions, AnalyserTaskContext ctx) throws Exception {
        for (int i = 0; i < apiRegions.listRegions().size(); i++) {
            ApiRegion sourceRegion = apiRegions.listRegions().get(i);
            List<ApiRegion> targetRegions = apiRegions.listRegions().subList(i + 1, apiRegions.listRegions().size());

            for (ApiRegion targetRegion : targetRegions) {
                if (sourceRegion.equals(targetRegion)) {
                    continue;
                }

                Set<String> intersection = calculateIntersection(sourceRegion, targetRegion);
                if (!intersection.isEmpty()) {
                    Formatter formatter = new Formatter();
                    formatter.format("Regions '%s' and '%s' defined in feature '%s' declare both %s package(s):%n",
                            sourceRegion.getName(), targetRegion.getName(),
                                     ctx.getFeature().getId(),
                                     intersection.size());
                    intersection.forEach(api -> formatter.format(" * %s%n", api));

                    ctx.reportError(formatter.toString());

                    formatter.close();
                }
            }
        }
    }

    private static Set<String> calculateIntersection(ApiRegion source, ApiRegion target) {
        final Set<String> intersection = new HashSet<>();

        for (ApiExport packageName : source.listExports()) {
            if (target.getExportByName(packageName.getName()) != null) {
                intersection.add(packageName.getName());
            }
        }

        return intersection;
    }

}
