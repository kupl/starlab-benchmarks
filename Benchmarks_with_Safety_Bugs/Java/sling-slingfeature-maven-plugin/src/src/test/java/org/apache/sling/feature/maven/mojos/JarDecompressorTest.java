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
package org.apache.sling.feature.maven.mojos;

import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.util.HashMap;
import java.util.Map;
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;
import java.util.jar.Manifest;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class JarDecompressorTest {
    @Test
    public void testDecompress() throws Exception {
        File cj = new File(getClass().getResource("/repository/compressed.jar").getFile());
        File uj = Files.createTempFile(getClass().getSimpleName(), "uncompressed.jar").toFile();

        try {
            JarDecompressor.copyDecompress(cj, uj);

            assertJarsEqual(cj, uj);
            assertTrue("Decompressed jar should be bigger", uj.length() > cj.length());
        } finally {
            assertTrue(uj.delete());
        }
    }

    @Test
    public void testSignedJar() throws Exception {
        File cj = new File(getClass().getResource("/repository/compressed2-signed.jar").getFile());
        File uj = Files.createTempFile(getClass().getSimpleName(), "still-compressed.jar").toFile();

        try {
            JarDecompressor.copyDecompress(cj, uj);

            assertJarsEqual(cj, uj);
            assertEquals("Signeds jar should not be decompressed", uj.length(), cj.length());
        } finally {
            assertTrue(uj.delete());
        }

    }

    @Test
    public void testJarWithEmbeddedJar() throws Exception {
        File cj = new File(getClass().getResource("/repository/compressed-embedded.jar").getFile());
        File uj = Files.createTempFile(getClass().getSimpleName(), "embedded-uncompressed.jar").toFile();

        try {
            JarDecompressor.copyDecompress(cj, uj);

            Map<String, byte[]> actualJar = readJar(uj);

            File tf = new File(getClass().getResource("/repository/textfile.txt").getFile());
            assertEquals(tf.length(), actualJar.get("textfile.txt").length);
            File cf = new File(getClass().getResource("/repository/compressed.jar").getFile());
            assertTrue("Embedded compressed jar should be decompressed",
                    cf.length() < actualJar.get("compressed.jar").length);
            assertTrue("Embedded compressed jar should be decompressed",
                    cf.length() < actualJar.get("com/foo/res/compressed.jar").length);
            File sf = new File(getClass().getResource("/repository/compressed2-signed.jar").getFile());
            assertEquals("Signed jar should not be decompressed",
                    sf.length(), actualJar.get("compressed2-signed.jar").length);
        } finally {
            assertTrue(uj.delete());
        }
    }

    private void assertJarsEqual(File cj, File uj) throws IOException {
        Map<String, byte[]> expectedJar = readJar(cj);
        Map<String, byte[]> actualJar = readJar(uj);

        assertEquals(expectedJar.size(), actualJar.size());
        for (Map.Entry<String, byte[]> entry : expectedJar.entrySet()) {
            byte[] actual = actualJar.get(entry.getKey());
            assertArrayEquals(entry.getValue(), actual);
        }
    }

    private Map<String, byte[]> readJar(File jar) throws IOException {
        Map<String, byte[]> map = new HashMap<>();

        try (JarInputStream jis = new JarInputStream(new FileInputStream(jar))) {
            ByteArrayOutputStream mfBaos = new ByteArrayOutputStream();
            Manifest manifest = jis.getManifest();
            manifest.write(mfBaos);
            map.put("META-INF/MANIFEST.MF", mfBaos.toByteArray());

            JarEntry je = null;
            while ((je = jis.getNextJarEntry()) != null) {
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                drainStream(jis, baos);
                map.put(je.getName(), baos.toByteArray());
            }
        }
        return map;
    }

    private static void drainStream(InputStream is, OutputStream os) throws IOException {
        byte[] buf = new byte[16384];
        int sz = 0;
        while ((sz = is.read(buf)) > 0) {
            os.write(buf, 0, sz);
        }
    }
}
