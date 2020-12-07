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

import java.util.ArrayList;
import java.util.List;

import org.apache.maven.model.Dependency;
import org.apache.sling.feature.maven.ProjectHelper;

public class FeatureSelectionConfig {

    enum SelectionType {
        FILES_INCLUDE, CLASSIFIER, ARTIFACT, REFS_INCLUDE
    }

    static class Selection {
        public final SelectionType type;
        public final String instruction;

        public Selection(final SelectionType type, final String instruction) {
            this.type = type;
            this.instruction = instruction;
        }

        @Override
        public String toString() {
            return "Selection [type=" + type + ", instruction=" + instruction + "]";
        }
    }

    private final List<Selection> selections = new ArrayList<>();

    private final List<String> filesExcludes = new ArrayList<>();

    public FeatureSelectionConfig() {
    }

    public void setFilesInclude(final String val) {
        selections.add(new Selection(SelectionType.FILES_INCLUDE, val));
    }

    public void setFilesExclude(final String val) {
        this.filesExcludes.add(val);
    }

    /**
     * Include refs files
     * @param val A pattern
     * @since 1.1.22
     */
    public void setRefsInclude(final String val) {
        selections.add(new Selection(SelectionType.REFS_INCLUDE, val));
    }

    public void setIncludeArtifact(final Dependency a) {
        selections.add(new Selection(SelectionType.ARTIFACT, ProjectHelper.toArtifactId(a).toMvnId()));
    }

    public void setIncludeClassifier(final String classifier) {
        selections.add(new Selection(SelectionType.CLASSIFIER, ":".equals(classifier) ? "" : classifier));
    }

    public List<String> getFilesExcludes() {
        return this.filesExcludes;
    }

    public List<Selection> getSelections() {
        return this.selections;
    }

    @Override
    public String toString() {
        return "FeatureSelectionConfig [selections=" + selections + ", filesExcludes=" + filesExcludes + "]";
    }
}