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
package org.apache.sling.feature.extension.apiregions.scanner;

import java.io.IOException;

import org.apache.sling.feature.Extension;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.builder.ArtifactProvider;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.apache.sling.feature.scanner.ContainerDescriptor;
import org.apache.sling.feature.scanner.FeatureDescriptor;
import org.apache.sling.feature.scanner.impl.FeatureDescriptorImpl;
import org.apache.sling.feature.scanner.spi.ExtensionScanner;

public class ApiRegionsExtensionScanner implements ExtensionScanner {

    @Override
    public String getId() {
        return ApiRegions.EXTENSION_NAME;
    }

    @Override
    public String getName() {
        return "Api Regions extention scanner";
    }

    @Override
    public ContainerDescriptor scan(Feature feature, Extension extension, ArtifactProvider provider) throws IOException {
        FeatureDescriptor featureDescriptor = new FeatureDescriptorImpl(feature);
        /*
         * for (Artifact artifact : feature.getBundles()) { URL file =
         * provider.provide(artifact.getId()); BundleDescriptor bundleDescriptor = new
         * BundleDescriptorImpl(artifact, file, artifact.getStartOrder());
         * featureDescriptor.getBundleDescriptors().add(bundleDescriptor); }
         */
        featureDescriptor.lock();

        return featureDescriptor;
    }

}
