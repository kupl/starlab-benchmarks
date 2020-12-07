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

import static org.junit.Assert.assertEquals;

import java.util.Properties;

import org.apache.maven.project.MavenProject;
import org.junit.Test;

public class SubstitutionTest {
    @Test
    public void testReplaceMavenVars() {
        MavenProject proj = new MavenProject();
        Properties p = proj.getProperties();
        p.put("test", "foo");
        assertEquals("hellofoogoodbyefoo", Substitution.replaceMavenVars(proj, false, false, new String[] {"test"}, "hello${test}goodbye${test}"));
        assertEquals("hello${test}goodbye${test}", Substitution.replaceMavenVars(proj, false, false, null, "hello${test}goodbye${test}"));
    }

    @Test
    public void testReplaceMavenVarsWithSystemProperties() {
        Properties storedProps = new Properties();
        storedProps.putAll(System.getProperties());

        try {
            MavenProject proj = new MavenProject();
            Properties p = proj.getProperties();
            p.put("test", "foo");

            System.setProperty("test", "bar");

            assertEquals("hellobargoodbyebar", Substitution.replaceMavenVars(proj, true, false, null, "hello${test}goodbye${test}"));
        } finally {
            // Restore the system properties
            System.setProperties(storedProps);
        }
    }

    @Test
    public void testOSGiVersion() {
    	assertEquals("1.2.3", Substitution.getOSGiVersion("1.2.3"));
    	assertEquals("1.2.0.SNAPSHOT", Substitution.getOSGiVersion("1.2-SNAPSHOT"));
    	assertEquals("4.5.6.SNAPSHOT", Substitution.getOSGiVersion("4.5.6-SNAPSHOT"));
    	assertEquals("6.5.0.some", Substitution.getOSGiVersion("6.5.0-some"));
    }
}
