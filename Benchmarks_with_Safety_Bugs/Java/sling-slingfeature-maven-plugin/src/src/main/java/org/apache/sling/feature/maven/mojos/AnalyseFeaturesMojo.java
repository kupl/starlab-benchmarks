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
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.maven.model.Dependency;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;
import org.apache.maven.plugins.annotations.ResolutionScope;
import org.apache.maven.shared.utils.logging.MessageUtils;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.analyser.Analyser;
import org.apache.sling.feature.analyser.AnalyserResult;
import org.apache.sling.feature.builder.ArtifactProvider;
import org.apache.sling.feature.builder.FeatureProvider;
import org.apache.sling.feature.maven.ProjectHelper;
import org.apache.sling.feature.scanner.Scanner;

/**
 * Analyse the feature.
 */
@Mojo(name = "analyse-features",
        defaultPhase = LifecyclePhase.VERIFY,
      requiresDependencyResolution = ResolutionScope.TEST,
      threadSafe = true
    )
public class AnalyseFeaturesMojo extends AbstractIncludingFeatureMojo {

    private static final String FILE_STORAGE_CONFIG_KEY = "fileStorage";

    private static final String ANALYSER_CONFIG_WILDCARD = "all";

    /**
     * The scans for the analyser
     */
    @Parameter
    private List<Scan> scans;

    /**
     * The framework to use for analysing the feature models if not
     * contained in an execution-environment extension.
     */
    @Parameter
    private Dependency framework;

    /**
     * By default the warnings from the analysers are logged. Setting this to
     * false disables logging them.
     * @since 1.3.8
     */
    @Parameter(defaultValue = "true")
    private boolean logWarnings;

    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        checkPreconditions();
        List<Scan> list = scans;
        if (list == null || list.isEmpty()) {
            // use default configuration
            final Scan a = new Scan();
            a.setFilesInclude("**/*.*");
            list = Collections.singletonList(a);
        }
        final ArtifactProvider am = new ArtifactProvider() {

            @Override
            public URL provide(final ArtifactId id) {
                try {
                    return ProjectHelper.getOrResolveArtifact(project, mavenSession, artifactHandlerManager, artifactResolver, id).getFile().toURI().toURL();
                } catch (final MalformedURLException e) {
                    getLog().debug("Malformed url " + e.getMessage(), e);
                    // ignore
                    return null;
                }
            }
        };

        getLog().debug(MessageUtils.buffer().a("Setting up the ").strong("Scanner").a("...").toString());
        Scanner scanner;
        try {
            scanner = new Scanner(am);
        } catch (final IOException e) {
            throw new MojoExecutionException("A fatal error occurred while setting up the Scanner, see error cause:",
                    e);
        }
        getLog().debug(MessageUtils.buffer().strong("Scanner").a(" successfully set up").toString());

        FeatureProvider featureProvider = new BaseFeatureProvider();

        boolean hasErrors = false;
        for (final Scan an : list) {
            try {
                Map<String, Map<String, String>> taskConfiguration = an.getTaskConfiguration();
                addTaskConfigurationDefaults(taskConfiguration);

                getLog().debug(MessageUtils.buffer().a("Setting up the ").strong("analyser")
                        .a(" with following configuration:").toString());
                getLog().debug(" * Task Configuration = " + taskConfiguration);
                Set<String> includedTasks = an.getIncludeTasks();
                if (includedTasks == null) {
                    // use defaults
                    includedTasks = new HashSet<>();
                    includedTasks.add("bundle-packages");
                    includedTasks.add("requirements-capabilities");
                    includedTasks.add("apis-jar");
                    if (an.getExcludeTasks() != null) {
                        includedTasks.removeAll(an.getExcludeTasks());
                        if (includedTasks.isEmpty()) {
                            includedTasks = null;
                        }
                    }
                }
                getLog().debug(" * Include Tasks = " + includedTasks);
                getLog().debug(" * Exclude Tasks = " + an.getExcludeTasks());
                final Analyser analyser = new Analyser(scanner, taskConfiguration, includedTasks, an.getExcludeTasks());
                getLog().debug(MessageUtils.buffer().strong("Analyser").a(" successfully set up").toString());

                getLog().debug("Retrieving Feature files...");
                final Collection<Feature> features = this.getSelectedFeatures(an).values();

                if (features.isEmpty()) {
                    getLog().debug(
                            "There are no assciated feature files to current project, plugin execution will be skipped");
                    continue;
                } else {
                    getLog().debug("Starting analysis of features...");
                }

                for (final Feature f : features) {
                    try {
                        getLog().debug(MessageUtils.buffer().a("Analyzing feature ").strong(f.getId().toMvnId())
                                .a(" ...").toString());
                        Dependency fwk = an.getFramework();
                        if (fwk == null) {
                            fwk = this.framework;
                        }
                        final AnalyserResult result = analyser.analyse(f, ProjectHelper.toArtifactId(fwk), featureProvider);
                        if ( logWarnings ) {
                            for (final String msg : result.getWarnings()) {
                                getLog().warn(msg);
                            }
                        }
                        for (final String msg : result.getErrors()) {
                            getLog().error(msg);
                        }

                        if (!result.getErrors().isEmpty()) {
                            getLog().error("Analyser detected errors on feature '" + f.getId().toMvnId()
                                    + "'. See log output for error messages.");
                            hasErrors = true;
                        } else {
                            getLog().debug(MessageUtils.buffer().a("feature ").debug(f.getId().toMvnId())
                                    .a(" succesfully passed all analysis").toString());
                        }
                    } catch (Exception t) {
                        throw new MojoFailureException(
                                "Exception during analysing feature " + f.getId().toMvnId() + " : " + t.getMessage(),
                                t);
                    }
                }
            } catch (IOException e) {
                throw new MojoExecutionException(
                        "A fatal error occurred while setting up the analyzer, see error cause:", e);
            } finally {
                getLog().debug("Features analysis complete");
            }
        }
        if (hasErrors) {
            throw new MojoFailureException(
                    "One or more feature analyzer(s) detected feature error(s), please read the plugin log for more details");
        }
    }

    void addTaskConfigurationDefaults(Map<String, Map<String, String>> taskConfiguration) {
        String featureModelFileStorage = project.getBuild().getDirectory() + "/sling-slingfeature-maven-plugin-fmtmp";
        Map<String, String> wildCardCfg = taskConfiguration.get(ANALYSER_CONFIG_WILDCARD);
        if (wildCardCfg == null) {
            wildCardCfg = new HashMap<String, String>();
            taskConfiguration.put(ANALYSER_CONFIG_WILDCARD, wildCardCfg);
        }
        if (!wildCardCfg.containsKey(FILE_STORAGE_CONFIG_KEY)) {
            new File(featureModelFileStorage).mkdirs();
            wildCardCfg.put(FILE_STORAGE_CONFIG_KEY, featureModelFileStorage);
        }
    }
}
