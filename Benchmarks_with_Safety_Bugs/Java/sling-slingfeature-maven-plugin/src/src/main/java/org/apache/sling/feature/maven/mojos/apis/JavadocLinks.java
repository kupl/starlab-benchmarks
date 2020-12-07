/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. The ASF
 * licenses this file to You under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package org.apache.sling.feature.maven.mojos.apis;

import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.maven.mojos.apis.ApisJarContext.ArtifactInfo;

public class JavadocLinks {

    private final Set<String> linkedGlobalPackages = new HashSet<>();

    private final Set<String> docLinks = new HashSet<>();

    public void calculateLinks(final String[] globalJavaDocLinks,
            final Collection<ArtifactInfo> infos,
            final Artifact framework) throws MojoExecutionException {
        final Map<String, Set<String>> linkedPackagesMap = new HashMap<>();
        this.docLinks.clear();
        this.linkedGlobalPackages.clear();
        if ( globalJavaDocLinks != null ) {
            for(final String val : globalJavaDocLinks) {
                docLinks.add(val);
                ApisUtil.getPackageList(val, linkedGlobalPackages, linkedPackagesMap);
            }
        }
        for(final ArtifactInfo info : infos) {
            final List<String> links = ApisUtil.getJavadocLinks(info.getArtifact());
            if ( links != null ) {
                for(final String v : links) {
                    ApisUtil.getPackageList(v, linkedGlobalPackages, linkedPackagesMap);
                    docLinks.add(v);
                }
            }
        }
        if ( framework != null ) {
            final List<String> links = ApisUtil.getJavadocLinks(framework);
            if ( links != null ) {
                for(final String v : links) {
                    ApisUtil.getPackageList(v, linkedGlobalPackages, linkedPackagesMap);
                    docLinks.add(v);
                }
            }
        }
    }

    public Set<String> getJavadocLinks() {
        return this.docLinks;
    }

    public Set<String> getLinkedPackages() {
        return this.linkedGlobalPackages;
    }
}
