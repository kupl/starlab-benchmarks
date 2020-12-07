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

import org.apache.maven.artifact.Artifact;

public class FeatureProjectConfig {

    public static final String CFG_FEATURES = "features";

    public static final String CFG_FEATURES_INCLUDES = "featuresIncludes";

    public static final String CFG_FEATURES_EXCLUDES = "featuresExcludes";

    public static final String CFG_TEST_FEATURES = "testFeatures";

    public static final String CFG_TEST_FEATURES_INCLUDES = "testFeaturesIncludes";

    public static final String CFG_TEST_FEATURES_EXCLUDES = "testFeaturesExcludes";

    public static final String CFG_SKIP_ADD_FEATURE_DEPENDENCIES = "skipAddFeatureDependencies";

    public static final String CFG_SKIP_ADD_TEST_FEATURE_DEPENDENCIES = "skipAddTestFeatureDependencies";

    public static final String CFG_SKIP_ADD_JAR_TO_FEATURE = "skipAddJarToFeature";

    public static final String CFG_SKIP_ADD_JAR_TO_TEST_FEATURE = "skipAddJarToTestFeature";

    public static final String CFG_JAR_START_ORDER = "jarStartOrder";

    public static final String CFG_VALIDATE_FEATURES = "validateFeatures";

    public static final String DEFAULT_FEATURE_DIR = "src/main/features";

    public static final String DEFAULT_FEATURE_INCLUDES = "**/*.json";

    public static final String DEFAULT_TEST_FEATURE_DIR = "src/test/features";

    public static final String CFG_ENABLE_PROJ_VARS = "enableProjectVariableReplacement";

    public static final String CFG_REPLACE_PROP_VARS = "replacePropertyVariables";

    public static final String CFG_LEGACY_REPLACE = "enableLegacyVariableReplacement";


    private final String featuresDirName;

    private final String includes;

    private final String excludes;

    private final boolean skipAddDep;

    private final String name;

    private final String scope;

    private final boolean isTest;

    private final String jarStartOrder;

    private final boolean skipAddJar;

    private final boolean validate;

    private final boolean enableProjectVariableReplacement;

    private final String[] replacePropertyVariables;

    private final boolean enableLegacyVariableReplacement;

    public static FeatureProjectConfig getMainConfig(final FeatureProjectInfo info) {
        return new FeatureProjectConfig(info, false);
    }

    public static FeatureProjectConfig getTestConfig(final FeatureProjectInfo info) {
        return new FeatureProjectConfig(info, true);
    }

    private FeatureProjectConfig(final FeatureProjectInfo info, final boolean test) {
        this.isTest = test;
        final String featuresDirCfgName;
        final String defaultDir;
        final String skipAddDepCfgName;
        final String defaultSkipValue;
        final String incCfgName;
        final String exCfgName;
        if ( test ) {
            featuresDirCfgName = CFG_TEST_FEATURES;
            defaultDir = DEFAULT_TEST_FEATURE_DIR;
            this.scope = Artifact.SCOPE_TEST;
            skipAddDepCfgName = CFG_SKIP_ADD_TEST_FEATURE_DEPENDENCIES;
            incCfgName = CFG_TEST_FEATURES_INCLUDES;
            exCfgName = CFG_TEST_FEATURES_EXCLUDES;
            defaultSkipValue = "true";
            this.name = "test feature";
            this.skipAddJar = "true".equals(ProjectHelper.getConfigValue(info.plugin, CFG_SKIP_ADD_JAR_TO_TEST_FEATURE, "true"));
        } else {
            featuresDirCfgName = CFG_FEATURES;
            defaultDir = DEFAULT_FEATURE_DIR;
            this.scope = Artifact.SCOPE_PROVIDED;
            skipAddDepCfgName = CFG_SKIP_ADD_FEATURE_DEPENDENCIES;
            incCfgName = CFG_FEATURES_INCLUDES;
            exCfgName = CFG_FEATURES_EXCLUDES;
            defaultSkipValue = "false";
            this.name = "feature";
            this.skipAddJar = "true".equals(ProjectHelper.getConfigValue(info.plugin, CFG_SKIP_ADD_JAR_TO_FEATURE, "true"));
        }
        this.featuresDirName = ProjectHelper.getConfigValue(info.plugin, featuresDirCfgName, defaultDir);
        this.includes = ProjectHelper.getConfigValue(info.plugin, incCfgName, DEFAULT_FEATURE_INCLUDES);
        this.excludes = ProjectHelper.getConfigValue(info.plugin, exCfgName, null);
        final String skipCfg = ProjectHelper.getConfigValue(info.plugin, skipAddDepCfgName, defaultSkipValue);
        this.skipAddDep = "true".equals(skipCfg.toLowerCase());
        this.jarStartOrder = ProjectHelper.getConfigValue(info.plugin, CFG_JAR_START_ORDER, null);
        this.validate = "true".equals(ProjectHelper.getConfigValue(info.plugin, CFG_VALIDATE_FEATURES, "true"));
        this.enableProjectVariableReplacement = "true".equals(ProjectHelper.getConfigValue(info.plugin, CFG_ENABLE_PROJ_VARS, "true"));
        String vars = ProjectHelper.getConfigValue(info.plugin, CFG_REPLACE_PROP_VARS, null);
        if ( vars == null ) {
            this.replacePropertyVariables = null;
        } else {
            this.replacePropertyVariables = vars.split(",");
        }
        this.enableLegacyVariableReplacement = "true".equals(ProjectHelper.getConfigValue(info.plugin, CFG_LEGACY_REPLACE, "false"));
    }

    public String getName() {
        return this.name;
    }

    public String getFeaturesDir() {
        return this.featuresDirName;
    }

    public String getIncludes() {
        return this.includes;
    }

    public String getExcludes() {
        return this.excludes;
    }

    public boolean isSkipAddDependencies() {
        return this.skipAddDep;
    }

    public String getScope() {
        return this.scope;
    }

    public boolean isTestConfig() {
        return this.isTest;
    }

    public String getJarStartOrder() {
        return this.jarStartOrder;
    }

    public boolean isSkipAddJarToFeature() {
        return this.skipAddJar;
    }

    public boolean isValidate() {
        return this.validate;
    }

    public boolean isEnableProjectVariableReplacement() {
        return enableProjectVariableReplacement;
    }

    public String[] getReplacePropertyVariables() {
        return replacePropertyVariables;
    }

    public boolean isEnableLegacyVariableReplacement() {
        return enableLegacyVariableReplacement;
    }
}

