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
package org.apache.sling.feature.maven.mojos.reports;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.scanner.BundleDescriptor;
import org.apache.sling.feature.scanner.FeatureDescriptor;
import org.apache.sling.feature.scanner.PackageInfo;

public class ExportPackagesReporter implements Reporter {

    @Override
    public String getName() {
        return "exported-packages";
    }

    @Override
    public void generateReport(final ReportContext ctx) throws MojoExecutionException {
        for(final Feature feature : ctx.getFeatures()) {
            FeatureDescriptor fd;
            try {
                fd = ctx.getScanner().scan(feature);
            } catch (final IOException e) {
                throw new MojoExecutionException("Unable to scan feature " + e.getMessage(), e);
            }

            final List<String> exportedPackages = this.getExportedPackages(fd);

            if ( !exportedPackages.isEmpty() ) {
                ctx.addReport(fd.getFeature().getId().changeType("packages.txt").toMvnName(), exportedPackages);
            }
        }
    }

    private List<String> getExportedPackages(final FeatureDescriptor fd) {
        final List<String> packages = new ArrayList<>();

        for (final BundleDescriptor bd : fd.getBundleDescriptors()) {
            for (PackageInfo p : bd.getExportedPackages()) {
                packages.add(p.getName());
            }
        }

        Collections.sort(packages);
        return packages;
    }

}
