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

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Formatter;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.maven.model.Dependency;
import org.apache.maven.model.Exclusion;
import org.apache.maven.project.MavenProject;
import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.ExtensionType;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.builder.BuilderContext;
import org.apache.sling.feature.builder.FeatureBuilder;
import org.apache.sling.feature.builder.FeatureProvider;
import org.apache.sling.feature.io.json.FeatureJSONReader;
import org.codehaus.plexus.logging.Logger;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.github.fge.jsonschema.core.exceptions.ProcessingException;
import com.github.fge.jsonschema.core.report.LogLevel;
import com.github.fge.jsonschema.core.report.ProcessingMessage;
import com.github.fge.jsonschema.core.report.ProcessingReport;
import com.github.fge.jsonschema.main.JsonSchema;
import com.github.fge.jsonschema.main.JsonSchemaFactory;

/**
 * The processor processes all feature projects.
 */
public class Preprocessor {

    private final JsonSchema schema;

    private final ObjectMapper objectMapper;

    public Preprocessor() {
       JsonSchemaFactory schemaFactory = JsonSchemaFactory.byDefault();
       String jsonSchemaUri = getClass().getClassLoader().getResource("META-INF/feature/Feature-1.0.0.schema.json").toExternalForm();
       try {
           schema = schemaFactory.getJsonSchema(jsonSchemaUri);
       } catch (ProcessingException e) {
           throw new RuntimeException("An error occured when retrieving the JSON Schema from " + jsonSchemaUri, e);
       }
       objectMapper = new ObjectMapper();
    }

    private void checkFeatureFileValidation(File featureFile, String json) {
       try {
           JsonNode instance = objectMapper.readTree(json);
           ProcessingReport report = schema.validate(instance, true);
           if (!report.isSuccess()) {
               Formatter formatter = new Formatter();
               formatter.format("Feature file %s validation detected one or more errors:%n", featureFile);
                for (ProcessingMessage message : report) {
                   if (LogLevel.FATAL == message.getLogLevel()
                           || LogLevel.ERROR == message.getLogLevel())
                   formatter.format(" * %s: %s%n",
                                    message.asJson().get("schema").get("pointer").asText(),
                                    message.getMessage());
               }
                String errorMessage = formatter.toString();
               formatter.close();
               throw new RuntimeException(errorMessage);
           }
       } catch (IOException e) {
           throw new RuntimeException("An error occurred while reading " + featureFile + " Feature file:", e);
       } catch (ProcessingException e) {
           throw new RuntimeException("An error occurred while validating Feature " + featureFile + ", read the log for details:", e);
       }
   }

    /**
     * Process the provided projects.
     * @param env The environment with all maven settings and projects
     */
    public void process(final Environment env) {
        for(final FeatureProjectInfo finfo : env.modelProjects.values()) {
            process(env, finfo, FeatureProjectConfig.getMainConfig(finfo));
            process(env, finfo, FeatureProjectConfig.getTestConfig(finfo));

            ProjectHelper.storeProjectInfo(finfo);

            ProjectHelper.validateFeatureClassifiers(finfo.project);
        }
    }

    /**
     * Process a feature project.
     * This method is invoked twice, once for the main project and then for testing.
     *
     * @param env The environment with all maven settings and projects
     * @param info The project to process.
     * @param config The configuration for the project.
     */
    private void process(final Environment env,
            final FeatureProjectInfo info,
            final FeatureProjectConfig config) {
        if ( (config.isTestConfig() && info.testFeatureDone == true )
             || (!config.isTestConfig() && info.featureDone == true) ) {
            env.logger.debug("Return assembled " + config.getName() + " for " + info.project.getId());
            return;
        }

        // prevent recursion and multiple processing
        if ( config.isTestConfig() ) {
            info.testFeatureDone = true;
        } else {
            info.featureDone = true;
        }
        env.logger.debug("Processing " + config.getName() + " in project " + info.project.getId());

        // read project features
        readProjectFeatures(env.logger, info, config);
        if ( (config.isTestConfig() ? info.testFeatures : info.features).isEmpty() ) {
            env.logger.debug("No " + config.getName() + " found in project " + info.project.getId());
            return;
        }

        // process attachments (only for jar or bundle)
        if ( "jar".equals(info.project.getPackaging())
             || "bundle".equals(info.project.getPackaging())) {
            if ( config.isSkipAddJarToFeature() ) {
                env.logger.debug("Skip adding jar to " + config.getName());
            } else {
                if ( info.features.size() > 1 ) {
                    throw new RuntimeException("Jar can only be added if just one feature is defined in the project");
                }
                final Artifact jar = new Artifact(new ArtifactId(info.project.getGroupId(),
                        info.project.getArtifactId(),
                        info.project.getVersion(),
                        null,
                        "jar"));
                if ( config.getJarStartOrder() != null ) {
                    jar.setStartOrder(Integer.valueOf(config.getJarStartOrder()));
                }
                // add to first feature
                (config.isTestConfig() ? info.testFeatures : info.features).values().iterator().next().getBundles().add(jar);
            }
        }

        // assemble features
        final Map<String, Feature> features = (config.isTestConfig() ? info.testFeatures : info.features);
        final Map<String, Feature> processFeatures = new HashMap<>(features);
        final Map<String, Feature> aggregatedFeatures = (config.isTestConfig() ? info.assembledTestFeatures : info.assembledFeatures);
        while ( aggregatedFeatures.size() < features.size() ) {
        	final int start = aggregatedFeatures.size();

        	final Iterator<Map.Entry<String, Feature>> iter = processFeatures.entrySet().iterator();
        	while ( iter.hasNext() ) {
        		final Map.Entry<String, Feature> entry = iter.next();
        		boolean process = false;
                if ( entry.getValue().getPrototype() == null ) {
        			// no include we can process
        			process = true;
        		} else {
                    final ArtifactId prototype = entry.getValue().getPrototype().getId();
                    if ( !prototype.getGroupId().equals(info.project.getGroupId())
                      || !prototype.getArtifactId().equals(info.project.getArtifactId())
                      || !prototype.getVersion().equals(info.project.getVersion()) ) {
        		    	process = true;
        		    } else {
        		    	// same project
        		    	for(final Feature f : aggregatedFeatures.values()) {
                            if ( f.getId().equals(prototype) ) {
        		    			process = true;
        		    			break;
        		    		}
        		    	}
        		    }
        		}
        		if ( process ) {
        			iter.remove();
    	            final Feature assembledFeature = FeatureBuilder.assemble(entry.getValue(), new BuilderContext(this.createFeatureProvider(env,
    		                info,
    		                config.isTestConfig(),
    		                config.isSkipAddDependencies(),
                                    config.getScope(), null))
                                            .setArtifactProvider(
                                                    aid ->
                                                    {
                                                        try
                                                        {
                                                            return ProjectHelper
                                                                    .getOrResolveArtifact(info.project, env.session,
                                                                            env.artifactHandlerManager, env.resolver, aid)
                                                                    .getFile().toURI().toURL();
                                                        }
                                                        catch (Exception e)
                                                        {
                                                            env.logger.error(e.getMessage(), e);
                                                            return null;
                                                        }
                                                    }));
    	            aggregatedFeatures.put(entry.getKey(), assembledFeature);
    	            break;
        		}
        	}
        	if ( aggregatedFeatures.size() == start ) {
        		throw new RuntimeException("Circular dependency in features in project " + info.project.getId());
        	}
        }

        if ( config.isSkipAddDependencies() ) {
            env.logger.debug("Not adding artifacts from features as dependencies");
        } else {
            for(final Feature f : (config.isTestConfig() ? info.assembledTestFeatures : info.assembledFeatures).values()) {
                addDependenciesFromFeature(env, info, f, config.getScope());
            }
        }
    }

    /**
     * Add all dependencies from the feature
     * @param env The environment
     * @param info The project info
     * @param assembledFeature The assembled feature for finding the artifacts.
     * @param scope The scope which the new dependencies should have
     */
    private void addDependenciesFromFeature(
            final Environment env,
            final FeatureProjectInfo info,
            final Feature assembledFeature,
            final String scope) {
        for(final org.apache.sling.feature.Artifact entry : assembledFeature.getBundles()) {
            this.addDependency(env.logger, info.project, entry.getId(), scope);
        }
        for(final Extension ext : assembledFeature.getExtensions()) {
            if ( ext.getType() != ExtensionType.ARTIFACTS ) {
                continue;
            }
            for(final org.apache.sling.feature.Artifact art : ext.getArtifacts()) {
                this.addDependency(env.logger, info.project, art.getId(), scope);
            }
        }
    }

    /**
     * Read the features for a feature project.
     * The feature is either inlined in the pom or stored in a file in the project.
     *
     * @param logger The logger
     * @param info The project info
     * @param config The configuration
     */
    protected void readProjectFeatures(
            final Logger logger,
            final FeatureProjectInfo info,
            final FeatureProjectConfig config) {
        // feature files first:
        final File dir = new File(info.project.getBasedir(), config.getFeaturesDir());
        if ( dir.exists() ) {
            final List<File> files = new ArrayList<>();
            ProjectHelper.scan(files, dir, config.getIncludes(), config.getExcludes());

            for(final File file : files) {
            	logger.debug("Reading feature file " + file + " in project " + info.project.getId());

                // if the feature is in the root of the configured directory
                // and the feature is named "feature.json"
                // and the feature is not a test feature, this is the main feature
                // which does not get a classifier
                final String suggestedClassifier;
                if (config.isTestConfig() || !file.getName().equals("feature.json")
                        || !file.getParentFile().getAbsolutePath().equals(
                                new File(info.project.getBasedir(), config.getFeaturesDir()).getAbsolutePath())) {
                    final int lastDot = file.getName().lastIndexOf('.');
                    suggestedClassifier = file.getName().substring(0, lastDot);
                } else {
                    suggestedClassifier = null;
                }

                final String readJson = ProjectHelper.readFeatureFile(info.project, file,
                        suggestedClassifier,
                        config.isEnableLegacyVariableReplacement(),
                        config.isEnableProjectVariableReplacement(),
                        config.getReplacePropertyVariables());

                final String json = preprocessFeature(info.project, config.isValidate(),
                        file, readJson);

                try (final Reader reader = new StringReader(json)) {
                    final Feature feature = FeatureJSONReader.read(reader, file.getAbsolutePath());

                    ProjectHelper.checkFeatureId(info.project, feature);

                    ProjectHelper.setFeatureInfo(info.project, feature);
                    this.postProcessReadFeature(feature);
                    (config.isTestConfig() ? info.testFeatures : info.features).put(file.toPath().normalize().toFile().getAbsolutePath(), feature);

                } catch ( final IOException io) {
                    throw new RuntimeException("Unable to read feature " + file.getAbsolutePath(), io);
                }
            }
        } else {
            logger.debug("Feature directory " + config.getFeaturesDir() + " does not exist in project " + info.project.getId());
        }
    }

    protected String preprocessFeature(final MavenProject project, boolean validate, final File file, String json) {

        // validate
        if (validate) {
            checkFeatureFileValidation(file, json);
        }

        return json;
	}

    /**
     * Hook to post process the local feature
     *
     * @param result The read feature
     * @return The post processed feature
     */
    protected Feature postProcessReadFeature(final Feature result)  {
        return result;
    }


    protected FeatureProvider createFeatureProvider(final Environment env,
            final FeatureProjectInfo info,
            final boolean isTest,
            final boolean skipAddDependencies,
            final String dependencyScope,
            final List<Feature> projectFeatures) {
    	final String projectKey = info.project.getGroupId() + ":" + info.project.getArtifactId();
        return new FeatureProvider() {

        	private final Set<ArtifactId> processing = new HashSet<>();

            @Override
            public Feature provide(final ArtifactId id) {
                if ( processing.contains(id) ) {
                    env.logger.error("Unable to get feature " + id.toMvnId() + " : Recursive dependency list including project " + info.project);
                    return null;
                }
                processing.add(id);
                try {
                	// add feature to dependencies
	                if ( !skipAddDependencies ) {
	                    addDependency(env.logger, info.project, id, dependencyScope);
	                }

	                final String key = id.getGroupId() + ":" + id.getArtifactId();
	                if ( projectKey.equals(key) ) {
	                	// this is a feature from the current project
	                	final Feature found = findFeature(info, isTest, id);
	                    if ( found == null ) {
	                        env.logger.error("Unable to find included feature " + id.toMvnId() + " in project " + info.project);
	                        return null;
	                    }
	                    return found;
	                }
	                // if it's a project from the current reactor build, we can't resolve it right now
	                final FeatureProjectInfo depProjectInfo = env.modelProjects.get(key);
	                if ( depProjectInfo != null ) {
	                    env.logger.debug("Found reactor " + id.getType() + " dependency to project: " + id);
	                    if ( isTest ) {
                            process(env, depProjectInfo, FeatureProjectConfig.getTestConfig(depProjectInfo));
	                    } else {
                            process(env, depProjectInfo, FeatureProjectConfig.getMainConfig(depProjectInfo));
	                    }
                        final Feature found = findFeature(depProjectInfo, isTest, id);

	                    if ( isTest && found == null ) {
	                        env.logger.error("Unable to get feature " + id.toMvnId() + " : Recursive test feature dependency list including project " + info.project);
	                    } else if ( !isTest && found == null ) {
	                        env.logger.error("Unable to get feature " + id.toMvnId() + " : Recursive feature dependency list including project " + info.project);
	                    }
	                    return found;
	                } else {
	                    env.logger.debug("Found external " + id.getType() + " dependency: " + id);

	                    // "external" dependency, we can already resolve it
	                    final File featureFile = ProjectHelper.getOrResolveArtifact(info.project, env.session, env.artifactHandlerManager, env.resolver, id).getFile();
	                    try (final FileReader r = new FileReader(featureFile)) {
	                        return FeatureJSONReader.read(r, featureFile.getAbsolutePath());
	                    } catch ( final IOException ioe) {
	                        env.logger.error("Unable to read feature file from " + featureFile, ioe);
	                    }
	                }

	                return null;
                } finally {
                	processing.remove(id);
                }
            }
        };
    }

    private void addDependency(final Logger logger, final MavenProject project, final ArtifactId id, final String scope) {
        if ( id.getGroupId().equals(project.getGroupId())
             && id.getArtifactId().equals(project.getArtifactId())
             && id.getVersion().equals(project.getVersion()) ) {
            // skip artifact from the same project
            logger.debug("- skipping dependency " + id.toMvnId());
        } else {

			boolean found = false;
			for(final Dependency d : project.getDependencies()) {
				if ( d.getGroupId().equals(id.getGroupId()) && d.getArtifactId().equals(id.getArtifactId())) {
					if ( d.getVersion().equals(id.getVersion()) && d.getType().equals(id.getType())) {
						if ( d.getClassifier() == null && id.getClassifier() == null ) {
							found = true;
							break;
						}
						if ( d.getClassifier() != null && d.getClassifier().equals(id.getClassifier())) {
							found = true;
							break;
						}
					}
				}
			}
			if ( !found ) {
				logger.debug("- adding dependency " + id.toMvnId());
				final Dependency dep = ProjectHelper.toDependency(id, scope);

				// Exclude all transitive dependencies coming from the feature model deps
				Exclusion exclusion = new Exclusion();
				exclusion.setGroupId("*");
				exclusion.setArtifactId("*");
				dep.addExclusion(exclusion);

				project.getDependencies().add(dep);
			}
        }
    }

    private Feature findFeature(final FeatureProjectInfo info, final boolean isTest, final ArtifactId id) {
    	Feature found = findFeature(isTest ? info.assembledTestFeatures : info.assembledFeatures, id);
        if ( found == null ) {
        	if ( isTest ) {
        		found = findFeature(info.assembledFeatures, id);
        	}
        }
        return found;
    }

    private Feature findFeature(final Map<String, Feature> featureMap, final ArtifactId id) {
    	Feature found = null;
    	if ( featureMap != null ) {
            for(final Map.Entry<String, Feature> f : featureMap.entrySet()) {
                if ( f.getValue().getId().equals(id) ) {
                    found = f.getValue();
                    break;
                }
            }
    	}
		return found;
    }
}
