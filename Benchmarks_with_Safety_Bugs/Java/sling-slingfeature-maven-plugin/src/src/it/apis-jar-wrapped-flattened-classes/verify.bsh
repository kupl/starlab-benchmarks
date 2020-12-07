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

        File baseApiJar = new File(apisJarDir, "slingfeature-maven-plugin-test-1.0.0-SNAPSHOT-base-apis.jar");
        if (!baseApiJar.exists()) {
            System.out.println("FAILED!");
            System.out.println("File '" + baseApiJar + "' not found");
            return false;
        }

        InputStream bytecodeStream = null;
        JarFile jarFile = null;
        try {
            jarFile = new JarFile(baseApiJar);
            Manifest manifest= jarFile.getManifest();
            String exportPackageHeader = manifest.getMainAttributes().getValue("Export-Package");

            if (exportPackageHeader.indexOf("org.apache.lucene.analysis.standard") < 0) {
                System.out.println("FAILED!");
                System.out.println("Export-Package header '" + exportPackageHeader + "' does not contain 'org.apache.lucene.analysis.standard' in bundle " + baseApiJar);
                return false;
            }

            JarEntry jarEntry = jarFile.getJarEntry("org/apache/lucene/analysis/standard/StandardAnalyzer.class");
            if (jarEntry == null) {
                System.out.println("FAILED!");
                System.out.println("Entry 'org/apache/lucene/analysis/standard/StandardAnalyzer.class' does not exist in bundle " + file);
            }

            bytecodeStream = jarFile.getInputStream(jarEntry);
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            byte[] buf = new byte[1024];
            int len = 0;
            while ((len = bytecodeStream.read(buf)) >= 0) {
                baos.write(buf, 0, len);
            }

            if (!baos.toString().contains("#(Lorg/apache/lucene/util/Version;)V")) {
                System.out.println("FAILED!");
                System.out.println("Expected org.apache.lucene.analysis.standard.StandardAnalyzer class containing a constructor with 'org.apache.lucene.util.Version' argument");
                return false;
            }
        } finally {
            if (jarFile != null) {
                jarFile.close();
            }

            if (bytecodeStream != null) {
                bytecodeStream.close();
            }
        }

        return true;
    }

    try {
        return check();
    } catch(Throwable t) {
        t.printStackTrace();
        return false;
    }

    return true;
