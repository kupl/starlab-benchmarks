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
package org.apache.sling.feature.maven.mojos.selection;

import java.util.ArrayList;
import java.util.List;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.sling.feature.ArtifactId;

public class IncludeExcludeMatcher {

    private List<String[]> includeMatches;

    private List<String[]> excludeMatches;

    public IncludeExcludeMatcher(final List<String> incudes,
            final List<String> excludes,
            final String separator,
            final boolean separatorRequired) throws MojoExecutionException {
        this.includeMatches = parseMatches(incudes, separator, separatorRequired);
        this.excludeMatches = parseMatches(excludes, null, false);
    }

    public String matches(final ArtifactId id) {
        String result = null;
        if ( this.includeMatches != null ) {
            result = this.match(this.includeMatches, id);
            if ( this.excludeMatches != null && this.match(this.excludeMatches, id) != null) {
                result = null;
            }
        }
        return result;
    }

    private List<String[]> parseMatches(final List<String> patterns,
            final String separator,
            final boolean separatorRequired) throws MojoExecutionException {
        List<String[]> matches = null;
        if (patterns != null && !patterns.isEmpty()) {
            matches = new ArrayList<>();
            for (final String t : patterns) {
                final String[] parts;
                if (separator == null ) {
                    parts = new String[] {t, ""};
                } else if (t.endsWith(separator)) {
                    parts = new String[] {t.substring(0, t.length() - 1), ""};
                } else if ( t.contains(separator) ) {
                    parts = t.split(separator);
                } else if ( separatorRequired ) {
                    throw new MojoExecutionException("Illegal pattern: " + t);
                } else {
                    parts = new String[] {t, ""};
                }
                if ( parts.length != 2 ) {
                    throw new MojoExecutionException("Illegal pattern: " + t);
                }
                final String[] val = parts[0].split(":");
                if (val.length > 5) {
                    throw new MojoExecutionException("Illegal pattern: " + t);
                }
                final String[] result = new String[val.length + 1];
                System.arraycopy(val, 0, result, 1, val.length);
                result[0] = parts[1];
                matches.add(result);
            }
        }
        return matches;
    }

    private boolean match(final String value, final String matcher) {
        if (matcher.endsWith("*")) {
            return value.startsWith(matcher.substring(0, matcher.length() - 1));
        }
        return matcher.equals(value);
    }

    private String match(final List<String[]> matches, final ArtifactId id) {
        boolean match = false;

        for(final String[] m : matches) {
            match = match(id.getGroupId(), m[1]);
            if (match && m.length > 2) {
                match = match(id.getArtifactId(), m[2]);
            }
            if (match && m.length == 4) {
                match = match(id.getVersion(), m[3]);
            } else if (match && m.length == 5) {
                match = match(id.getVersion(), m[4]);
                if (match) {
                    match = match(id.getType(), m[3]);
                }
            } else if (match && m.length == 6) {
                match = match(id.getVersion(), m[5]);
                if (match) {
                    match = match(id.getType(), m[3]);
                    if (match) {
                        match = match(id.getClassifier(), m[4]);
                    }
                }
            }
            if (match) {
                return m[0];
            }
        }

        return null;
    }
}
