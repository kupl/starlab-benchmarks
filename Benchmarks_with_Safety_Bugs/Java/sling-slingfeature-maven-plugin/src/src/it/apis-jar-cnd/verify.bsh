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

import java.io.*;
import java.util.*;
import java.util.jar.*;

import org.codehaus.plexus.util.*;

    boolean check() throws Exception {
        File apisJarDir = new File(basedir, "target/apis-jars");

        // base

        File baseApiJar = new File(apisJarDir, "slingfeature-maven-plugin-test-1.0.0-SNAPSHOT-global-apis.jar");
        if (!baseApiJar.exists()) {
            System.out.println("FAILED!");
            System.out.println("File '" + baseApiJar + "' not found");
            return false;
        }

        JarFile jarFile = null;
        // cannot use try..finally due to https://github.com/beanshell/beanshell/issues/367
        jarFile = new JarFile(baseApiJar);
        Manifest manifest= jarFile.getManifest();
        String nodeTypes = manifest.getMainAttributes().getValue("Sling-Nodetypes");

        for (String expectedEntry : nodeTypes.split(",")) {
            if (jarFile.getJarEntry(expectedEntry) == null) {
                System.out.println("FAILED!");
                System.out.println("Entry '" + expectedEntry + "' does not exist in bundle '" + baseApiJar + "' but it is referenced from Sling-Nodetypes");
                jarFile.close();
                return false;
            }
        }
        jarFile.close();
        return true;
    }

    try {
        return check();
    } catch(Throwable t) {
        t.printStackTrace();
        return false;
    }

