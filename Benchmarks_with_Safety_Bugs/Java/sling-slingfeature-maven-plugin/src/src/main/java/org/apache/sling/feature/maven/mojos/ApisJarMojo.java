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
package org.apache.sling.feature.maven.mojos;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.TreeSet;
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;
import java.util.jar.Manifest;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import javax.json.JsonArray;

import org.apache.commons.lang3.JavaVersion;
import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.SystemUtils;
import org.apache.felix.utils.manifest.Clause;
import org.apache.felix.utils.manifest.Parser;
import org.apache.maven.archiver.MavenArchiveConfiguration;
import org.apache.maven.archiver.MavenArchiver;
import org.apache.maven.artifact.resolver.ArtifactResolutionException;
import org.apache.maven.artifact.resolver.ArtifactResolutionRequest;
import org.apache.maven.artifact.resolver.ArtifactResolutionResult;
import org.apache.maven.artifact.resolver.filter.ArtifactFilter;
import org.apache.maven.model.License;
import org.apache.maven.model.Model;
import org.apache.maven.model.Scm;
import org.apache.maven.model.building.ModelBuilder;
import org.apache.maven.model.building.ModelBuildingRequest;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.Component;
import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.plugins.annotations.ResolutionScope;
import org.apache.maven.repository.RepositorySystem;
import org.apache.maven.scm.ScmException;
import org.apache.maven.scm.ScmFileSet;
import org.apache.maven.scm.ScmTag;
import org.apache.maven.scm.ScmVersion;
import org.apache.maven.scm.command.checkout.CheckOutScmResult;
import org.apache.maven.scm.manager.NoSuchScmProviderException;
import org.apache.maven.scm.manager.ScmManager;
import org.apache.maven.scm.repository.ScmRepository;
import org.apache.maven.scm.repository.ScmRepositoryException;
import org.apache.maven.shared.utils.io.DirectoryScanner;
import org.apache.maven.shared.utils.io.FileUtils;
import org.apache.maven.shared.utils.logging.MessageUtils;
import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.ExecutionEnvironmentExtension;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.Extensions;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.builder.ArtifactProvider;
import org.apache.sling.feature.extension.apiregions.api.ApiExport;
import org.apache.sling.feature.extension.apiregions.api.ApiRegion;
import org.apache.sling.feature.extension.apiregions.api.ApiRegions;
import org.apache.sling.feature.io.IOUtils;
import org.apache.sling.feature.maven.ProjectHelper;
import org.apache.sling.feature.maven.mojos.apis.ApisJarContext;
import org.apache.sling.feature.maven.mojos.apis.ApisJarContext.ArtifactInfo;
import org.apache.sling.feature.maven.mojos.apis.ApisUtil;
import org.apache.sling.feature.maven.mojos.apis.JavadocExecutor;
import org.apache.sling.feature.maven.mojos.apis.JavadocLinks;
import org.codehaus.plexus.archiver.UnArchiver;
import org.codehaus.plexus.archiver.jar.JarArchiver;
import org.codehaus.plexus.archiver.manager.ArchiverManager;
import org.codehaus.plexus.archiver.manager.NoSuchArchiverException;
import org.codehaus.plexus.archiver.util.DefaultFileSet;
import org.codehaus.plexus.components.io.fileselectors.FileSelector;
import org.codehaus.plexus.components.io.fileselectors.IncludeExcludeFileSelector;
import org.osgi.framework.Constants;

/**
 * Generates the APIs JARs for the selected feature files.
 */
@Mojo(name = "apis-jar",
    defaultPhase = LifecyclePhase.PACKAGE,
    requiresDependencyResolution = ResolutionScope.TEST,
    threadSafe = true
)
public class ApisJarMojo extends AbstractIncludingFeatureMojo {

    public enum ArtifactType {
        APIS("apis", "class", "jar"),
        SOURCES("sources", "java", "jar"),
        JAVADOC("javadoc", "html", "jar"),
        DEPENDENCIES("apideps", "txt", "ref"),
        CND("cnd", "cnd", "jar"),
        REPORT("report", "txt", "txt");

        private final String id;

        private final String type;

        private final String extension;

        ArtifactType(final String id, final String type, final String extension) {
            this.id = id;
            this.type = type;
            this.extension = extension;
        }

        public String getId() {
            return this.id;
        }

        public String getContentType() {
            return this.type;
        }

        public String getContentExtension() {
            return ".".concat(this.type);
        }

        public String getExtension() {
            return this.extension;
        }
    }

    /**
     * Select the features for api generation.
     * Separate api jars will be generated for each feature.
     */
    @Parameter
    private FeatureSelectionConfig selection;

    /**
     * Patterns identifying which resources to include from bundles.
     * This can be used to include files like license or notices files.
     * Starting with version 1.2.0 these files are only searched
     * in the folders mentioned by {@code #resourceFolders}
     */
    @Parameter
    private String[] includeResources;

    /**
     * Names of the regions to include, by default all regions are included.
     */
    @Parameter(defaultValue = "*")
    private Set<String> includeRegions;

    /**
     * Names of the regions to exclude, by default no regions is excluded.
     */
    @Parameter
    private Set<String> excludeRegions;

    /**
     * List of javadoc links used in the javadoc generation.
     */
    @Parameter
    private String[] javadocLinks;

    /**
     * Ignore errors in javadoc generation
     */
    @Parameter(defaultValue = "false")
    private boolean ignoreJavadocErrors;

    /**
     * If set to true and api jars are created for more than one region, then the
     * higher region only gets the difference to the lower region. If set to false
     * each api jar gets the full region information (duplicating information)
     */
    @Parameter(defaultValue = "true")
    private boolean incrementalApis;

    /**
     * Additional resources for the api jar
     */
    @Parameter
    private List<File> apiResources;

    /**
     * Additional resources for the api source jar
     */
    @Parameter
    private List<File> apiSourceResources;

    /**
     * Additional resources for the api javadoc jar
     */
    @Parameter
    private List<File> apiJavadocResources;

    /**
     * If enabled, the created api jars will be attached to the project
     */
    @Parameter(defaultValue = "true")
    private boolean attachApiJars;

    /**
     * Mapping for an api region name to a user defined name
     */
    @Parameter
    private Map<String, String> apiRegionNameMappings;

    /**
     * Mapping for the feature classifier to a user defined name
     */
    @Parameter
    private Map<String, String> apiClassifierMappings;

    /**
     * Generate api jar
     */
    @Parameter(defaultValue = "true")
    private boolean generateApiJar;

    /**
     * Generate the sources jar
     */
    @Parameter(defaultValue = "true")
    private boolean generateSourceJar;

    /**
     * Generate the javadoc jar
     */
    @Parameter(defaultValue = "true")
    private boolean generateJavadocJar;

    /**
     * Source level for javadoc generation
     */
    @Parameter(defaultValue = "8")
    private String javadocSourceLevel;

    /**
     * Optional version to be put into the manifest of the created jars
     * @since 1.2.0
     */
    @Parameter
    private String apiVersion;

    /**
     * Comma separated list of folders where files are renamed.
     * @since 1.2.0
     */
    @Parameter(defaultValue = "META-INF,SLING-INF")
    private String resourceFolders;

    /**
     * Create a license report file. This is the name of that file within the jar
     * @since 1.2.0
     */
    @Parameter
    private String licenseReport;

    /**
     * A artifact patterns to match artifacts without a license. Follows the pattern
     * "groupId:artifactId:type:classifier:version". After the patter a "=" followed by
     * the license information needs to be specified. This information is used in the
     * license report if no license is specified for an artifact.
     * @since 1.2.0
     */
    @Parameter
    private List<String> licenseDefaults;

    /**
     * Header added on top of the license report
     * @since 1.2.0
     */
    @Parameter(defaultValue = "This archive contains files from the following artifacts:")
    private String licenseReportHeader;

    /**
     * Footer added at the bottom of the license report
     * @since 1.2.0
     */
    @Parameter
    private String licenseReportFooter;

    /**
     * If enabled, packages from artifacts which are fully consumed (all public api) are omitted from the api and source jars
     * and a dependency list is generated instead.
     * @since 1.3.0
     */
    @Parameter(defaultValue = "false")
    private boolean useApiDependencies;

    /**
     * Comma separated list of Maven repository lists. If set, and {@link #useApiDependencies} is enabled,
     * then one of the listed repositories must provide the artifact.
     * @since 1.3.0
     */
    @Parameter
    private String apiRepositoryUrls;

    /**
     * Fail the build if errors are detected. For example, errors could be missing packages in the api jars, or
     * too many packages in those jars.
     * @since 1.3.2
     */
    @Parameter(defaultValue = "false")
    private boolean failOnError;

    /**
     * specify the manifest properties values that you need to replace in the Manifest file.
     * @since 1.3.2
     */
    @Parameter
    private final Properties manifestProperties = new Properties();

    /**
     * Fail the build if sources are mising for javadoc generation
     * @since 1.3.6
     */
    @Parameter(defaultValue = "false")
    private boolean failOnMissingSourcesForJavadoc;

    /**
     * Whether the index should be generated
     * @since 1.3.6
     */
    @Parameter(defaultValue = "true")
    private boolean javadocIndex;

    /**
     * Whether the tree should be generated
     * @since 1.3.6
     */
    @Parameter(defaultValue = "true")
    private boolean javadocTree;

    @Parameter(defaultValue = "${project.build.directory}/apis-jars", readonly = true)
    private File mainOutputDir;

    @Component(hint = "default")
    private ModelBuilder modelBuilder;

    @Component
    private ScmManager scmManager;

    @Component
    private ArchiverManager archiverManager;

    @Component
    private RepositorySystem repositorySystem;

    private final Pattern pomPropertiesPattern = Pattern.compile("META-INF/maven/[^/]+/[^/]+/pom.properties");

    /** Artifact Provider. */
    private final ArtifactProvider artifactProvider = new BaseArtifactProvider();

    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        checkPreconditions();

        getLog().debug("Retrieving Feature files...");
        final Collection<Feature> features = this.getSelectedFeatures(selection).values();

        if (features.isEmpty()) {
            getLog().info(
                    "There are no assciated Feature files in the current project, plugin execution will be skipped");
        } else {
            getLog().debug("Starting APIs JARs creation...");

            for (final Feature feature : features) {
                onFeature(feature);
            }
        }
    }

    /**
     * Apply region name mapping if configured
     *
     * @param regionName The region name
     * @return The mapped name or the original name
     */
    private String mapApiRegionName(final String regionName) {
        if (this.apiRegionNameMappings != null && this.apiRegionNameMappings.containsKey(regionName)) {
            return this.apiRegionNameMappings.get(regionName);
        }
        return regionName;
    }

    /**
     * Apply classifier mapping if configured
     *
     * @param classifier The classifier
     * @return The mapped classifier or the original classifier
     */
    private String mapApiClassifier(final String classifier) {
        if (this.apiClassifierMappings != null && this.apiClassifierMappings.containsKey(classifier)) {
            return this.apiClassifierMappings.get(classifier);
        }
        return classifier;
    }

    /**
     * Check if the region is included
     *
     * @param name The region name
     * @return {@code true} if the region is included
     */
    private boolean isRegionIncluded(final String name) {
        boolean included = false;
        for (final String i : this.includeRegions) {
            if ("*".equals(i) || i.equals(name)) {
                included = true;
                break;
            }
        }
        if (included && this.excludeRegions != null) {
            for (final String e : this.excludeRegions) {
                if (name.equals(e)) {
                    included = false;
                    break;
                }
            }
        }

        return included;
    }

    /**
     * Get the api regions for a feature If the feature does not have an api region
     * an artificial global region is returned.
     *
     * @param feature The feature
     * @return The api regions or {@code null} if the feature is wrongly configured
     *         or all regions are excluded
     * @throws MojoExecutionException If an error occurs
     */
    private ApiRegions getApiRegions(final Feature feature) throws MojoExecutionException {
        ApiRegions regions = new ApiRegions();

        Extensions extensions = feature.getExtensions();
        Extension apiRegionsExtension = extensions.getByName(ApiRegions.EXTENSION_NAME);
        if (apiRegionsExtension != null) {
            if (apiRegionsExtension.getJSONStructure() == null) {
                getLog().info(
                        "Feature file " + feature.getId().toMvnId() + " declares an empty '" + ApiRegions.EXTENSION_NAME
                    + "' extension, no API JAR will be created");
                regions = null;
            } else {
                ApiRegions sourceRegions;
                try {
                    sourceRegions = ApiRegions
                            .parse((JsonArray) apiRegionsExtension.getJSONStructure());
                } catch (final IOException ioe) {
                    throw new MojoExecutionException(ioe.getMessage(), ioe);
                }

                // calculate all api-regions first, taking the inheritance in account
                for (final ApiRegion r : sourceRegions.listRegions()) {
                    if (r.getParent() != null && !this.incrementalApis) {
                        for (final ApiExport exp : r.getParent().listExports()) {
                            r.add(exp);
                        }
                    }
                    if (isRegionIncluded(r.getName())) {
                        getLog().debug("API Region " + r.getName()
                                    + " will not processed due to the configured include/exclude list");
                        regions.add(r);
                    }
                }

                if (regions.isEmpty()) {
                    getLog().info("Feature file " + feature.getId().toMvnId()
                            + " has no included api regions, no API JAR will be created");
                    regions = null;
                }
            }
        } else {
            // create exports on the fly
            regions.add(new ApiRegion(ApiRegion.GLOBAL) {

                @Override
                public ApiExport getExportByName(final String name) {
                    ApiExport exp = super.getExportByName(name);
                    if (exp == null) {
                        exp = new ApiExport(name);
                        this.add(exp);
                    }
                    return exp;
                }
            });
        }

        return regions;
    }

    /**
     * Create api jars for a feature
     */
    private void onFeature(final Feature feature)
            throws MojoExecutionException {
        getLog().info(MessageUtils.buffer().a("Creating API JARs for Feature ").strong(feature.getId().toMvnId())
                .a(" ...").toString());

        final ApiRegions regions = getApiRegions(feature);
        if (regions == null) {
            // wrongly configured api regions - skip execution, info is logged already so we can just return
            return;
        }

        // create an output directory per feature
        final File featureDir = new File(mainOutputDir, feature.getId().getArtifactId());
        final ApisJarContext ctx = new ApisJarContext(this.mainOutputDir, feature.getId(), regions);
        ctx.setLicenseDefaults(licenseDefaults);
        ctx.setDependencyRepositories(this.apiRepositoryUrls);

        // for each bundle included in the feature file and record directories
        for (final Artifact artifact : feature.getBundles()) {
            onArtifact(ctx, artifact);
        }

        if ( this.generateSourceJar || this.generateJavadocJar ) {
            getLog().info("--------------------------------------------------------");
            getLog().info("Used sources:");
            for(final ArtifactInfo info : ctx.getArtifactInfos()) {
                if ( info.getSources().isEmpty()) {
                    getLog().info("- ".concat(info.getId().toMvnId()).concat(" : NO SOURCES FOUND"));
                } else {
                    getLog().info("- ".concat(info.getId().toMvnId()).concat(" : ").concat(info.getSources().toString()));
                }
            }
            getLog().info("--------------------------------------------------------");
        }

        boolean hasErrors = false;

        // recollect and package stuff per region
        for (final ApiRegion apiRegion : regions.listRegions()) {
            final List<String> report = new ArrayList<>();

            final File regionDir = new File(featureDir, apiRegion.getName());

            if (generateApiJar) {
                final File apiJar = createArchive(ctx, apiRegion, ArtifactType.APIS, this.apiResources,
                        ctx.getArtifactInfos(apiRegion, this.useApiDependencies), report);
                report(ctx, apiJar, ArtifactType.APIS, apiRegion, this.useApiDependencies, report, null);
            }

            if (generateSourceJar) {
                final File sourceJar = createArchive(ctx, apiRegion, ArtifactType.SOURCES, this.apiSourceResources,
                        ctx.getArtifactInfos(apiRegion, this.useApiDependencies), report);
                report(ctx, sourceJar, ArtifactType.SOURCES, apiRegion, this.useApiDependencies, report, null);
            }

            if ( this.useApiDependencies && (this.generateApiJar || this.generateSourceJar)) {
               this.createDependenciesFile(ctx, apiRegion);
            }

            if (generateJavadocJar) {
                final File javadocsDir = new File(regionDir, ArtifactType.JAVADOC.getId());
                final ExecutionEnvironmentExtension ext = ExecutionEnvironmentExtension.getExecutionEnvironmentExtension(feature);
                final JavadocLinks links = new JavadocLinks();
                links.calculateLinks(this.javadocLinks, ctx.getArtifactInfos(apiRegion, false), ext != null ? ext.getFramework() : null);

                final Collection<ArtifactInfo> infos = generateJavadoc(ctx, apiRegion, links, javadocsDir);
                if ( infos != null ) {
                    ctx.setJavadocDir(javadocsDir);
                    final File javadocJar = createArchive(ctx, apiRegion, ArtifactType.JAVADOC, this.apiJavadocResources, infos, report);
                    report(ctx, javadocJar, ArtifactType.JAVADOC, apiRegion, false, report, links);
                } else {
                    getLog().warn("Javadoc JAR will NOT be generated - sources directory " + ctx.getDeflatedSourcesDir()
                            + " was empty or contained no Java files!");
                }
            }

            final ArtifactId reportId = this.buildArtifactId(ctx, apiRegion, ArtifactType.REPORT);
            final File reportFile = new File(mainOutputDir, reportId.toMvnName());
            if ( !report.isEmpty() ) {
                report.stream().forEach(v -> getLog().info(v));
                try {
                    Files.write(reportFile.toPath(), report);
                } catch (final IOException e) {
                    throw new MojoExecutionException("Unable to write " + reportFile, e);
                }
                hasErrors = true;
            } else {
                if ( reportFile.exists()) {
                    reportFile.delete();
                }
            }
        }

        if ( hasErrors && this.failOnError ) {
            throw new MojoExecutionException("API generation has errors, please see report files for more information");
        }

        getLog().info(MessageUtils.buffer().a("APIs JARs for Feature ").debug(feature.getId().toMvnId())
                .a(" succesfully created").toString());
    }

    private void report(final ApisJarContext ctx,
            final File jarFile,
            final ArtifactType artifactType,
            final ApiRegion apiRegion,
            final boolean omitDependencyArtifacts,
            final List<String> report,
            final JavadocLinks links)
    throws MojoExecutionException {
        final Map.Entry<Set<String>, Set<String>> packageResult = getPackages(jarFile, artifactType.getContentExtension());
        final Set<String> apiPackages = packageResult.getKey();
        final Set<String> otherPackages = packageResult.getValue();
        if ( omitDependencyArtifacts ) {
            for(final ArtifactInfo info : ctx.getArtifactInfos(apiRegion, false)) {
                if ( info.isUseAsDependencyPerRegion(apiRegion) ) {
                    for(final Clause c : info.getUsedExportedPackages(apiRegion) ) {
                        apiPackages.add(c.getName());
                    }
                }
            }
        }
        // make sure no reports for packages not containing java classes
        otherPackages.addAll(ctx.getPackagesWithoutJavaClasses());
        // ignore packages without sources for javadoc?
        if ( artifactType == ArtifactType.JAVADOC && !failOnMissingSourcesForJavadoc) {
            otherPackages.addAll(ctx.getPackagesWithoutSources());
        }
        // add packages found in links
        if ( links != null ) {
            apiPackages.addAll(links.getLinkedPackages());
        }
        final List<ApiExport> missing = new ArrayList<>();

        for (final ApiExport exp : apiRegion.listExports()) {
            final String packageName = exp.getName();
            if ( !apiPackages.remove(packageName) && !otherPackages.remove(packageName)) {
                missing.add(exp);
            }
        }
        // correct remaining packages
        if ( links != null ) {
            apiPackages.removeAll(links.getLinkedPackages());
        }
        if ( artifactType == ArtifactType.JAVADOC ) {
            otherPackages.removeAll(ctx.getPackagesWithoutSources());
        }
        otherPackages.removeAll(ctx.getPackagesWithoutJavaClasses());

        apiPackages.addAll(otherPackages);
        if (missing.isEmpty() && apiPackages.isEmpty()) {
            getLog().info("Verified " + artifactType.getId() + " jar for region " + apiRegion.getName());
        } else {
            Collections.sort(missing);
            report.add(artifactType.getId().concat(" jar for region ").concat(apiRegion.getName()).concat(" has ").concat( String.valueOf(missing.size() + apiPackages.size()) ).concat(" errors:"));
            for (final ApiExport exp : missing) {
                final List<String> candidates = new ArrayList<>();
                for(final ArtifactInfo info : ctx.getArtifactInfos()) {
                    for(final Clause clause : info.getUsedExportedPackages(apiRegion)) {
                        if ( exp.getName().equals(clause.getName())) {
                            candidates.add(info.getId().toMvnName());
                            break;
                        }
                    }
                }
                report.add("- Missing package ".concat(exp.getName()).concat(" from bundle(s) ").concat(String.join(",", candidates)));
            }
            for (final String m : apiPackages) {
                report.add("- Wrong package ".concat(m));
            }
        }
    }

    private File getArtifactFile(final ArtifactProvider artifactProvider, final ArtifactId artifactId)
            throws MojoExecutionException {
        final URL artifactURL = retrieve(artifactProvider, artifactId);
        if (artifactURL == null) {
            throw new MojoExecutionException("Unable to find artifact " + artifactId.toMvnId());
        }
        File bundleFile = null;
        try
        {
            bundleFile = IOUtils.getFileFromURL(artifactURL, true, this.getTmpDir());
        } catch (IOException e) {
            throw new MojoExecutionException(e.getMessage());
        }

        return bundleFile;
    }

    private Manifest getManifest(final ArtifactId artifactId, final File bundleFile) throws MojoExecutionException {
        try (JarInputStream jis = new JarInputStream(new FileInputStream(bundleFile))) {
            getLog().debug("Reading Manifest headers from bundle " + bundleFile);

            final Manifest manifest = jis.getManifest();

            if (manifest == null) {
                throw new MojoExecutionException("Artifact + " + artifactId.toMvnId() + " does not  have a manifest.");
            }
            return manifest;
        } catch (final IOException e) {
            throw new MojoExecutionException("An error occurred while reading manifest from file " + bundleFile
                    + " for artifact " + artifactId.toMvnId(), e);
        }
    }

    private boolean calculateOmitDependenciesFlag(final Clause[] exportedPackageClauses, final Set<Clause> usedExportedPackagesPerRegion) {
        // check whether all packages are exported in this region
        boolean fullUsage = true;
        for(final Clause c : exportedPackageClauses) {
            boolean found = false;
            for(final Clause current : usedExportedPackagesPerRegion) {
                if ( current.getName().equals(c.getName()) ) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                fullUsage = false;
                break;
            }
        }

        return fullUsage;
    }

    /**
     * Process a single artifact.
     * This is a "global" processing and not per region
     * @param ctx The context
     * @param artifact The artifact
     * @throws MojoExecutionException
     */
    private void onArtifact(final ApisJarContext ctx, final Artifact artifact)
    throws MojoExecutionException {
        final File bundleFile = getArtifactFile(artifactProvider, artifact.getId());

        final Manifest manifest = getManifest(artifact.getId(), bundleFile);

        // check if the bundle is exporting packages?
        final Clause[] exportedPackageClauses = this.getExportedPackages(manifest);
        if (exportedPackageClauses.length > 0) {

            // calculate the exported packages in the manifest file for all regions
            final Set<String> usedExportedPackages = computeUsedExportPackages(ctx.getApiRegions(), exportedPackageClauses, artifact);

            if ( !usedExportedPackages.isEmpty()) {
                final ArtifactInfo info = ctx.addArtifactInfo(artifact);
                info.setUsedExportedPackages(usedExportedPackages);

                // calculate per region packages
                for(final ApiRegion region : ctx.getApiRegions().listRegions()) {
                    final Set<Clause> usedExportedPackagesPerRegion = computeUsedExportPackages(region, exportedPackageClauses, artifact);

                    // check whether packages are included in api jars - or added as a dependency
                    boolean useAsDependency = this.useApiDependencies ? calculateOmitDependenciesFlag(exportedPackageClauses, usedExportedPackagesPerRegion) : false;
                    if ( useAsDependency ) {
                        useAsDependency = ctx.findDependencyArtifact(getLog(), info);
                        if ( useAsDependency ) {
                            // check scm info
                            if ( artifact.getMetadata().get(ApisUtil.SCM_LOCATION) != null ) {
                                throw new MojoExecutionException("Dependency artifact must not specify " + ApisUtil.SCM_LOCATION + " : " + artifact.getId().toMvnId());
                            }
                        }
                    }
                    info.setUsedExportedPackages(region, usedExportedPackagesPerRegion, useAsDependency);
                }

                info.setBinDirectory(new File(ctx.getDeflatedBinDir(), info.getId().toMvnName()));
                info.setSourceDirectory(new File(ctx.getDeflatedSourcesDir(), info.getId().toMvnName()));

                final boolean skipBinDeflate = info.getBinDirectory().exists();
                if ( skipBinDeflate ) {
                    getLog().debug("Artifact " + info.getId().toMvnName() + " already deflated");
                }
                final boolean skipSourceDeflate = info.getSourceDirectory().exists();
                if ( skipSourceDeflate ) {
                    getLog().debug("Source for artifact " + info.getId().toMvnName() + " already deflated");
                }

                final String bundleClassPath = manifest.getMainAttributes().getValue(Constants.BUNDLE_CLASSPATH);
                final String[] embeddedBundles;
                if (bundleClassPath != null && !bundleClassPath.isEmpty()) {
                    embeddedBundles = bundleClassPath.split(",");
                } else {
                    embeddedBundles = null;
                }

                processBinary(ctx, info, bundleFile, artifact, embeddedBundles, skipBinDeflate, skipSourceDeflate);

                // check if the bundle wraps other bundles
                if ( embeddedBundles != null ) {
                    computeWrappedBundles(ctx, info, embeddedBundles, skipBinDeflate, skipSourceDeflate);
                }

                postProcessArtifact(ctx, info, artifact);

                if ( !info.getSourceDirectory().exists() ) {
                    info.setSourceDirectory(null);
                }

                if ( generateJavadocJar ) {
                    buildJavadocClasspath(artifact.getId()).forEach( ctx::addJavadocClasspath );
                }
            }

        }
    }

    /**
     * Post process
     * <ul>
     * <li>Find node types
     * <li>Find empty packages
     * <li>Find empty directories and remove them
     * <li>Clean up sources - if encoding is not UTF-8
     * </ul>
     * @param ctx The context
     * @param info The artifact info
     * @throws MojoExecutionException
     */
    private void postProcessArtifact(final ApisJarContext ctx, final ArtifactInfo info, final Artifact artifact)
    throws MojoExecutionException {
        // binary post processing
        this.postProcessBinDirectory(ctx, info, info.getBinDirectory(), "");

        // source post processing
        if ( (generateSourceJar || generateJavadocJar) ) {
            final Set<String> foundPackages = new HashSet<>();
            if ( info.getSourceDirectory() != null && info.getSourceDirectory().exists()) {
                final String encoding = artifact.getMetadata().getOrDefault(ApisUtil.SCM_ENCODING, "UTF-8");
                this.postProcessSourcesDirectory(ctx, info, foundPackages, info.getSourceDirectory(), "UTF-8".equals(encoding) ? null : encoding, "");
            }
            // check for missing packages
            for(final String pck : info.getUsedExportedPackages()) {
                if ( !foundPackages.contains(pck) ) {
                    // We need to record this kind of packages and ensure we don't trigger warnings for them
                    // when checking the api jars for correctness.
                    getLog().debug("No sources found in " + pck);
                    ctx.getPackagesWithoutSources().add(pck);
                }
            }
        }

    }

    private void postProcessBinDirectory(final ApisJarContext ctx, final ArtifactInfo info, final File dir, final String pck) {
        boolean hasJavaFile = false;
        for(final File child : dir.listFiles()) {
            if ( child.isDirectory() ) {
                postProcessBinDirectory(ctx, info, child, pck.isEmpty() ? child.getName() : pck.concat(".").concat(child.getName()));
            } else if ( child.getName().endsWith(ArtifactType.APIS.getContentExtension())) {
                hasJavaFile = true;
            }
        }
        if ( dir.listFiles().length == 0 && !pck.isEmpty() ) {
            // empty dir -> remove
            dir.delete();
        } else if ( !hasJavaFile && info.getUsedExportedPackages().contains(pck) ) {

            // We need to record this kind of packages and ensure we don't trigger warnings for them
            // when checking the api jars for correctness.
            getLog().debug("No classes found in " + pck);
            ctx.getPackagesWithoutJavaClasses().add(pck);
        }
    }

    /**
     * Process a binary
     * Extract the binary, rename resources and (optional) download the sources
     * @param ctx The context
     * @param info The current artifact
     * @param binFile The binary to extract
     * @param binArtifact The artifact to extract
     * @param embeddedBundles Embedded bundles (optional)
     * @param skipBinDeflate Flag to skip deflating the binary
     * @param skipSourceDeflate Flag to skip deflating the source
     * @throws MojoExecutionException
     */
    private void processBinary(final ApisJarContext ctx,
            final ArtifactInfo info,
            final File binFile,
            final Artifact binArtifact,
            final String[] embeddedBundles,
            final boolean skipBinDeflate,
            final boolean skipSourceDeflate)
    throws MojoExecutionException {
        if ( !skipBinDeflate ) {
            // deflate all bundles first, in order to copy APIs and resources later,
            // depending to the region
            final List<String> deflateIncludes = new ArrayList<>();

            // add all used exported packages
            deflateIncludes.addAll(Arrays.asList(info.getUsedExportedPackageIncludes()));
            // add embedded bundles
            if ( embeddedBundles != null ) {
                for(final String jarName : embeddedBundles) {
                    if ( !".".equals(jarName) ) {
                        deflateIncludes.add(jarName);
                    }
                }
            }
            // add resources from the folders
            deflateIncludes.addAll(getIncludeResourcePatterns(ctx, info.getId()));

            // deflate
            this.deflate(info.getBinDirectory(), binFile, deflateIncludes.toArray(new String[deflateIncludes.size()]));

        }
        // renaming potential name-collapsing resources
        this.renameResources(ctx, info, binArtifact.getId());

        // download sources
        if ( this.generateSourceJar || this.generateJavadocJar ) {
            if ( !skipSourceDeflate ) {
                this.downloadSources(ctx, info, binArtifact);
            } else {
                info.addSourceInfo("USE CACHE FROM PREVIOUS BUILD");
            }
        }

    }

    private List<String> getIncludeResourcePatterns(final ApisJarContext ctx, final ArtifactId id) {
        final List<String> pattern = new ArrayList<>();
        if ( includeResources != null ) {
            for(final String folder : this.resourceFolders.split(",")) {
                for(final String inc : this.includeResources) {
                    pattern.add(folder.trim().concat("/").concat(inc));
                }
            }
        }
        // add NOTICE and LICENSE for license report
        if ( this.licenseReport != null ) {
            final String licenseDefault = ctx.getLicenseDefault(id);
            if ( licenseDefault == null || !licenseDefault.isEmpty() ) {
                pattern.add("META-INF/NOTICE");
                pattern.add("META-INF/LICENSE");
            }
        }

        return pattern;
    }

    private void postProcessSourcesDirectory(final ApisJarContext ctx, final ArtifactInfo info,
            final Set<String> foundPackages,
            final File dir,
            final String readEncoding,
            final String pck) throws MojoExecutionException {
        boolean hasSourceFile = false;
        for(final File child : dir.listFiles()) {
            if ( child.isDirectory() ) {
                postProcessSourcesDirectory(ctx, info,
                        foundPackages, child, readEncoding,
                        pck.isEmpty() ? child.getName() : pck.concat(".").concat(child.getName()));
            } else if ( child.getName().endsWith(ArtifactType.SOURCES.getContentExtension())) {
                hasSourceFile = true;
                if (readEncoding != null ) {
                    try {
                        final String javaSource = FileUtils.fileRead(child, readEncoding);
                        FileUtils.fileWrite(child, StandardCharsets.UTF_8.name(), javaSource);
                    } catch ( final IOException ioe) {
                        throw new MojoExecutionException("Unable to clean up java source " + child, ioe);
                    }
                }
            }
        }
        if ( dir.listFiles().length == 0 && !pck.isEmpty() ) {
            // empty dir -> remove
            dir.delete();
        } else if ( hasSourceFile ) {
            foundPackages.add(pck);
        }
    }

    private void computeWrappedBundles(final ApisJarContext ctx,
            final ArtifactInfo info,
            final String[] embeddedBundles,
            final boolean skipBinDeflate,
            final boolean skipSourceDeflate)
    throws MojoExecutionException {
        for(final String jarName : embeddedBundles) {
            if (".".equals(jarName)) {
                continue;
            }

            final File wrappedJar = new File(info.getBinDirectory(), jarName);
            getLog().debug("Processing wrapped bundle " + wrappedJar);

            final Properties properties = new Properties();

            try (final JarInputStream jis = new JarInputStream(new FileInputStream(wrappedJar))) {
                JarEntry jarEntry = null;
                while ( (jarEntry = jis.getNextJarEntry()) != null ) {
                    if (!jarEntry.isDirectory()
                            && pomPropertiesPattern.matcher(jarEntry.getName()).matches()) {
                        getLog().debug("Loading Maven GAV from " + wrappedJar + '!' + jarEntry.getName());
                        properties.load(jis);
                        break;
                    }
                    jis.closeEntry();
                }
            } catch (IOException e) {
                throw new MojoExecutionException("An error occurred while processing wrapped bundle " + wrappedJar, e);
            }

            if (properties.isEmpty()) {
                getLog().warn("No Maven GAV info attached to wrapped bundle " + wrappedJar + ", it will be ignored");
            } else {
                getLog().debug("Handling synthetic artifacts from Maven GAV: " + properties);

                String groupId = properties.getProperty("groupId");
                String artifactId = properties.getProperty("artifactId");
                String version = properties.getProperty("version");
                String classifier = properties.getProperty("classifier");
                if (classifier == null) {
                    classifier = inferClassifier(jarName, artifactId, version);
                }

                Artifact syntheticArtifact = new Artifact(
                        new ArtifactId(groupId, artifactId, version, classifier, null));
                final File bundleFile = getArtifactFile(artifactProvider, syntheticArtifact.getId());

                processBinary(ctx, info, bundleFile, syntheticArtifact, null, skipBinDeflate, skipSourceDeflate);
            }
        }
    }

    // Guess the classifier based on the file name
    String inferClassifier(String bundleName, String artifactId, String version) {
        if (bundleName == null || artifactId == null || version == null)
            return null;

        int idx = bundleName.lastIndexOf('/');
        if (idx >= 0)
            bundleName = bundleName.substring(idx + 1);

        int edx = bundleName.lastIndexOf('.');
        if (edx > 0)
            bundleName = bundleName.substring(0, edx);

        // bundleName is now the bare name without extension
        String synthesized = artifactId + "-" + version;
        if (synthesized.length() < bundleName.length() &&
                bundleName.startsWith(synthesized)) {
            String suffix = bundleName.substring(synthesized.length());
            if (suffix.length() > 1 && suffix.startsWith("-")) {
                String classifier = suffix.substring(1);
                getLog().debug(
                        "Inferred classifier of '" + artifactId + ":" + version + "' to be '" + classifier + "'");
                return classifier;
            }
        }
        return null;
    }

    private Set<String> buildJavadocClasspath(ArtifactId artifactId)
            throws MojoExecutionException {
        final Set<String> javadocClasspath = new HashSet<>();
        getLog().debug("Retrieving " + artifactId + " and related dependencies...");

        org.apache.maven.artifact.Artifact toBeResolvedArtifact = repositorySystem.createArtifactWithClassifier(artifactId.getGroupId(),
                                                                                                                artifactId.getArtifactId(),
                                                                                                                artifactId.getVersion(),
                                                                                                                artifactId.getType(),
                                                                                                                artifactId.getClassifier());
        ArtifactResolutionRequest request = new ArtifactResolutionRequest()
                                            .setArtifact(toBeResolvedArtifact)
                                            .setServers(mavenSession.getRequest().getServers())
                                            .setMirrors(mavenSession.getRequest().getMirrors())
                                            .setProxies(mavenSession.getRequest().getProxies())
                                            .setLocalRepository(mavenSession.getLocalRepository())
                                            .setRemoteRepositories(mavenSession.getRequest().getRemoteRepositories())
                                            .setForceUpdate(false)
                                            .setResolveRoot(true)
                                            .setResolveTransitively(true)
                                            .setCollectionFilter(new ArtifactFilter() {
                                                    // artifact filter
                                                    @Override
                                                    public boolean include(org.apache.maven.artifact.Artifact artifact) {
                                                        if (org.apache.maven.artifact.Artifact.SCOPE_TEST.equals(artifact.getScope())) {
                                                            return false;
                                                        }
                                                        return true;
                                                    }});

        ArtifactResolutionResult result = repositorySystem.resolve(request);

        if (!result.isSuccess()) {
            if (result.hasCircularDependencyExceptions()) {
                getLog().warn("Cyclic dependency errors detected:");
                reportWarningMessages(result.getCircularDependencyExceptions());
            }

            if (result.hasErrorArtifactExceptions()) {
                getLog().warn("Resolution errors detected:");
                reportWarningMessages(result.getErrorArtifactExceptions());
            }

            if (result.hasMetadataResolutionExceptions()) {
                getLog().warn("Metadata resolution errors detected:");
                reportWarningMessages(result.getMetadataResolutionExceptions());
            }

            if (result.hasMissingArtifacts()) {
                getLog().warn("Missing artifacts detected:");
                for (org.apache.maven.artifact.Artifact missingArtifact : result.getMissingArtifacts()) {
                    getLog().warn(" - " + missingArtifact.getId());
                }
            }

            if (result.hasExceptions()) {
                getLog().warn("Generic errors detected:");
                for (Exception exception : result.getExceptions()) {
                    getLog().warn(" - " + exception.getMessage());
                }
            }
        }

        for (org.apache.maven.artifact.Artifact resolvedArtifact : result.getArtifacts()) {
            if (resolvedArtifact.getFile() != null) {
                getLog().debug("Adding to javadoc classpath " + resolvedArtifact);
                javadocClasspath.add(resolvedArtifact.getFile().getAbsolutePath());
            } else {
                getLog().debug("Ignoring for javadoc classpath " + resolvedArtifact);
            }
        }

        return javadocClasspath;
    }

    private <E extends ArtifactResolutionException> void reportWarningMessages(Collection<E> exceptions) {
        for (E exception : exceptions) {
            getLog().warn(" - "
                          + exception.getMessage()
                          + " ("
                          + exception.getArtifact().getId()
                          + ")");
        }
    }

    private URL retrieve(final ArtifactProvider artifactProvider, final ArtifactId artifactId) {
        getLog().debug("Retrieving artifact " + artifactId + "...");
        URL sourceFile = artifactProvider.provide(artifactId);
        if (sourceFile != null) {
            getLog().debug("Artifact " + artifactId + " successfully retrieved : " + sourceFile);
        }
        return sourceFile;
    }

    private void deflate(final File destDirectory, final File artifact, final String...includes) throws MojoExecutionException {
        getLog().debug("Deflating artifact " + artifact.getName() + "...");
        destDirectory.mkdirs();

        // unarchive the bundle
        try {
            final UnArchiver unArchiver = archiverManager.getUnArchiver(artifact);
            unArchiver.setSourceFile(artifact);
            unArchiver.setDestDirectory(destDirectory);
            final IncludeExcludeFileSelector selector = new IncludeExcludeFileSelector();
            selector.setIncludes(includes);
            unArchiver.setFileSelectors(new FileSelector[] { selector });
            unArchiver.setOverwrite(false);
            unArchiver.extract();
        } catch (final NoSuchArchiverException e) {
            throw new MojoExecutionException(
                    "An error occurred while deflating file " + artifact + " to directory " + destDirectory, e);
        }

        getLog().debug("Artifact " + artifact + " successfully deflated");
    }

    private void renameResources(final ApisJarContext ctx, final ArtifactInfo info, final ArtifactId artifactId) throws MojoExecutionException {
        final List<String> patterns = getIncludeResourcePatterns(ctx, info.getId());
        if (patterns.isEmpty()) {
            getLog().debug("No configured resources to rename in " + info.getBinDirectory());
        }

        getLog().debug("Renaming " + patterns + " files in " + info.getBinDirectory() + "...");

        final DirectoryScanner directoryScanner = new DirectoryScanner();
        directoryScanner.setBasedir(info.getBinDirectory());
        directoryScanner.setIncludes(patterns.toArray(new String[patterns.size()]));
        directoryScanner.scan();

        if (directoryScanner.getIncludedFiles().length == 0) {
            getLog().debug("No " + patterns + " resources in " + info.getBinDirectory() + " to be renamed found.");
            return;
        }

        for (final String resourceName : directoryScanner.getIncludedFiles()) {
            final File resource = new File(info.getBinDirectory(), resourceName);

            String includedName = resourceName.replace(File.separatorChar, '/');
            if ( !info.getIncludedResources().contains(resource) ) {
                final String prefix = artifactId.toMvnName().concat("-");

                if (resource.getName().startsWith(prefix)) {
                    getLog().debug("No need to rename " + resource);
                    info.getIncludedResources().add(resource);
                } else {
                    File renamed = new File(resource.getParentFile(), prefix.concat(resource.getName()));

                    getLog().debug("Renaming resource " + resource + " to " + renamed + "...");

                    if (resource.renameTo(renamed)) {
                        getLog().debug("Resource renamed to " + renamed);
                        info.getIncludedResources().add(renamed);
                    } else {
                        throw new MojoExecutionException("Impossible to rename resource " + resource + " to " + renamed
                                + ", please check the current user has enough rights on the File System");
                    }
                    final int lastSlash = includedName.lastIndexOf('/');
                    if ( lastSlash == -1 ) {
                        includedName = renamed.getName();
                    } else {
                        includedName = includedName.substring(0, lastSlash + 1).concat(renamed.getName());
                    }
                }
            }
            if ( includedName.endsWith(ArtifactType.CND.getContentExtension())) {
                info.getNodeTypes().add(includedName);
            }
        }

        getLog().debug(patterns + " resources in " + info.getBinDirectory() + " successfully renamed");
    }

    private boolean downloadSourceAndDeflate(final ApisJarContext ctx,
            final ArtifactInfo info,
            final ArtifactId sourcesArtifactId,
            final boolean allowFallback) throws MojoExecutionException {
        boolean failed = false;
        try {
            final URL url = retrieve(artifactProvider, sourcesArtifactId);
            if (url != null) {
                File sourcesBundle = IOUtils.getFileFromURL(url, true, null);
                deflate(info.getSourceDirectory(), sourcesBundle, info.getUsedExportedPackageIncludes());
             } else {
                if (!allowFallback) {
                    throw new MojoExecutionException("Unable to download sources for " + info.getId().toMvnId()
                            + " due to missing artifact " + sourcesArtifactId.toMvnId());
                }
                getLog().warn("Unable to download sources for " + info.getId().toMvnId() + " due to missing artifact "
                        + sourcesArtifactId.toMvnId() + ", trying source checkout next...");
                failed = true;
            }
        } catch (final MojoExecutionException mee) {
            throw mee;
        } catch (final Throwable t) {
            if (!allowFallback) {
                throw new MojoExecutionException("Unable to download sources for " + info.getId().toMvnId()
                        + " due to missing artifact " + sourcesArtifactId.toMvnId());
            }
            getLog().warn("Unable to download sources for " + info.getId().toMvnId() + " from "
                    + sourcesArtifactId.toMvnId() + " due to " + t.getMessage() + ", trying source checkout next...");
            failed = true;
        }
        return failed;
    }

    private void downloadSources(final ApisJarContext ctx, final ArtifactInfo info, final Artifact artifact) throws MojoExecutionException {
        ArtifactId artifactId = artifact.getId();
        getLog().debug("Downloading sources for " + artifactId.toMvnId() + "...");

        List<ArtifactId> scmIds = ApisUtil.getSourceIds(artifact);
        String scmLocation = artifact.getMetadata().get(ApisUtil.SCM_LOCATION);
        if ( scmIds != null && scmLocation != null) {
            throw new MojoExecutionException("Both " + ApisUtil.SCM_IDS + " and " + ApisUtil.SCM_LOCATION + " are defined for " + artifactId);
        }

        if ( scmIds != null ) {
            for(final ArtifactId sourcesArtifactId : scmIds) {
                downloadSourceAndDeflate(ctx, info, sourcesArtifactId, false);
                info.addSourceInfo(sourcesArtifactId);
            }
        } else if ( scmLocation != null ) {
            final String connection = checkoutSourcesFromSCM(ctx, info, artifact);
            info.addSourceInfo(connection);
        } else {
            final ArtifactId sourcesArtifactId = artifactId.changeClassifier("sources").changeType("jar");
            if ( downloadSourceAndDeflate(ctx, info, sourcesArtifactId, true) ) {
                final String connection =  checkoutSourcesFromSCM(ctx, info, artifact);
                info.addSourceInfo(connection);
            } else {
                info.addSourceInfo(sourcesArtifactId);
            }
        }
    }

    private Model getArtifactPom(final ApisJarContext ctx, final ArtifactId artifactId) throws MojoExecutionException {
        final ArtifactId pomArtifactId = artifactId.changeClassifier(null).changeType("pom");
        // check model cache
        Model model = ctx.getModelCache().get(pomArtifactId);
        if ( model == null ) {
            getLog().debug("Retrieving POM " + pomArtifactId.toMvnId() + "...");
            // POM file must exist, let the plugin fail otherwise
            final URL pomURL = retrieve(artifactProvider, pomArtifactId);
            if (pomURL == null) {
                throw new MojoExecutionException("Unable to find artifact " + pomArtifactId.toMvnId());
            }

            File pomFile = null;
            try {
                pomFile = IOUtils.getFileFromURL(pomURL, true, null);
            } catch (IOException e) {
                throw new MojoExecutionException(e.getMessage());
            }
            getLog().debug("POM " + pomArtifactId.toMvnId() + " successfully retrieved, reading the model...");

            // read model
            model = modelBuilder.buildRawModel(pomFile, ModelBuildingRequest.VALIDATION_LEVEL_MINIMAL, false).get();
            getLog().debug("POM model " + pomArtifactId.toMvnId() + " successfully read");

            // cache model
            ctx.getModelCache().put(pomArtifactId, model);
        }
        return model;
    }

    private String checkoutSourcesFromSCM(final ApisJarContext ctx,
            final ArtifactInfo info,
            final Artifact sourceArtifact)
    throws MojoExecutionException {
        // fallback to Artifacts SCM metadata first
        String connection = sourceArtifact.getMetadata().get(ApisUtil.SCM_LOCATION);
        String tag = sourceArtifact.getMetadata().get(ApisUtil.SCM_TAG);

        // Artifacts SCM metadata may not available or are an override, let's fallback to the POM
        getLog().debug("Falling back to SCM checkout...");
        final Model pomModel = getArtifactPom(ctx, sourceArtifact.getId());
        getLog().debug("Processing SCM info from pom...");

        final Scm scm = pomModel.getScm();
        if (scm != null) {
            if ( connection == null ) {
                connection = scm.getConnection();
            }
            if ( tag == null ) {
                tag = scm.getTag();
            }
        }

        if (connection == null) {
            getLog().warn("Ignoring sources for artifact " + sourceArtifact.getId().toMvnId() + " : SCM not defined in "
                    + sourceArtifact.getId().toMvnId()
                          + " bundle neither in "
                    + pomModel.getId() + " POM file.");
            return null;
        }

        try {
            ScmRepository repository = scmManager.makeScmRepository(connection);

            ScmVersion scmVersion = null;
            if (tag != null) {
                scmVersion = new ScmTag(tag);
            }

            File basedir = new File(ctx.getCheckedOutSourcesDir(), sourceArtifact.getId().toMvnName());
            if (basedir.exists()) {
                getLog().debug("Source checkout directory " + basedir + " already exists");
            } else {
                getLog().debug("Checking out source to directory " + basedir);
                basedir.mkdirs();
                ScmFileSet fileSet = new ScmFileSet(basedir);

                CheckOutScmResult result = null;
                try {
                    if (scmVersion != null) {
                        result = scmManager.checkOut(repository, fileSet, true);
                    } else {
                        result = scmManager.checkOut(repository, fileSet, scmVersion, true);
                    }
                } catch (ScmException se) {
                    throw new MojoExecutionException("An error occurred while checking sources from " + repository
                            + " for artifact " + sourceArtifact.getId().toMvnId() + " model", se);
                }

                if (!result.isSuccess()) {
                    getLog().warn("Ignoring sources for artifact " + sourceArtifact.getId().toMvnId()
                            + " : An error occurred while checking out sources from " + connection + ": "
                            + result.getProviderMessage());
                    return null;
                }
            }

            // retrieve the exact pom location to detect the bundle path in the repo
            DirectoryScanner pomScanner = new DirectoryScanner();
            pomScanner.setBasedir(basedir);
            pomScanner.setIncludes("**/pom.xml");
            pomScanner.scan();
            for (String pomFileLocation : pomScanner.getIncludedFiles()) {
                final File pomFile = new File(basedir, pomFileLocation);
                final Model model = modelBuilder.buildRawModel(pomFile, ModelBuildingRequest.VALIDATION_LEVEL_MINIMAL, false).get();

                if (sourceArtifact.getId().getArtifactId().equals(model.getArtifactId())) {
                    basedir = pomFile.getParentFile();
                    break;
                }
            }

            // copy all interested sources to the proper location
            File javaSources = new File(basedir, "src/main/java");
            if (!javaSources.exists()) { // old modules could still use src/java
                javaSources = new File(basedir, "src/java");

                // there could be just resources artifacts
                if (!javaSources.exists()) {
                    getLog().warn("Ignoring sources for artifact " + sourceArtifact.getId().toMvnId() + " : SCM checkout for "
                            + sourceArtifact.getId().toMvnId()
                            + " does not contain any source.");
                    return null;
                }
            }

            final File sourceDirectory = new File(ctx.getDeflatedSourcesDir(), info.getId().toMvnName());
            info.setSourceDirectory(sourceDirectory);
            sourceDirectory.mkdir();

            final DirectoryScanner directoryScanner = new DirectoryScanner();
            directoryScanner.setBasedir(javaSources);
            directoryScanner.setIncludes(info.getUsedExportedPackageIncludes());
            directoryScanner.scan();

            for (String file : directoryScanner.getIncludedFiles()) {
                final File source = new File(javaSources, file);
                final File destination = new File(sourceDirectory, file);
                destination.getParentFile().mkdirs();
                try {
                    FileUtils.copyFile(source, destination);
                } catch (IOException e) {
                    throw new MojoExecutionException(
                                "An error occurred while copying sources from " + source + " to " + destination, e);
                }
            }

            return tag == null ? connection : connection.concat("@").concat(tag);
        } catch (ScmRepositoryException se) {
            throw new MojoExecutionException("An error occurred while reading SCM from "
                                             + connection
                                             + " connection for bundle "
                                             + sourceArtifact.getId(), se);
        } catch (NoSuchScmProviderException nsspe) {
            getLog().warn("Ignoring sources for artifact " + sourceArtifact.getId().toMvnId()
                    + " : bundle points to an SCM connection "
                           + connection
                           + " which does not specify a valid or supported SCM provider", nsspe);
            return null;
        }
    }

    private Clause[] getExportedPackages(final Manifest manifest) {
        final String exportPackageHeader = manifest.getMainAttributes().getValue(Constants.EXPORT_PACKAGE);
        final Clause[] exportPackages = Parser.parseHeader(exportPackageHeader);

        return exportPackages;
    }

    /**
     * Compute exports based on a single region
     *
     * @return List of packages exported by this bundle and used in the region
     */
    private Set<Clause> computeUsedExportPackages(final ApiRegion apiRegion,
            final Clause[] exportedPackages,
            final Artifact bundle)
            throws MojoExecutionException {

        final Set<Clause> result = new HashSet<>();

        final Set<String> ignoredPackages = ApisUtil.getIgnoredPackages(bundle);

        // filter for each region
        for (final Clause exportedPackage : exportedPackages) {
            final String packageName = exportedPackage.getName();

            if ( !ignoredPackages.contains(packageName)) {
                final ApiExport exp = apiRegion.getExportByName(packageName);
                if (exp != null) {
                    result.add(exportedPackage);
                }
            }
        }

        return result;
    }

    /**
     * Compute exports based on all regions
     *
     * @return List of packages exported by this bundle and used in any region
     */
    private Set<String> computeUsedExportPackages(final ApiRegions apiRegions,
            final Clause[] exportedPackages,
            final Artifact bundle)
            throws MojoExecutionException {
        final Set<String> result = new HashSet<>();

        // filter for each region
        for (final Clause exportedPackage : exportedPackages) {
            final String packageName = exportedPackage.getName();

            for (ApiRegion apiRegion : apiRegions.listRegions()) {
                final ApiExport exp = apiRegion.getExportByName(packageName);
                if (exp != null) {
                    result.add(exportedPackage.getName());
                }
            }
        }

        // check ignored packages configuration
        result.removeAll(ApisUtil.getIgnoredPackages(bundle));

        return result;
    }

    private String getApiExportClause(final ApiRegion region, final Collection<ArtifactInfo> infos) {
        final StringBuilder sb = new StringBuilder();
        boolean first = true;
        for(final ArtifactInfo info : infos) {
            for(final Clause clause : info.getUsedExportedPackages(region)) {
                if (first) {
                    first = false;
                } else {
                    sb.append(',');
                }
                sb.append(clause.toString());
            }
        }
        return sb.toString();
    }

    private File createArchive(final ApisJarContext ctx,
            final ApiRegion apiRegion,
            final ArtifactType archiveType,
            final List<File> resources,
            final Collection<ArtifactInfo> infos,
            final List<String> report) throws MojoExecutionException {
        final JarArchiver jarArchiver = new JarArchiver();

        if ( archiveType == ArtifactType.APIS || archiveType == ArtifactType.SOURCES ) {
            // api or source
            for(final ArtifactInfo info : infos) {
                final File dir = archiveType == ArtifactType.APIS ? info.getBinDirectory() : info.getSourceDirectory();

                if ( dir != null ) {
                    final String[] usedExportedPackageIncludes = info.getUsedExportedPackageIncludes(apiRegion);
                    getLog().debug("Adding directory " + dir.getName() + " with " + Arrays.toString(usedExportedPackageIncludes));
                    final DefaultFileSet fileSet = new DefaultFileSet(dir);
                    fileSet.setIncludingEmptyDirectories(false);
                    fileSet.setIncludes(usedExportedPackageIncludes);
                    jarArchiver.addFileSet(fileSet);
                }
            }
        } else {
            // javadoc
            final DefaultFileSet fileSet = new DefaultFileSet(ctx.getJavadocDir());
            jarArchiver.addFileSet(fileSet);
        }

        // add included resources
        for(final ArtifactInfo info : infos) {
            final int prefixLength = info.getBinDirectory().getAbsolutePath().length() + 1;
            for(final File resource : info.getIncludedResources()) {
                final String name = resource.getAbsolutePath().substring(prefixLength);
                jarArchiver.addFile(resource, name);
                getLog().debug("Adding resource " + name);
            }
        }

        // add additional resources
        if (resources != null) {
            for (final File rsrc : resources) {
                getLog().debug("Adding resource " + rsrc);
                if (rsrc.isDirectory()) {
                    DirectoryScanner ds = new DirectoryScanner();
                    ds.setBasedir(rsrc);
                    ds.setIncludes("**/*.*");
                    ds.scan();

                    for (String includedFile : ds.getIncludedFiles()) {
                        jarArchiver.addFile(new File(rsrc, includedFile), includedFile);
                    }
                } else {
                    jarArchiver.addFile(rsrc, rsrc.getName());
                }
            }
        }

        // check for license report
        if ( this.licenseReport != null ) {
            final File out = this.createLicenseReport(ctx, apiRegion, infos, report);
            jarArchiver.addFile(out, this.licenseReport);
        }

        final ArtifactId targetId = this.buildArtifactId(ctx, apiRegion, archiveType);
        final String artifactName = String.format("%s-%s", targetId.getArtifactId(), targetId.getClassifier());

        MavenArchiveConfiguration archiveConfiguration = new MavenArchiveConfiguration();
        archiveConfiguration.setAddMavenDescriptor(false);
        if (archiveType == ArtifactType.APIS) {
            // APIs need OSGi Manifest entry
            String symbolicName = artifactName.replace('-', '.');
            archiveConfiguration.addManifestEntry("Export-Package", getApiExportClause(apiRegion, infos));
            archiveConfiguration.addManifestEntry("Bundle-Description", project.getDescription());
            archiveConfiguration.addManifestEntry("Bundle-Version", targetId.getOSGiVersion().toString());
            archiveConfiguration.addManifestEntry("Bundle-ManifestVersion", "2");
            archiveConfiguration.addManifestEntry("Bundle-SymbolicName", symbolicName);
            archiveConfiguration.addManifestEntry("Bundle-Name", artifactName);

            final Set<String> nodeTypes = new HashSet<>();
            for(final ArtifactInfo info : infos) {
                 nodeTypes.addAll(info.getNodeTypes());
            }
            if (!nodeTypes.isEmpty()) {
                archiveConfiguration.addManifestEntry("Sling-Nodetypes", String.join(",", nodeTypes));
            }
            if (project.getOrganization() != null) {
                archiveConfiguration.addManifestEntry("Bundle-Vendor", project.getOrganization().getName());
            }

            // add provide / require capability to make the jar unresolvable
            archiveConfiguration.addManifestEntry("Provide-Capability", "osgi.unresolvable");
            archiveConfiguration.addManifestEntry("Require-Capability", "osgi.unresolvable;filter:=\"(&(must.not.resolve=*)(!(must.not.resolve=*)))\",osgi.ee;filter:=\"(&(osgi.ee=JavaSE/compact2)(version=1.8))\"");
        }
        archiveConfiguration.addManifestEntry("Implementation-Version", targetId.getVersion());
        archiveConfiguration.addManifestEntry("Specification-Version", targetId.getVersion());

        archiveConfiguration.addManifestEntry("Implementation-Title", artifactName);
        archiveConfiguration.addManifestEntry("Specification-Title", artifactName);
        if (project.getOrganization() != null) {
            archiveConfiguration.addManifestEntry("Implementation-Vendor", project.getOrganization().getName());
            archiveConfiguration.addManifestEntry("Specification-Vendor", project.getOrganization().getName());
        }

        // replace/add manifest entries with the one provided in manifestProperties configuration
        archiveConfiguration.addManifestEntries(ProjectHelper.propertiesToMap(manifestProperties));

        final File target = new File(mainOutputDir, targetId.toMvnName());
        MavenArchiver archiver = new MavenArchiver();
        archiver.setArchiver(jarArchiver);
        archiver.setOutputFile(target);

        try {
            archiver.createArchive(mavenSession, project, archiveConfiguration);
            if (this.attachApiJars) {
                projectHelper.attachArtifact(project, targetId.getType(), targetId.getClassifier(), target);
            }
        } catch (Exception e) {
            throw new MojoExecutionException("An error occurred while creating APIs "
                    + target
                    +" archive", e);
        }

        return target;
    }

    private ArtifactId buildArtifactId(final ApisJarContext ctx, final ApiRegion apiRegion, final ArtifactType artifactType) {
        final StringBuilder classifierBuilder = new StringBuilder();
        if (ctx.getFeatureId().getClassifier() != null) {
            classifierBuilder.append(mapApiClassifier(ctx.getFeatureId().getClassifier()))
                             .append('-');
        }
        final String finalClassifier = classifierBuilder.append(mapApiRegionName(apiRegion.getName()))
                                                  .append('-')
                                                  .append(artifactType.getId())
                                                  .toString();

        return new ArtifactId(this.project.getGroupId(),
                this.project.getArtifactId(),
                this.apiVersion != null ? this.apiVersion : this.project.getVersion(),
                finalClassifier,
                artifactType.getExtension());
    }

    /**
     * Create the dependencies file for a region
     * @param ctx The context
     * @param apiRegion The region
     */
    private void createDependenciesFile(final ApisJarContext ctx, final ApiRegion apiRegion) throws MojoExecutionException {
        final Collection<ArtifactInfo> infos = ctx.getArtifactInfos(apiRegion, false);

        final List<ArtifactId> dependencies = new ArrayList<>();

        for(final ArtifactInfo info : infos) {
            if ( info.isUseAsDependencyPerRegion(apiRegion) ) {
                dependencies.addAll(info.getDependencyArtifacts());
            }
        }
        Collections.sort(dependencies);

        final ArtifactId targetId = this.buildArtifactId(ctx, apiRegion, ArtifactType.DEPENDENCIES);
        final File target = new File(mainOutputDir, targetId.toMvnName());

        if ( !dependencies.isEmpty() ) {
            getLog().info("Writing dependencies file ".concat(target.getAbsolutePath()));
            try ( final Writer w = new FileWriter(target)) {
                for(final ArtifactId id : dependencies) {
                    w.write(id.toMvnId());
                    w.write(System.lineSeparator());
                }
            } catch (final IOException e) {
                throw new MojoExecutionException("Unable to write dependencies file", e);
            }
            if (this.attachApiJars) {
                projectHelper.attachArtifact(project, targetId.getType(), targetId.getClassifier(), target);
            }
        } else {
            getLog().info("No dependencies found");
            if ( target.exists() ) {
                target.delete();
            }
        }
    }


    /**
     * Generate the javadoc
     * @param ctx The generation context
     * @param region The region
     * @param javadocDir The output directory
     * @return {@code true} if generation succeeded
     * @throws MojoExecutionException on error
     */
    private Collection<ArtifactInfo> generateJavadoc(final ApisJarContext ctx, final ApiRegion region, final JavadocLinks links, final File javadocDir)
            throws MojoExecutionException {
        final Collection<ArtifactInfo> usedInfos = new ArrayList<>();

        final List<String> sourceDirectories = new ArrayList<>();
        final Set<String> javadocPackages = new HashSet<>();
        for(final ArtifactInfo info : ctx.getArtifactInfos(region, false)) {
            boolean addDirectory = false;
            for(final Clause clause : info.getUsedExportedPackages(region)) {
                if ( !ctx.getPackagesWithoutSources().contains(clause.getName()) && !links.getLinkedPackages().contains(clause.getName())) {
                    addDirectory = true;
                    javadocPackages.add(clause.getName());
                }
            }
            if ( addDirectory && info.getSourceDirectory() != null ) {
                usedInfos.add(info);
                sourceDirectories.add(info.getSourceDirectory().getAbsolutePath());
            }
        }

        if (javadocPackages.isEmpty()) {
            return null;
        }

        javadocDir.mkdirs();

        final JavadocExecutor javadocExecutor = new JavadocExecutor(javadocDir.getParentFile())
                                          .addArgument("-public")
                                          .addArgument("-encoding", false)
                                          .addArgument("UTF-8")
                                          .addArgument("-charset", false)
                                          .addArgument("UTF-8")
                                          .addArgument("-docencoding", false)
                                          .addArgument("UTF-8")
                                          .addArgument("-d", false)
                                          .addArgument(javadocDir.getAbsolutePath())
                                          .addArgument("-sourcepath", false)
                                          .addArgument(String.join(File.pathSeparator, sourceDirectories));

        javadocExecutor.addArgument("-source", false)
                       .addArgument(javadocSourceLevel);

        final String versionSuffix = this.apiVersion != null ? this.apiVersion : ctx.getFeatureId().getVersion();

        if (!StringUtils.isBlank(project.getName())) {
            javadocExecutor.addArgument("-doctitle", false)
                           .addQuotedArgument(project.getName().trim().concat(" ").concat(versionSuffix));
        }

        if (!StringUtils.isBlank(project.getDescription())) {
            javadocExecutor.addArgument("-windowtitle", false)
                           .addQuotedArgument(project.getDescription().trim().concat(" ").concat(versionSuffix));
        }

        if (!StringUtils.isBlank(project.getInceptionYear())
                && project.getOrganization() != null
                && !StringUtils.isBlank(project.getOrganization().getName())) {
            javadocExecutor.addArgument("-bottom", false)
                           .addQuotedArgument(String.format("Copyright &copy; %s - %s %s. All Rights Reserved",
                                              project.getInceptionYear().trim(),
                                              Calendar.getInstance().get(Calendar.YEAR),
                                              project.getOrganization().getName().trim()));
        }

        if ( !links.getJavadocLinks().isEmpty()) {
            javadocExecutor.addArguments("-link", links.getJavadocLinks());
        }

        if (!ctx.getJavadocClasspath().isEmpty()) {
            javadocExecutor.addArgument("-classpath", false)
                           .addArgument(ctx.getJavadocClasspath(), File.pathSeparator);
        }

        // turn off doclint when running Java8
        // http://blog.joda.org/2014/02/turning-off-doclint-in-jdk-8-javadoc.html
        if (SystemUtils.isJavaVersionAtLeast(JavaVersion.JAVA_1_8)) {
            javadocExecutor.addArgument("-Xdoclint:none");
        }

        javadocExecutor.addArgument("--allow-script-in-comments");

        if ( !this.javadocIndex ) {
            javadocExecutor.addArgument("-noindex");
        }
        if ( !this.javadocTree ) {
            javadocExecutor.addArgument("-notree");
        }

        // list packages
        javadocExecutor.addArguments(javadocPackages);

        // .addArgument("-J-Xmx2048m")
        javadocExecutor.execute(javadocDir, getLog(), this.ignoreJavadocErrors);

        return usedInfos;
    }

    /**
     * Get all packages contained in the archive
     * @param file The archive to check
     * @param extension The extension to check for
     * @return A tuple of packages containing files with the extension and packages with files not having the extension
     * @throws MojoExecutionException
     */
    private Map.Entry<Set<String>, Set<String>> getPackages(final File file, final String extension) throws MojoExecutionException {
        final Set<String> packages = new TreeSet<>();
        final Set<String> otherPackages = new TreeSet<>();

        final Set<String> excludes = new HashSet<>();
        for(final String v : resourceFolders.split(",")) {
            excludes.add(v.concat("/"));
        }

        try (final JarInputStream jis = new JarInputStream(new FileInputStream(file))) {
            JarEntry entry;
            while ((entry = jis.getNextJarEntry()) != null) {
                if ( !entry.isDirectory() ) {
                    boolean exclude = false;
                    for(final String v : excludes) {
                        if ( entry.getName().startsWith(v)) {
                            exclude = true;
                            break;
                        }
                    }
                    if ( !exclude ) {
                        final int lastPos = entry.getName().lastIndexOf('/');
                        if (lastPos != -1) {
                            final String packageName = entry.getName().substring(0, lastPos).replace('/', '.');

                            if (entry.getName().endsWith(extension)) {
                                packages.add(packageName);
                            } else {
                                otherPackages.add(packageName);
                            }
                        }
                    }
                }
                jis.closeEntry();
            }
        } catch (final IOException ioe) {
            throw new MojoExecutionException("Unable to scan file " + file + " : " + ioe.getMessage());
        }

        otherPackages.removeAll(packages);

        return Collections.singletonMap(packages, otherPackages).entrySet().iterator().next();
    }

    private File createLicenseReport(final ApisJarContext ctx,
            final ApiRegion region,
            final Collection<ArtifactInfo> infos,
            final List<String> report) throws MojoExecutionException {
        final File out = new File(this.getTmpDir(), region.getName() + "-license-report.txt");
        if ( !out.exists() ) {

            final List<String> output = new ArrayList<>();
            output.add(licenseReportHeader);
            output.add("");
            for(final ArtifactInfo info : infos) {
                final String licenseDefault = ctx.getLicenseDefault(info.getId());

                final StringBuilder sb = new StringBuilder(info.getId().toMvnId());
                boolean exclude = false;
                if ( licenseDefault != null ) {
                    if ( licenseDefault.isEmpty()) {
                        exclude = true;
                        getLog().debug("Excluding from license report " + info.getId().toMvnId());
                    } else {
                        sb.append(" - License(s) : ");
                        sb.append(licenseDefault);
                    }
                } else {
                    final List<License> licenses = this.getLicenses(ctx, info);

                    if ( !licenses.isEmpty() ) {
                        sb.append(" - License(s) : ");
                        sb.append(String.join(", ",
                                licenses.stream()
                                        .map(l -> l.getName().concat(" (").concat(l.getUrl()).concat(")"))
                                        .collect(Collectors.toList())));
                    } else {
                        report.add("No license info found for ".concat(info.getId().toMvnId()));
                    }
                }
                if ( !exclude ) {
                    output.add(sb.toString());
                }
            }
            if ( this.licenseReportFooter != null ) {
                output.add("");
                output.add(this.licenseReportFooter);
            }
            try {
                Files.write(out.toPath(), output);
            } catch (IOException e) {
                throw new MojoExecutionException("Unable to write license report: " + e.getMessage(), e);
            }
        }
        return out;
    }

    private List<License> getLicenses(final ApisJarContext ctx, final ArtifactInfo info) {
        getLog().debug("Getting license for " + info.getId().toMvnId());
        List<License> result = info.getLicenses();
        if  ( result == null ) {
            try {
                ArtifactId id = info.getId();
                do {
                    final Model model = getArtifactPom(ctx, id);
                    final List<License> ll = model.getLicenses();

                    if ( ll != null && !ll.isEmpty() ) {
                        getLog().debug("Found license for " + id.toMvnId());
                        result = ll;
                    } else {
                        if ( model.getParent() != null ) {
                            final ArtifactId newId = new ArtifactId(model.getParent().getGroupId(), model.getParent().getArtifactId(), model.getParent().getVersion(), null, "pom");
                            if ( newId.equals(id) ) {
                                break;
                            } else {
                                id = newId;
                            }
                        } else {
                            break;
                        }
                    }
                }  while (result == null);
            } catch (MojoExecutionException m) {
                // nothing to do
            }
            if ( result == null ) {
                result = Collections.emptyList();
            }
            info.setLicenses(result);
        }
        getLog().debug("License for " + info.getId().toMvnId() + " = " + result);
        return result;
    }
}

