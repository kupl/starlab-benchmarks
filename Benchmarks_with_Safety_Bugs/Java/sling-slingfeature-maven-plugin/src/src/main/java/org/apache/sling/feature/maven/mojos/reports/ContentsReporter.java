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

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.ExtensionType;
import org.apache.sling.feature.Feature;

/**
 * Simple reporting mojo
 */
public class ContentsReporter implements Reporter {

    @Override
    public String getName() {
        return "contents";
    }

    @Override
    public void generateReport(final ReportContext ctx) throws MojoExecutionException {
        final Set<ArtifactId> bundles = new TreeSet<>();
        final Set<ArtifactId> artifacts = new TreeSet<>();

        for (final Feature feature : ctx.getFeatures()) {
            for (final Artifact bundle : feature.getBundles()) {
                if ( ctx.matches(bundle.getId())) {
                    bundles.add(bundle.getId());
                }
            }
            for (final Extension ext : feature.getExtensions()) {
                if (ext.getType() == ExtensionType.ARTIFACTS) {
                    for (final Artifact artifact : ext.getArtifacts()) {
                        if ( ctx.matches(artifact.getId())) {
                            artifacts.add(artifact.getId());
                        }
                    }
                }
            }
        }

        final List<String> report = new ArrayList<>();

        report.add("Bundles:");
        report.add("-------------------------------------------");
        bundles.stream().forEach(id -> report.add(id.toMvnId()));
        report.add("");
        report.add("Artifacts:");
        report.add("-------------------------------------------");
        artifacts.stream().forEach(id -> report.add(id.toMvnId()));

        ctx.addReport(this.getName().concat(".txt"), report);
    }
}
