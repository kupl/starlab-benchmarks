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
import javax.json.JsonArray;

import org.apache.sling.feature.Extension;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.apache.sling.feature.launcher.spi.extensions.ExtensionContext;
import org.apache.sling.feature.launcher.spi.extensions.ExtensionHandler;

public class RegionLauncherExtension implements ExtensionHandler
{
    public static final String FEATURE_REGION_FILENAME = "features.properties";
    public static final String REGION_PACKAGE_FILENAME = "regions.properties";

    @Override
    public boolean handle(ExtensionContext extensionContext, Extension extension) throws Exception
    {
        if (!extension.getName().equals(ApiRegions.EXTENSION_NAME))
            return false;

        final File base = File.createTempFile("apiregions", ".properties");
        base.delete();
        base.mkdirs();
        File featuresFile = new File(base, FEATURE_REGION_FILENAME);
        File regionsFile = new File(base, REGION_PACKAGE_FILENAME);

        final ApiRegions apiRegions = ApiRegions.parse((JsonArray) extension.getJSONStructure());

        LauncherProperties.save(LauncherProperties.getFeatureIDtoRegionsMap(apiRegions), featuresFile);
        LauncherProperties.save(LauncherProperties.getRegionNametoPackagesMap(apiRegions), regionsFile);

        extensionContext.addFrameworkProperty("sling.feature.apiregions.resource." + FEATURE_REGION_FILENAME, featuresFile.toURI().toURL().toString());
        extensionContext.addFrameworkProperty("sling.feature.apiregions.resource." + REGION_PACKAGE_FILENAME, regionsFile.toURI().toURL().toString());

        return true;
    }
}
