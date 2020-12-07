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

import org.apache.commons.io.FileUtils;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.jar.JarInputStream;
import java.util.jar.JarOutputStream;
import java.util.jar.Manifest;
import java.util.zip.CRC32;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

class JarDecompressor {
    private static final int BUFFER_SIZE = 16384;

    static void copyDecompress(File in, File out) throws IOException {
        if (decompress(in, out))
            return;

        // It's a signed jar, just copy it as-is...
        FileUtils.copyFile(in, out);
    }

    // Returns false if the input file is a signed jar which should not be decompressed
    private static boolean decompress(File in, File out) throws IOException {
        try (JarInputStream jis = new JarInputStream(new FileInputStream(in));
            ZipOutputStream jos = new JarOutputStream(new FileOutputStream(out))) {
            jos.setMethod(ZipOutputStream.STORED);

            writeManifestIfPresent(jis, jos);

            byte[] buffer = new byte[BUFFER_SIZE];
            JarEntry je = null;
            while ((je = jis.getNextJarEntry()) != null) {
                if (JarFile.MANIFEST_NAME.equals(je.getName()))
                    continue;

                if (je.getName().startsWith("META-INF/") &&
                    je.getName().endsWith(".SF")) {
                    // This is a signed jar, don't decompress it.
                    return false;
                }

                // Put the jar entry in a temp file because we need to read it twice
                // once to compute the CRC and once again to write it to the Jar file
                File tmpFile = Files.createTempFile("tempfile", ".tmp").toFile();
                try (FileOutputStream fos = new FileOutputStream(tmpFile)) {
                    drainStream(jis, fos, buffer);
                }

                // If the embedded file is a jar file, also decompress that one
                if (je.getName().toLowerCase().endsWith(".jar")) {
                    File tmpFile2 = tmpFile;
                    tmpFile = Files.createTempFile("tempfile", ".tmp").toFile();
                    copyDecompress(tmpFile2, tmpFile);

                    if (!tmpFile2.delete()) {
                        throw new IOException("Could not delete temp file " + tmpFile);
                    }
                }

                ZipEntry ze = new ZipEntry(je);
                crcEntry(ze, tmpFile, buffer);
                jos.putNextEntry(ze);

                try (InputStream is = new FileInputStream(tmpFile)) {
                    drainStream(is, jos, buffer);
                }
                jos.closeEntry();

                if (!tmpFile.delete()) {
                    throw new IOException("Could not delete temp file " + tmpFile);
                }

            }
        }
        return true;
    }

    private static void drainStream(InputStream is, OutputStream os, byte[] buf) throws IOException {
        int sz = 0;
        while ((sz = is.read(buf)) > 0) {
            os.write(buf, 0, sz);
        }
    }

    private static void crcEntry(ZipEntry ze, File f, byte[] buf) throws IOException {
        try (SizeAndCrcComputeStream cs = new SizeAndCrcComputeStream()) {
            try (InputStream is = new FileInputStream(f)) {
                drainStream(is, cs, buf);
            }
            cs.updateZipEntry(ze);
        }
    }

    private static void crcEntryManifestEntry(ZipEntry ze, Manifest mf) throws IOException {
        SizeAndCrcComputeStream cs = new SizeAndCrcComputeStream();
        mf.write(cs);
        cs.updateZipEntry(ze);
    }

    private static class SizeAndCrcComputeStream extends OutputStream {
        final CRC32 crc = new CRC32();
        long sizeCounter = 0;

        public void write(int r) throws IOException {
            sizeCounter++;
            crc.update(r);
        }

        public void write(byte[] b, int off, int len) throws IOException {
            sizeCounter += len;
            crc.update(b, off, len);
        }

        public void updateZipEntry(ZipEntry e) {
            e.setSize(sizeCounter);
            e.setCompressedSize(sizeCounter);
            e.setCrc(crc.getValue());
            e.setMethod(ZipEntry.STORED);
        }
    }

    private static void writeManifestIfPresent(JarInputStream jis, ZipOutputStream jos) throws IOException {
        Manifest manifest = jis.getManifest();
        if (manifest == null)
            return;

        ZipEntry e = new ZipEntry(JarFile.MANIFEST_NAME);
        crcEntryManifestEntry(e, manifest);
        jos.putNextEntry(e);
        manifest.write(new BufferedOutputStream(jos));
        jos.closeEntry();
    }
}
