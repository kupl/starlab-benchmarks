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

/**
 * Several features can be selected to be put into a single archive
 */
public class Archive extends FeatureSelectionConfig {

    public static final String DEFAULT_EXTENSION = "far";

    private String classifier;

    private String type = DEFAULT_EXTENSION;

    /**
     * If this is set to {@code false} the archive is not added to the project
     * artifacts.
     */
    public boolean attach = true;

    public String getClassifier() {
        return classifier;
    }

    public void setClassifier(String classifier) {
        this.classifier = classifier;
    }

    public String getType() {
        return type;
    }

    public void setType(String value) {
        this.type = value;
    }

    @Override
    public String toString() {
        return "Archive [classifier=" + classifier + ", type=" + type + ", getFilesExcludes()=" + getFilesExcludes() + ", getSelections()="
                + getSelections() + ", attach()=" + attach + "]";
    }
}
