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
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.Configuration;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.ExtensionType;
import org.apache.sling.feature.Feature;

public class DuplicatesReporter implements Reporter {

    @Override
    public String getName() {
        return "duplicates";
    }

    @Override
    public void generateReport(ReportContext ctx) throws MojoExecutionException {
        final Map<String, Set<String>> artifactMap = new TreeMap<>();
        final Map<String, Set<String>> bundleMap = new TreeMap<>();
        final Map<String, Set<String>> configMap = new TreeMap<>();
        final Map<String, Set<String>> propsMap = new TreeMap<>();

        for(final Feature feature : ctx.getFeatures()) {
            for(final Extension ext : feature.getExtensions()) {
                if ( ext.getType() == ExtensionType.ARTIFACTS ) {
                    for(final Artifact a : ext.getArtifacts()) {
                        final String key = a.getId().changeVersion("0").toMvnId();
                        artifactMap.computeIfAbsent(key, k -> new HashSet<>()).add(feature.getId().getClassifier().concat("(").concat(a.getId().getVersion()).concat(")"));
                    }
                }
            }

            for(final Artifact a : feature.getBundles()) {
                final String key = a.getId().changeVersion("0").toMvnId();
                bundleMap.computeIfAbsent(key, k -> new HashSet<>()).add(feature.getId().getClassifier().concat("(").concat(a.getId().getVersion()).concat(")"));
            }

            for(final Configuration c : feature.getConfigurations()) {
                configMap.computeIfAbsent(c.getPid(), k -> new HashSet<>()).add(feature.getId().getClassifier());
            }

            for(final String a : feature.getFrameworkProperties().keySet()) {
                propsMap.computeIfAbsent(a, k -> new HashSet<>()).add(feature.getId().getClassifier());
            }
        }
        final List<String> output = new ArrayList<>();
        outputDuplicates(output, "framework-properties", propsMap);
        outputDuplicates(output, "bundles", bundleMap);
        outputDuplicates(output, "configurations", configMap);
        outputDuplicates(output, "artifacts", artifactMap);
        if ( output.isEmpty()) {
            output.add("No duplicates found");
        }
        ctx.addReport(this.getName().concat(".txt"), output);
    }

    private void outputDuplicates(final List<String> output, final String key, final Map<String, Set<String>> duplicates) {
        boolean writeHeader;
        if ( !duplicates.isEmpty() ) {
            writeHeader = true;
            for(final Map.Entry<String, Set<String>> entry : duplicates.entrySet()) {
                if ( entry.getValue().size() > 1 ) {
                    if ( writeHeader ) {
                        writeHeader = false;
                        output.add(key);
                        output.add("-------------------------------------------");
                    }
                    output.add(entry.getKey().concat(" : ").concat(entry.getValue().toString()));
                }
            }
            if ( !writeHeader ) {
                output.add("");
            }
        }
    }
}
