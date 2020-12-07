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
package org.apache.sling.feature.maven.mojos;

import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.apache.sling.feature.ArtifactId;

public class Aggregate extends FeatureSelectionConfig {

    /**
     * This is the classifier for the new feature. If not specified the feature is
     * the main artifact for the project.
     */
    public String classifier;

    /**
     * If this is set to {@code false} the feature is not added to the project
     * artifacts.
     */
    public boolean attach = true;

    /**
     * If this is set to {@code true} the feature is marked as final.
     */
    public boolean markAsFinal = false;

    /**
     * If this is set to {@code true} the feature is marked as complete.
     */
    public boolean markAsComplete = false;

    /**
     * Optional title for the feature
     */
    public String title;

    /**
     * Optional description for the feature
     */
    public String description;

    /**
     * Optional vendor for the feature
     */
    public String vendor;

    public List<String> artifactsOverrides;

    public List<String> configurationOverrides;

    public Map<String, String> variablesOverrides;

    public Map<String, String> frameworkPropertiesOverrides;

    @Override
    public String toString() {
        return "Aggregate [selection=" + getSelections() + ", filesExcludes=" + getFilesExcludes()
                + ", classifier=" + classifier + ", attach=" + attach
                + ", markAsFinal=" + markAsFinal + ", markAsComplete=" + markAsComplete + ", title=" + title
                + ", description=" + description + ", vendor=" + vendor + ", artifactsOverrides=" + artifactsOverrides
                + ", variablesOverrides=" + variablesOverrides + ", frameworkPropertiesOverrides=" + frameworkPropertiesOverrides + "]";
    }

    public List<ArtifactId> getArtifactOverrideRules() {
        if (artifactsOverrides == null) {
            return Collections.emptyList();
        }
        return artifactsOverrides.stream().map(r -> ArtifactId.parse(r)).collect(Collectors.toList());
    }

    public Map<String, String> getConfigurationOverrideRules() {
        if (configurationOverrides == null) {
            return Collections.emptyMap();
        }
        LinkedHashMap<String, String> result = new LinkedHashMap<>();
        for (String entry : configurationOverrides) {
            int idx = entry.lastIndexOf("=");
            if (idx != -1) {
                result.put(entry.substring(0, idx), entry.substring(idx + 1));
            }
        }
        return result;
    }
}