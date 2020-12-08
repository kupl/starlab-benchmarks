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
// DWB20: ThreadIO should check and reset IO if something (e.g. jetty) overrides
package org.apache.felix.gogo.runtime.threadio;

import java.io.InputStream;
import java.io.PrintStream;
import java.util.logging.Logger;

import org.apache.felix.service.threadio.ThreadIO;

public class ThreadIOImpl implements ThreadIO
{
    static private final Logger log = Logger.getLogger(ThreadIOImpl.class.getName());

    final Marker defaultMarker = new Marker(System.in, System.out, System.err, null);
    final ThreadPrintStream err = new ThreadPrintStream(this, System.err, true);
    final ThreadPrintStream out = new ThreadPrintStream(this, System.out, false);
    final ThreadInputStream in = new ThreadInputStream(this, System.in);
    final ThreadLocal<Marker> current = new InheritableThreadLocal<Marker>()
    {
        @Override
        protected Marker initialValue()
        {
            return defaultMarker;
        }
    };

    public void start()
    {
        if (System.out instanceof ThreadPrintStream)
        {
            throw new IllegalStateException("Thread Print Stream already set");
        }
        System.setOut(out);
        System.setIn(in);
        System.setErr(err);
    }

    public void stop()
    {
        System.setErr(defaultMarker.err);
        System.setOut(defaultMarker.out);
        System.setIn(defaultMarker.in);
    }

    private void checkIO()
    { // derek
        if (System.in != in)
        {
            log.fine("ThreadIO: eek! who's set System.in=" + System.in);
            System.setIn(in);
        }

        if (System.out != out)
        {
            log.fine("ThreadIO: eek! who's set System.out=" + System.out);
            System.setOut(out);
        }

        if (System.err != err)
        {
            log.fine("ThreadIO: eek! who's set System.err=" + System.err);
            System.setErr(err);
        }
    }

    Marker current()
    {
        Marker m = current.get();
        if (m.deactivated)
        {
            while (m.deactivated)
            {
                m = m.previous;
            }
            current.set(m);
        }
        return m;
    }

    public void close()
    {
        checkIO(); // derek
        Marker top = this.current.get();
        if (top == null)
        {
            throw new IllegalStateException("No thread io active");
        }
        if (top != defaultMarker)
        {
            top.deactivate();
            this.current.set(top.previous);
        }
    }

    public void setStreams(InputStream in, PrintStream out, PrintStream err)
    {
        assert in != null;
        assert out != null;
        assert err != null;
        checkIO(); // derek
        Marker prev = current();
        if (in == this.in) {
            in = prev.getIn();
        }
        if (out == this.out) {
            out = prev.getOut();
        }
        if (err == this.err) {
            err = prev.getErr();
        }
        Marker marker = new Marker(in, out, err, prev);
        this.current.set(marker);
    }
}
