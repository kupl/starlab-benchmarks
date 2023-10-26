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
package org.apache.felix.service.command;

import java.util.List;

public interface Job
{

    class Utils {
        /**
         * Get the job running in the current thread or null.
         */
        public static Job current() {
            Process p = Process.Utils.current();
            Job j = p != null ? p.job() : null;
            while (j != null && j.parent() != null) {
                j = j.parent();
            }
            return j;
        }
    }

    enum Status
    {
        Created,
        Suspended,
        Background,
        Foreground,
        Done
    }

    int id();

    CharSequence command();

    Status status();

    void suspend();

    void background();

    void foreground();

    void interrupt();

    Result result();

    List<Process> processes();

    CommandSession session();

    Job parent();

}
