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
package org.apache.sling.feature.extension.apiregions.analyser;

import java.io.StringReader;
import java.net.URL;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.jar.Manifest;
import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonReader;
import javax.json.stream.JsonParsingException;

import org.apache.sling.feature.Artifact;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Extension;
import org.apache.sling.feature.Extensions;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.analyser.task.AnalyserTask;
import org.apache.sling.feature.analyser.task.AnalyserTaskContext;
import org.apache.sling.feature.scanner.BundleDescriptor;
import org.apache.sling.feature.scanner.FeatureDescriptor;
import org.apache.sling.feature.scanner.PackageInfo;
import org.apache.sling.feature.scanner.impl.FeatureDescriptorImpl;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public abstract class AbstractApiRegionsAnalyserTaskTest<T extends AbstractApiRegionsAnalyserTask> {

    protected abstract T newTask();

    private AnalyserTask analyserTask;

    @Before
    public void setUp() {
        analyserTask = newTask();
    }

    @After
    public void tearDown() {
        analyserTask = null;
    }

    @Test
    public void testNoApiRegionsExtension() throws Exception {
        List<String> errors = execute((Extension) null);
        assertTrue(errors.isEmpty());
    }

    @Test
    public void testNullApiRegionsJSON() throws Exception {
        List<String> errors = execute((String) null);
        assertTrue(errors.isEmpty());
    }

    @Test
    public void testEmptyApiRegionsJSON() throws Exception {
        List<String> errors = execute("");
        assertTrue(errors.isEmpty());
    }

    @Test
    public void testApiRegionsIsNotJSON() throws Exception {
        List<String> errors = execute("this is not a JSON string");

        assertFalse(errors.isEmpty());
        assertTrue(errors.iterator().next().contains("does not represent a valid JSON 'api-regions'"));
    }

    protected final List<String> execute(String apiRegionJSON) throws Exception {
        Extension extension = mock(Extension.class);
        when(extension.getJSON()).thenReturn(apiRegionJSON);
        if (apiRegionJSON != null) {
            JsonReader reader = Json.createReader(new StringReader(apiRegionJSON));
            JsonArray array = null;
            try {
                array = reader.readArray();
            } catch (final JsonParsingException ignore) {

            }
            when(extension.getJSONStructure()).thenReturn(array);
        }
        return execute(extension);
    }

    protected final List<String> execute(Extension extension) throws Exception {
        Extensions extensions = mock(Extensions.class);
        when(extensions.getByName("api-regions")).thenReturn(extension);

        Feature feature = mock(Feature.class);
        when(feature.getId()).thenReturn(new ArtifactId("org.apache.sling.testing",
                                                        "org.apache.sling.testing.apiregions",
                                                        "1.0.0",
                                                        null,
                                                        null));
        when(feature.getExtensions()).thenReturn(extensions);

        AnalyserTaskContext ctx = mock(AnalyserTaskContext.class);
        when(ctx.getFeature()).thenReturn(feature);
        @SuppressWarnings("unchecked")
        Map<String,String> cfg = Mockito.mock(Map.class);
        when(ctx.getConfiguration()).thenReturn(cfg);
        when(cfg.getOrDefault(anyString(), anyString())).thenAnswer(new Answer<String>() {

            @Override
            public String answer(InvocationOnMock invocation) throws Throwable {
                Object[] args = invocation.getArguments();
                return (String) args[1];
            }

        });

        PackageInfo packageInfo = new PackageInfo("org.osgi.util.function", "1.0", false, Collections.singleton("org.objectweb.asm"));

        BundleDescriptor bundleDescriptor = new TestBundleDescriptor();
        bundleDescriptor.getExportedPackages().add(packageInfo);

        FeatureDescriptor featureDescriptor = new FeatureDescriptorImpl(feature);
        featureDescriptor.getBundleDescriptors().add(bundleDescriptor);

        when(ctx.getFeatureDescriptor()).thenReturn(featureDescriptor);

        List<String> errors = new LinkedList<>();
        doAnswer(invocation -> {
            String error = invocation.getArgument(0);
            errors.add(error);
            return null;
        }).when(ctx).reportError(anyString());

        analyserTask.execute(ctx);

        return errors;
    }

    private static final class TestBundleDescriptor extends BundleDescriptor {
        TestBundleDescriptor() {
            super("org.osgi:org.osgi.util.function:1.0.0");
        }

        @Override
        public URL getArtifactFile() {
            // do nothing
            return null;
        }

        @Override
        public Artifact getArtifact() {
            return new Artifact(ArtifactId.fromMvnId("org.osgi:org.osgi.util.function:1.0.0"));
        }

        @Override
        public Manifest getManifest() {
            // do nothing
            return null;
        }

        @Override
        public String getBundleVersion() {
            // do nothing
            return null;
        }

        @Override
        public String getBundleSymbolicName() {
            // do nothing
            return null;
        }

        @Override
        public int getBundleStartLevel() {
            // do nothing
            return 0;
        }

    }

}
