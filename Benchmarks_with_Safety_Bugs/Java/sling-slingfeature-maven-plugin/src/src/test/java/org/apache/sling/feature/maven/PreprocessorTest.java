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
package org.apache.sling.feature.maven;

import org.apache.maven.model.Dependency;
import org.apache.maven.model.Exclusion;
import org.apache.maven.project.MavenProject;
import org.apache.sling.feature.ArtifactId;
import org.codehaus.plexus.logging.Logger;
import org.junit.Test;
import org.mockito.Mockito;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.assertEquals;

public class PreprocessorTest {
    @Test
    public void testDependencyExclusions() throws Exception {
        Preprocessor p = new Preprocessor();

        List<Dependency> deplist = new ArrayList<>();

        Logger logger = Mockito.mock(Logger.class);
        MavenProject project = Mockito.mock(MavenProject.class);
        Mockito.when(project.getDependencies()).thenReturn(deplist);

        ArtifactId id = ArtifactId.fromMvnId("g:a:7");
        String scope = "provided";

        assertEquals("Precondition", 0, deplist.size());

        Method m = getPrivateMethod(p, "addDependency");
        m.invoke(p, logger, project, id, scope);

        assertEquals(1, deplist.size());
        Dependency dep = deplist.iterator().next();
        assertEquals("g", dep.getGroupId());
        assertEquals("a", dep.getArtifactId());
        assertEquals("7", dep.getVersion());
        assertEquals(1, dep.getExclusions().size());
        Exclusion ex = dep.getExclusions().iterator().next();
        assertEquals("*", ex.getGroupId());
        assertEquals("*", ex.getArtifactId());
    }

    private Method getPrivateMethod(Object o, String name) {
        for (Method m : o.getClass().getDeclaredMethods()) {
            if (m.getName().equals(name)) {
                m.setAccessible(true);
                return m;
            }
        }
        return null;
    }
}
