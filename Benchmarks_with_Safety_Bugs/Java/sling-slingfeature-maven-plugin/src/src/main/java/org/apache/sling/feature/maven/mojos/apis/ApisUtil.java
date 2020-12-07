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

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.ArtifactId;

/**
 * Context for creating the api jars
 */
public class ApisUtil {

    /** Tag for source when using SCM info */
    public static final String SCM_TAG = "scm-tag";

    /** Alternative SCM location. */
    public static final String SCM_LOCATION = "scm-location";

    /** Alternative SCM encoding, default is UTF-8 */
    public static final String SCM_ENCODING = "scm-encoding";

    /** Alternative IDs for artifact dependencies. */
    public static final String API_IDS = "api-ids";

    /** Alternative IDS to a source artifact. */
    public static final String SCM_IDS = "source-ids";

    /** Links for javadocs. */
    public static final String JAVADOC_LINKS = "javadoc-links";

    /** Ignore packages for api generation */
    public static final String IGNORE_PACKAGES = "apis-ignore";

    public static List<ArtifactId> getSourceIds(final Artifact artifact) throws MojoExecutionException {
        final String val = artifact.getMetadata().get(SCM_IDS);
        if ( val != null ) {
            final List<ArtifactId> result = new ArrayList<>();
            for(final String v : val.split(",")) {
                try {
                    final ArtifactId sourceId = ArtifactId.parse(v.trim());
                    if ( sourceId.getClassifier() == null ) {
                        throw new MojoExecutionException("Metadata " + SCM_IDS + " must specify classifier for source artifacts : " + sourceId.toMvnId());
                    }
                    result.add(sourceId);
                } catch (final IllegalArgumentException iae) {
                    throw new MojoExecutionException("Wrong format for artifact id : " + v);
                }
            }
            return result;
        }
        return null;
    }

    public static List<ArtifactId> getApiIds(final Artifact artifact) throws MojoExecutionException {
        final String val = artifact.getMetadata().get(API_IDS);
        if ( val != null ) {
            final List<ArtifactId> result = new ArrayList<>();
            for(final String v : val.split(",")) {
                try {
                    final ArtifactId id = ArtifactId.parse(v.trim());
                    result.add(id);
                } catch (final IllegalArgumentException iae) {
                    throw new MojoExecutionException("Wrong format for artifact id : " + v);
                }
            }
            return result;
        }
        return null;
    }

    public static List<String> getJavadocLinks(final Artifact artifact) {
        final String val = artifact.getMetadata().get(JAVADOC_LINKS);
        if ( val != null ) {
            final List<String> result = new ArrayList<>();
            for(String v : val.split(",")) {
                v = v.trim();
                if ( v.endsWith("/") ) {
                    v = v.substring(0, v.length() - 1);
                }
                result.add(v);
            }
            return result;
        }
        return null;
    }

    public static void getPackageList(final String javadocUrl, final Set<String> linkedPackages,
            final Map<String, Set<String>> linkedPackagesMap) throws MojoExecutionException {
        Set<String> result = linkedPackagesMap.get(javadocUrl);
        if ( result == null ) {
            result = new HashSet<>();
            linkedPackagesMap.put(javadocUrl, result);
            try {
                final URL url = new URL(javadocUrl.concat("/package-list"));
                try (final LineNumberReader reader = new LineNumberReader(new InputStreamReader(url.openConnection().getInputStream(), StandardCharsets.UTF_8))) {
                    String line = null;
                    while ( (line = reader.readLine()) != null ) {
                        result.add(line.trim());
                    }
                }
            } catch (final IOException e) {
                throw new MojoExecutionException("Unable to find/read package-list at ".concat(javadocUrl), e);
            }
        }
        result.stream().forEach(v -> linkedPackages.add(v));
    }

    public static Set<String> getIgnoredPackages(final Artifact bundle) {
        final Set<String> result = new HashSet<>();
        final String ignore = bundle.getMetadata().get(ApisUtil.IGNORE_PACKAGES);
        if (ignore != null) {
            for(final String p : ignore.split(",")) {
                result.add(p.trim());
            }
        }
        return result;
    }

}
