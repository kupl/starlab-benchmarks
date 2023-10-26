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
package org.apache.felix.scr.impl.manager;

import java.lang.management.ManagementFactory;
import java.lang.management.ThreadInfo;
import java.lang.management.ThreadMXBean;
import java.util.concurrent.Callable;

import org.codehaus.mojo.animal_sniffer.IgnoreJRERequirement;

public class ThreadDump implements Callable<String>
{

    @IgnoreJRERequirement
    public String call() throws Exception
    {
        ThreadMXBean threadMXBean = ManagementFactory.getThreadMXBean();
        StringBuilder b = new StringBuilder( "Thread dump\n" );
        ThreadInfo[] infos = threadMXBean.dumpAllThreads( threadMXBean.isObjectMonitorUsageSupported(), threadMXBean.isSynchronizerUsageSupported() );
        for ( int i = 0; i < infos.length; i++ )
        {
            ThreadInfo ti = infos[i];
            b.append( "\n\nThreadId: " ).append( ti.getThreadId() ).append( " : name: " ).append( ti.getThreadName() ).append( " State: " ).append( ti.getThreadState() );
            b.append( "\n  LockInfo: " ).append( ti.getLockInfo() ).append( " LockOwnerId: " ).append( ti.getLockOwnerId() ).append( " LockOwnerName: ").append( ti.getLockOwnerName() );
            StackTraceElement[] stackTrace = ti.getStackTrace();
            for (int j = 0; j < stackTrace.length; j++ )
            {
                b.append( "\n  " ).append( stackTrace[j] );
            }
        }
        return b.toString();
    }

}
