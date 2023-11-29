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
package org.apache.sling.feature.extension.apiregions.launcher;

import java.io.File;
import java.io.IOException;
import java.io.UncheckedIOException;

import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.builder.ArtifactProvider;
import org.apache.sling.feature.launcher.impl.launchers.FrameworkLauncher;
import org.apache.sling.feature.launcher.spi.LauncherPrepareContext;
import org.apache.sling.feature.launcher.spi.LauncherRunContext;

public class RegionLauncher extends FrameworkLauncher
{
    public static final String IDBSNVER_FILENAME = "idbsnver.properties";
    public static final String BUNDLE_FEATURE_FILENAME = "bundles.properties";

    @Override
    public void prepare(LauncherPrepareContext context, ArtifactId frameworkId, Feature app) throws Exception
    {
        super.prepare(context, frameworkId, app);

        ArtifactProvider artifactProvider = id ->
        {
            try
            {
                return context.getArtifactFile(id);
            }
            catch (IOException e)
            {
                throw new UncheckedIOException(e);
            }
        };


        final File base = File.createTempFile("apiregions", ".properties");
        base.delete();
        base.mkdirs();
        File idbsnverFile = new File(base, IDBSNVER_FILENAME);
        File bundlesFile = new File(base, BUNDLE_FEATURE_FILENAME);

        LauncherProperties.save(LauncherProperties.getBundleIDtoBSNandVersionMap(app, artifactProvider), idbsnverFile);
        LauncherProperties.save(LauncherProperties.getBundleIDtoFeaturesMap(app), bundlesFile);

        app.getFrameworkProperties().put("sling.feature.apiregions.resource." + IDBSNVER_FILENAME, idbsnverFile.toURI().toURL().toString());
        app.getFrameworkProperties().put("sling.feature.apiregions.resource." + BUNDLE_FEATURE_FILENAME, bundlesFile.toURI().toURL().toString());
    }

    @Override
    public int run(LauncherRunContext context, ClassLoader cl) throws Exception
    {
        return super.run(context, cl);
    }
}
