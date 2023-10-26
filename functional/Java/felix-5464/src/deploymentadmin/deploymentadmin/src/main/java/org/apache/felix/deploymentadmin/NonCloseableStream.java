/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
package org.apache.felix.deploymentadmin;

import java.io.IOException;
import java.io.InputStream;

/**
 * Stream that does nothing when close() is invoked, calls to one of the read methods will throw an <code>IOException</code>
 * after close() is called. Also, mark/reset is not supported. Deployment Admin can use this class to pass on as an <code>InputStream</code>
 * to a resource processor.
 * 
 */
public class NonCloseableStream extends InputStream {
    private final InputStream m_input;
    private volatile boolean m_closed;

    public NonCloseableStream(InputStream input) {
        m_input = input;
    }

    public int available() throws IOException {
        return m_input.available();
    }

    public void close() throws IOException {
        // stream should not be actually closed, subsequent calls to read methods will throw an exception though
        assertOpen();
        m_closed = true;
    }

    public boolean equals(Object obj) {
        return m_input.equals(obj);
    }

    public int hashCode() {
        return m_input.hashCode();
    }

    public void mark(int readlimit) {
        // do nothing
    }

    public boolean markSupported() {
        return false;
    }

    public int read() throws IOException {
        return m_input.read();
    }

    public int read(byte[] b) throws IOException {
        assertOpen();
        return m_input.read(b);
    }

    public int read(byte[] b, int off, int len) throws IOException {
        assertOpen();
        return m_input.read(b, off, len);
    }

    public void reset() throws IOException {
        throw new IOException("Mark and reset are not available on this type of stream.");
    }

    public long skip(long n) throws IOException {
        return m_input.skip(n);
    }

    public String toString() {
        return m_input.toString();
    }

    private void assertOpen() throws IOException {
        if (m_closed) {
            throw new IOException("Unable to read, stream is closed.");
        }
    }
}
