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
package org.apache.sling.feature.maven.mojos.apis;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Collection;

import org.apache.commons.exec.CommandLine;
import org.apache.commons.exec.DefaultExecutor;
import org.apache.commons.lang3.SystemUtils;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.logging.Log;
import org.codehaus.plexus.util.StringUtils;

public final class JavadocExecutor {

    private static final char QUOTE_CHAR = '"';

    private final File argFile;

    private final PrintWriter argFileWriter;

    public JavadocExecutor(File regionDir) throws MojoExecutionException {
        argFile = new File(regionDir, regionDir.getName() + "-javadoc");
        try {
            argFileWriter = new PrintWriter(argFile);
        } catch (FileNotFoundException e) {
            throw new MojoExecutionException("Impossible to create the javadoc arg file on " + argFile, e);
        }
    }

    public JavadocExecutor addArgument(String value) {
        return this.addArgument(value, true);
    }

    public <T> JavadocExecutor addArgument(String stringValue, boolean newLine) {
        if (newLine) {
            argFileWriter.println(stringValue);
        } else {
            argFileWriter.print(stringValue);
            argFileWriter.print(' ');
        }
        return this;
    }

    public <T> JavadocExecutor addQuotedArgument(String value) {
        return addArgument(StringUtils.quoteAndEscape(value, QUOTE_CHAR));
    }

    public <T> JavadocExecutor addArgument(String[] value, String valueSeparator) {
        return addArgument(StringUtils.join(value, valueSeparator));
    }

    public JavadocExecutor addArgument(Collection<String> value, String valueSeparator) {
        return addArgument(StringUtils.join(value.iterator(), valueSeparator));
    }

    public JavadocExecutor addArguments(Collection<String> value) {
        for (String current : value) {
            addArgument(current);
        }
        return this;
    }

    public JavadocExecutor addArguments(String key, Collection<String> value) {
        for (String current : value) {
            addArgument(key, false);
            addArgument(current);
        }
        return this;
    }

    public void execute(File workingDir, Log logger, boolean ignoreErrors) throws MojoExecutionException {
        argFileWriter.close();

        CommandLine javadocCommand = new CommandLine(getJavadocExecutable());
        javadocCommand.addArgument('@' + argFile.getAbsolutePath(), false);

        logger.info("Executing javadoc tool: " + javadocCommand);

        DefaultExecutor executor = new DefaultExecutor();
        executor.setWorkingDirectory(workingDir);
        if (ignoreErrors) {
            executor.setExitValues(new int[] { 1, 0, -1 });
        }
        try {
            executor.getStreamHandler().setProcessInputStream(new LoggerOutputStream(logger));
            final int exitValue = executor.execute(javadocCommand);
            if (!ignoreErrors && executor.isFailure(exitValue)) {
                throw new MojoExecutionException("Javadoc generation failed. See log for more details.");
            }
        } catch (IOException ioe) {
            throw new MojoExecutionException("Javadoc generation failed. See log for more details.", ioe);
        }
    }

    /**
     * Try to find javadocExe from System.getProperty( "java.home" ) By default,
     * System.getProperty( "java.home" ) = JRE_HOME and JRE_HOME should be in the
     * JDK_HOME.
     *
     * @return
     */
    private static File getJavadocExecutable() throws MojoExecutionException {
        String javadocCommand = "javadoc" + (SystemUtils.IS_OS_WINDOWS ? ".exe" : "");

        File javadocExe;

        // For IBM's JDK 1.2
        if (SystemUtils.IS_OS_AIX) {
            javadocExe = getFile(SystemUtils.getJavaHome(), "..", "sh", javadocCommand);
        }
        // For Apple's JDK 1.6.x (and older?) on Mac OSX
        else if (SystemUtils.IS_OS_MAC_OSX && org.apache.commons.lang.SystemUtils.JAVA_VERSION_FLOAT < 1.7f) {
            javadocExe = getFile(SystemUtils.getJavaHome(), "bin", javadocCommand);
        } else {
            javadocExe = getFile(SystemUtils.getJavaHome(), "..", "bin", javadocCommand);
        }

        // ----------------------------------------------------------------------
        // Try to find javadocExe from JAVA_HOME environment variable
        // ----------------------------------------------------------------------
        if (!javadocExe.exists() || !javadocExe.isFile()) {
            String javaHome = System.getenv().get("JAVA_HOME");
            if (StringUtils.isEmpty(javaHome)) {
                throw new MojoExecutionException("The environment variable JAVA_HOME is not correctly set.");
            }

            File javaHomeDir = new File(javaHome);
            if ((!javaHomeDir.exists()) || javaHomeDir.isFile()) {
                throw new MojoExecutionException("The environment variable JAVA_HOME=" + javaHome
                        + " doesn't exist or is not a valid directory.");
            }

            javadocExe = getFile(javaHomeDir, "bin", javadocCommand);
        }

        if (!javadocExe.exists() || !javadocExe.isFile()) {
            throw new MojoExecutionException("The javadoc executable '" + javadocExe
                    + "' doesn't exist or is not a file. Verify the JAVA_HOME environment variable.");
        }

        return javadocExe;
    }

    private static File getFile(File parent, String... path) {
        File tmp = parent;
        for (String current : path) {
            tmp = new File(tmp, current);
        }
        return tmp;
    }

    private static final class LoggerOutputStream extends OutputStream {

        private final StringBuilder output = new StringBuilder();

        private final Log log;

        public LoggerOutputStream(Log log) {
            this.log = log;
        }

        @Override
        public void write(int data) throws IOException {
            output.append((char) data);
            if ('\n' == data) {
                flush();
            }
        }

        @Override
        public void flush() throws IOException {
            log.info(output);
            output.setLength(0);
        }

        @Override
        public void close() throws IOException {
            flush();
            super.close();
        }

    }

}
