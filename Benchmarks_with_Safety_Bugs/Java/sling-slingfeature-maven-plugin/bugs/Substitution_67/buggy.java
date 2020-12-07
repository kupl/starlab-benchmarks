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
package org.apache.sling.feature.maven;

import java.util.Collections;
import java.util.List;
import java.util.Properties;

import org.apache.maven.artifact.versioning.DefaultArtifactVersion;
import org.apache.maven.project.MavenProject;
import org.codehaus.plexus.interpolation.InterpolationException;
import org.codehaus.plexus.interpolation.ObjectBasedValueSource;
import org.codehaus.plexus.interpolation.PrefixAwareRecursionInterceptor;
import org.codehaus.plexus.interpolation.PrefixedValueSourceWrapper;
import org.codehaus.plexus.interpolation.PropertiesBasedValueSource;
import org.codehaus.plexus.interpolation.RecursionInterceptor;
import org.codehaus.plexus.interpolation.RegexBasedInterpolator;
import org.codehaus.plexus.interpolation.SimpleRecursionInterceptor;

import aQute.bnd.version.MavenVersion;

public class Substitution {

public static java.lang.String replaceMavenVars(org.apache.maven.project.MavenProject project, boolean legacyReplace, boolean replaceProjectProps, java.lang.String[] additionalProperties, java.lang.String s) {
    org.codehaus.plexus.interpolation.RegexBasedInterpolator interpolator = new org.codehaus.plexus.interpolation.RegexBasedInterpolator();
    final org.codehaus.plexus.interpolation.RecursionInterceptor recursionInterceptor;
    if (legacyReplace) {
        project.getProperties().setProperty("project.osgiVersion", org.apache.sling.feature.maven.Substitution.getOSGiVersion(project.getVersion()));
        interpolator.addValueSource(new org.codehaus.plexus.interpolation.PropertiesBasedValueSource(java.lang.System.getProperties()));
        interpolator.addValueSource(new org.codehaus.plexus.interpolation.PropertiesBasedValueSource(project.getProperties()));
        java.util.List<java.lang.String> synonymPrefixes = java.util.Collections.singletonList("project.");
        org.codehaus.plexus.interpolation.PrefixedValueSourceWrapper modelWrapper = new org.codehaus.plexus.interpolation.PrefixedValueSourceWrapper(new org.codehaus.plexus.interpolation.ObjectBasedValueSource(project), synonymPrefixes, true);
        interpolator.addValueSource(modelWrapper);
        recursionInterceptor = new org.codehaus.plexus.interpolation.PrefixAwareRecursionInterceptor(synonymPrefixes, true);
    } else {
        final java.util.Properties props = new java.util.Properties();
        if (replaceProjectProps) {
            props.setProperty("project.groupId", project.getGroupId());
            props.setProperty("project.artifactId", project.getArtifactId());
            props.setProperty("project.version", project.getVersion());
            props.setProperty("project.osgiVersion", org.apache.sling.feature.maven.Substitution.getOSGiVersion(project.getVersion()));
        }
        {
            for (java.lang.String p : /* NPEX_NULL_EXP */
            additionalProperties) {
                p = p.trim();
                if (project.getProperties().containsKey(p)) {
                    props.setProperty(p, project.getProperties().getProperty(p));
                }
            }
        }
        interpolator.addValueSource(new org.codehaus.plexus.interpolation.PropertiesBasedValueSource(props));
        recursionInterceptor = new org.codehaus.plexus.interpolation.SimpleRecursionInterceptor();
    }
    try {
        return interpolator.interpolate(s, recursionInterceptor);
    } catch (org.codehaus.plexus.interpolation.InterpolationException e) {
        throw new java.lang.RuntimeException("An error occurred while interpolating variables to JSON:\n" + s, e);
    }
}

    /**
     * Remove leading zeros for a version part
     */
    private static String cleanVersionString(final String version) {
        final StringBuilder sb = new StringBuilder();
        boolean afterDot = false;
        for(int i=0;i<version.length(); i++) {
            final char c = version.charAt(i);
            if ( c == '.' ) {
                if (afterDot == true ) {
                    sb.append('0');
                }
                afterDot = true;
                sb.append(c);
            } else if ( afterDot && c == '0' ) {
                // skip
            } else if ( afterDot && c == '-' ) {
                sb.append('0');
                sb.append(c);
                afterDot = false;
            } else {
                afterDot = false;
                sb.append(c);
            }
        }
        return sb.toString();
    }

    public static String getOSGiVersion(final String version) {
        final DefaultArtifactVersion dav = new DefaultArtifactVersion(cleanVersionString(version));
        final StringBuilder sb = new StringBuilder();
        sb.append(dav.getMajorVersion());
        sb.append('.');
        sb.append(dav.getMinorVersion());
        sb.append('.');
        sb.append(dav.getIncrementalVersion());
        if ( dav.getQualifier() != null ) {
            sb.append('.');
            sb.append(dav.getQualifier());
        }
        final MavenVersion mavenVersion = new MavenVersion(sb.toString());
        return mavenVersion.getOSGiVersion().toString();
    }
}
