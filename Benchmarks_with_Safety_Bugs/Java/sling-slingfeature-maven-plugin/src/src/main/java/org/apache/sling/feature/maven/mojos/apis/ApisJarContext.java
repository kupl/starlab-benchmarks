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

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

import org.apache.felix.utils.manifest.Clause;
import org.apache.maven.model.License;
import org.apache.maven.model.Model;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.logging.Log;
import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.extension.apiregions.api.ApiRegion;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.apache.sling.feature.maven.mojos.selection.IncludeExcludeMatcher;

/**
 * Context for creating the api jars
 */
public class ApisJarContext {

    /**
     * Information about a single artifact (bundle) taking part in the api generation.
     */
    public static final class ArtifactInfo {

        private Artifact artifact;

        private File binDirectory;

        private File sourceDirectory;

        /** Exported packages used by all regions. */
        private Set<String> usedExportedPackages;

        /** Exported packages per region. */
        private final Map<String, Set<Clause>> usedExportedPackagesRegion = new HashMap<>();

        /** Flag if used as dependency */
        private final Map<String, Boolean> useAsDependencyPerRegion = new HashMap<>();

        private final Set<File> includedResources = new HashSet<>();

        private final Set<String> nodeTypes = new HashSet<>();

        private List<License> licenses;

        private final Set<String> sources = new HashSet<>();

        public ArtifactInfo(final Artifact artifact) {
            this.artifact = artifact;
        }

        public ArtifactId getId() {
            return this.artifact.getId();
        }

        public Artifact getArtifact() {
            return this.artifact;
        }

        public File getBinDirectory() {
            return binDirectory;
        }

        public void setBinDirectory(File binDirectory) {
            this.binDirectory = binDirectory;
        }

        public File getSourceDirectory() {
            return sourceDirectory;
        }

        public void setSourceDirectory(File sourceDirectory) {
            this.sourceDirectory = sourceDirectory;
        }

        public Set<String> getUsedExportedPackages() {
            return usedExportedPackages;
        }

        public void setUsedExportedPackages(Set<String> usedExportedPackages) {
            this.usedExportedPackages = usedExportedPackages;
        }

        public String[] getUsedExportedPackageIncludes() {
            final Set<String> includes = new HashSet<>();
            for(final String pck : usedExportedPackages) {
                includes.add(pck.replace('.', '/').concat("/*"));
            }
            return includes.toArray(new String[includes.size()]);
        }

        public Set<Clause> getUsedExportedPackages(final ApiRegion region) {
            return this.usedExportedPackagesRegion.get(region.getName());
        }

        public void setUsedExportedPackages(final ApiRegion region, final Set<Clause> usedExportedPackages, final boolean useAsDependency) {
            this.usedExportedPackagesRegion.put(region.getName(), usedExportedPackages);
            this.useAsDependencyPerRegion.put(region.getName(), useAsDependency);
        }

        public String[] getUsedExportedPackageIncludes(final ApiRegion region) {
            final Set<Clause> clauses = this.getUsedExportedPackages(region);
            final Set<String> includes = new HashSet<>();
            for(final Clause clause : clauses) {
                includes.add(clause.getName().replace('.', '/').concat("/*"));
            }
            return includes.toArray(new String[includes.size()]);
        }

        public boolean isUseAsDependencyPerRegion(final ApiRegion region) {
            return this.useAsDependencyPerRegion.get(region.getName());
        }

        public Set<File> getIncludedResources() {
            return includedResources;
        }

        /**
         * Get all node types from this artifact
         * @return The set of node types, might be empty
         */
        public Set<String> getNodeTypes() {
            return this.nodeTypes;
        }

        public List<License> getLicenses() {
            return licenses;
        }

        public void setLicenses(List<License> licenses) {
            this.licenses = licenses;
        }

        /**
         * Get the dependency artifacts
         * <ol>
         * <li>If {@code ApisUtil#API_IDS} is provided as metadata for the artifact,
         * the value is used as a list of artifacts
         * <li>If {@code ApisUtil#SCM_IDS} is provided as metadata for the artifact,
         * the value is used as a list of artifacts. The artifacts must have a classifier
         * set. The classifier is removed and then the artifacts are used
         * <li>The artifact itself is used
         * </ol>
         * @return The list of dependency artifacts
         * @throws MojoExecutionException
         */
        public List<ArtifactId> getDependencyArtifacts() throws MojoExecutionException {
            final List<ArtifactId> dependencies = new ArrayList<>();
            final List<ArtifactId> apiIds = ApisUtil.getApiIds(artifact);
            if ( apiIds != null ) {
                for(final ArtifactId id : apiIds) {
                    dependencies.add(id);
                }
            } else {
                final List<ArtifactId> sourceIds = ApisUtil.getSourceIds(artifact);
                if ( sourceIds != null ) {
                    for(final ArtifactId id : sourceIds) {
                        dependencies.add(id.changeClassifier(null));
                    }
                } else {
                    dependencies.add(getId());
                }
            }
            return dependencies;
        }

        public void addSourceInfo(final ArtifactId id) {
            if ( id != null ) {
                this.sources.add(id.toMvnId());
            }
        }

        public void addSourceInfo(final String connection) {
            if ( connection != null ) {
                this.sources.add(connection);
            }
        }

        public Set<String> getSources() {
            return this.sources;
        }
    }

    private final Set<String> javadocClasspath = new HashSet<>();

    private final Set<String> packagesWithoutJavaClasses = new HashSet<>();

    private final Set<String> packagesWithoutSources = new HashSet<>();

    private final File deflatedBinDir;

    private final File deflatedSourcesDir;

    private final File checkedOutSourcesDir;

    private File javadocDir;

    private final List<ArtifactInfo> infos = new ArrayList<>();

    private final ArtifactId featureId;

    private final ApiRegions apiRegions;

    private final Map<ArtifactId, Model> modelCache = new HashMap<>();

    private IncludeExcludeMatcher licenseDefaultMatcher;

    /** The set of dependency repositories (URLs) */
    private Set<String> dependencyRepositories = new HashSet<>();

    public ApisJarContext(final File mainDir, final ArtifactId featureId, final ApiRegions regions) {
        this.featureId = featureId;

        // deflated and source dirs can be shared
        this.deflatedBinDir = new File(mainDir, "deflated-bin");
        this.deflatedSourcesDir = new File(mainDir, "deflated-sources");
        this.checkedOutSourcesDir = new File(mainDir, "checkouts");
        this.apiRegions = regions;
    }

    public ArtifactId getFeatureId() {
        return featureId;
    }

    public ApiRegions getApiRegions() {
        return this.apiRegions;
    }

    public File getDeflatedBinDir() {
        return deflatedBinDir;
    }

    public File getDeflatedSourcesDir() {
        return deflatedSourcesDir;
    }

    public File getCheckedOutSourcesDir() {
        return checkedOutSourcesDir;
    }

    public boolean addJavadocClasspath(final String classpathItem) {
        return javadocClasspath.add(classpathItem);
    }

    public Set<String> getJavadocClasspath() {
        return javadocClasspath;
    }

    public File getJavadocDir() {
        return javadocDir;
    }

    public void setJavadocDir(final File javadocDir) {
        this.javadocDir = javadocDir;
    }

    public Set<String> getPackagesWithoutJavaClasses() {
        return packagesWithoutJavaClasses;
    }

    public Set<String> getPackagesWithoutSources() {
        return packagesWithoutSources;
    }

    public ArtifactInfo addArtifactInfo(final Artifact artifact) {
        final ArtifactInfo info = new ArtifactInfo(artifact);
        this.infos.add(info);

        return info;
    }

    public List<ArtifactInfo> getArtifactInfos() {
        return this.infos;
    }

    public Map<ArtifactId, Model> getModelCache() {
        return this.modelCache;
    }

    public Collection<ArtifactInfo> getArtifactInfos(final ApiRegion region, final boolean omitDependencyArtifacts) {
        final Map<ArtifactId, ArtifactInfo> result = new TreeMap<>();
        for(final ArtifactInfo info : this.infos) {
            if ( !info.getUsedExportedPackages(region).isEmpty() ) {
                if ( !omitDependencyArtifacts || !info.isUseAsDependencyPerRegion(region) ) {
                    result.put(info.getId(), info);
                }
            }
        }
        return result.values();
    }


    public void setLicenseDefaults(final List<String> licenseDefaults) throws MojoExecutionException {
        this.licenseDefaultMatcher = new IncludeExcludeMatcher(licenseDefaults, null, "=", true);
    }

    public String getLicenseDefault(final ArtifactId id) {
        return this.licenseDefaultMatcher.matches(id);
    }

    /**
     * Set the dependency repositories
     * @param list Comma separated list or {@code null}
     */
    public void setDependencyRepositories(final String list) {
        this.dependencyRepositories.clear();
        if ( list != null ) {
            for(String val : list.split(",") ) {
                val = val.trim();
                if ( !val.endsWith("/") ) {
                    val = val.concat("/");
                }
                this.dependencyRepositories.add(val);
            }
        }
    }

    /**
     * Find a an artifact
     * If dependency repositories are configured, one of them must provide the artifact
     * @param log Logger
     * @param id The artifact id
     * @return {@code true} if the artifact could be found.
     * @throws MojoExecutionException
     */
    private boolean findDependencyArtifact(final Log log, final ArtifactId id) throws MojoExecutionException {
        boolean result = true;
        if ( !this.dependencyRepositories.isEmpty() ) {
            result = false;
            log.debug("Trying to resolve ".concat(id.toMvnId()).concat(" from ").concat(this.dependencyRepositories.toString()));
            for(final String server : this.dependencyRepositories) {
                try {
                    final URL url = new URL(server.concat(id.toMvnPath()));
                    try {
                        url.openConnection().getInputStream().close();
                        log.debug("Found ".concat(id.toMvnId()).concat(" at ").concat(url.toString()));
                        result = true;
                        break;
                    } catch (IOException e) {
                        // not available
                        log.debug("Missed ".concat(id.toMvnId()).concat(" at ").concat(url.toString()).concat(" : ").concat(e.toString()));
                    }
                } catch ( final MalformedURLException mue) {
                    throw new MojoExecutionException("Unable to find dependency on ".concat(server), mue);
                }
            }
        }
        return result;
    }

    /**
     * Check if all dependency artifacts can be found
     * @param log The logger
     * @param info The artifact info
     * @return {@code true} if all artifacts are publically available
     * @throws MojoExecutionException
     */
    public boolean findDependencyArtifact(final Log log, final ArtifactInfo info) throws MojoExecutionException {
        boolean result = true;
        for(final ArtifactId id : info.getDependencyArtifacts()) {
            if ( !findDependencyArtifact(log, id) ) {
                result = false;
                break;
            }
        }
        return result;
    }
}
