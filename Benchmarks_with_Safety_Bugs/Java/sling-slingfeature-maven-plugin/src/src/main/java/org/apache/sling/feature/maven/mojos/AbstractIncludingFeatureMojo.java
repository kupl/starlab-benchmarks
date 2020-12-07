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

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.TreeMap;

import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.shared.utils.io.DirectoryScanner;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.io.IOUtils;
import org.apache.sling.feature.maven.ProjectHelper;
import org.codehaus.plexus.util.AbstractScanner;

import edu.emory.mathcs.backport.java.util.Arrays;
import edu.emory.mathcs.backport.java.util.Collections;

public abstract class AbstractIncludingFeatureMojo extends AbstractFeatureMojo {

    /**
     * Get all selected features for the provided configuration
     * @param config The selection configuration
     * @return An ordered map of features
     * @throws MojoExecutionException
     */
    protected Map<String, Feature> getSelectedFeatures(final FeatureSelectionConfig config)
            throws MojoExecutionException {
        final Map<String, Feature> result = new LinkedHashMap<>();

        boolean hasFileInclude = false;

        for(final FeatureSelectionConfig.Selection selection : config.getSelections()) {
            switch ( selection.type ) {
            case FILES_INCLUDE:
                hasFileInclude = true;
                selectFeatureFiles(selection.instruction, config.getFilesExcludes(), result);
                break;
            case CLASSIFIER:
                selectFeatureClassifier(selection.instruction, result);
                break;
            case ARTIFACT:
                selectFeatureArtifact(selection.instruction, result);
                break;
            case REFS_INCLUDE:
                selectRefsFiles(selection.instruction, config.getFilesExcludes(), result);
                break;
            default:
                break;
            }
        }

        if (!hasFileInclude && !config.getFilesExcludes().isEmpty()) {
            throw new MojoExecutionException("filesExclude configured without filesInclude in " + config);
        }
        return result;
    }

    /**
     * Select all feature files from the project
     * @return Ordered map of feature files
     * @throws MojoExecutionException
     */
    protected Map<String, Feature> selectAllFeatureFiles() throws MojoExecutionException {
        final FeatureSelectionConfig config = new FeatureSelectionConfig();
        config.setFilesInclude("**/*.*");

        return this.getSelectedFeatures(config);
    }

    /**
     * Select all features including aggregates from the project
     * @return Ordered map of features
     * @throws MojoExecutionException
     */
    protected Map<String, Feature> selectAllFeatureFilesAndAggregates() throws MojoExecutionException {
        final FeatureSelectionConfig config = new FeatureSelectionConfig();
        config.setFilesInclude("**/*.*");
        config.setIncludeClassifier("*");
        return this.getSelectedFeatures(config);
    }

    /**
     * Add the features with the specified classifier
     * @param selection The classifier
     * @param result Map containing the result
     * @throws MojoExecutionException
     */
    private void selectFeatureClassifier(final String selection, final Map<String, Feature> result)
            throws MojoExecutionException {
        final Map<String, Feature> projectFeatures = ProjectHelper.getAssembledFeatures(this.project);
        boolean includeAll = "*".equals(selection);
        boolean found = false;
        for (final Map.Entry<String, Feature> entry : projectFeatures.entrySet()) {
            final String classifier = entry.getValue().getId().getClassifier();
            boolean include = includeAll;
            if (!include) {
                if (selection.trim().length() == 0 && classifier == null) {
                    include = true;
                } else if (selection.equals(classifier)) {
                    include = true;
                }
            }
            if (include) {
                result.put(entry.getKey(), entry.getValue());
                found = true;
            }
        }
        if (!found) {
            throw new MojoExecutionException("Aggregate Classifier " + selection + " not found.");
        }
    }

    /**
     * Add the feature files with the pattern
     * @param include The pattern
     * @param excludes The excludes
     * @param result Map containing the result
     * @throws MojoExecutionException
     */
    private void selectFeatureFiles(final String include, final List<String> excludes,
            final Map<String, Feature> result)
            throws MojoExecutionException {
        final Map<String, Feature> projectFeatures = ProjectHelper.getAssembledFeatures(this.project);

        final String prefix = this.features.toPath().normalize().toFile().getAbsolutePath().concat(File.separator);
        final FeatureScanner scanner = new FeatureScanner(projectFeatures, prefix);
        if (!excludes.isEmpty()) {
            scanner.setExcludes(excludes.toArray(new String[excludes.size()]));
        }
        scanner.setIncludes(new String[] { include });
        scanner.scan();

        if (!include.contains("*") && scanner.getIncluded().isEmpty()) {
            throw new MojoExecutionException("FeatureInclude " + include + " not found.");
        }
        for (Map.Entry<String, Feature> entry : scanner.getIncluded().entrySet()) {
            if (!result.containsKey(entry.getKey())) {
                result.put(entry.getKey(), entry.getValue());
            }
        }

        if (!excludes.isEmpty()) {
            for (final String exclude : excludes) {
                if (!exclude.contains("*")) {
                    final FeatureScanner excludeScanner = new FeatureScanner(projectFeatures, prefix);
                    excludeScanner.setIncludes(new String[] { exclude });
                    excludeScanner.scan();
                    if (excludeScanner.getIncluded().isEmpty()) {
                        throw new MojoExecutionException("FeatureExclude " + exclude + " not found.");
                    }
                }
            }
        }
    }

    /**
     * Add the feature from the ref files with the pattern
     * @param selection The pattern
     * @param excludes The excludes
     * @param result Map containing the result
     * @throws MojoExecutionException
     */
    private void selectRefsFiles(final String selection, final List<String> excludes, final Map<String, Feature> result)
            throws MojoExecutionException {
        final DirectoryScanner scanner = new DirectoryScanner();
        scanner.setBasedir(this.project.getBasedir());
        if (!excludes.isEmpty()) {
            scanner.setExcludes(excludes.toArray(new String[excludes.size()]));
        }
        scanner.setIncludes(new String[] { selection });
        scanner.scan();

        if (!selection.contains("*") && scanner.getIncludedFiles().length == 0 ) {
            throw new MojoExecutionException("RefsInclude " + selection + " not found.");
        }
        // sort result
        @SuppressWarnings("unchecked")
        final List<String> includedFiles = new ArrayList<>(Arrays.asList(scanner.getIncludedFiles()));
        Collections.sort(includedFiles);
        for(final String path : includedFiles) {
            final File selectedFile = new File(this.project.getBasedir(), path.replace('/', File.separatorChar));
            try {
                final List<String> urls = IOUtils.parseFeatureRefFile(selectedFile);
                for(final String url : urls) {
                    try {
                        final ArtifactId id = ArtifactId.parse(url);
                        if (ProjectHelper.isLocalProjectArtifact(this.project, id)) {
                            throw new MojoExecutionException(
                                        "RefsFile configuration is used to select a local feature: " + id.toMvnId());
                        }
                        final Feature feature = ProjectHelper.getOrResolveFeature(this.project, this.mavenSession,
                                this.artifactHandlerManager, this.artifactResolver, id);
                        result.put(id.toMvnUrl(), feature);
                    } catch ( final IllegalArgumentException e) {
                        throw new MojoExecutionException("Reference " + url + " in " + selectedFile.getAbsolutePath() + " is not a supported url.");
                    }
                }
            } catch (final IOException e) {
                throw new MojoExecutionException(e.getMessage(), e);
            }
        }
    }

    private void selectFeatureArtifact(final String artifactId, final Map<String, Feature> result)
            throws MojoExecutionException {
        final ArtifactId id = ArtifactId.parse(artifactId);
        if (ProjectHelper.isLocalProjectArtifact(this.project, id)) {
            throw new MojoExecutionException(
                        "FeatureArtifact configuration is used to select a local feature: " + id.toMvnId());
        }
        final Feature feature = ProjectHelper.getOrResolveFeature(this.project, this.mavenSession,
                this.artifactHandlerManager, this.artifactResolver, id);
        result.put(id.toMvnUrl(), feature);
    }

    public static class FeatureScanner extends AbstractScanner {

        private final Map<String, Feature> features;

        private final Map<String, Feature> included = new TreeMap<>();

        private final String prefix;

        public FeatureScanner(final Map<String, Feature> features, final String prefix) {
            this.features = features;
            this.prefix = prefix;
        }

        @Override
        public void scan() {
            setupDefaultFilters();
            setupMatchPatterns();

            for (Map.Entry<String, Feature> entry : features.entrySet()) {
                // skip aggregates
                if (ProjectHelper.isAggregate(entry.getKey())) {
                    continue;
                }
                final String name = entry.getKey().substring(prefix.length());
                final String[] tokenizedName = tokenizePathToString(name, File.separator);
                if (isIncluded(name, tokenizedName)) {
                    if (!isExcluded(name, tokenizedName)) {
                        included.put(entry.getKey(), entry.getValue());
                    }
                }
            }
        }

        static String[] tokenizePathToString(String path, String separator) {
            List<String> ret = new ArrayList<>();
            StringTokenizer st = new StringTokenizer(path, separator);
            while (st.hasMoreTokens()) {
                ret.add(st.nextToken());
            }
            return ret.toArray(new String[ret.size()]);
        }

        public Map<String, Feature> getIncluded() {
            return this.included;
        }

        @Override
        public String[] getIncludedFiles() {
            return null;
        }

        @Override
        public String[] getIncludedDirectories() {
            return null;
        }

        @Override
        public File getBasedir() {
            return null;
        }
    }
}
