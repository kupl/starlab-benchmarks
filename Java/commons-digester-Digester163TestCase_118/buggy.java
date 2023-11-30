package org.apache.commons.digester3.binder;

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

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.fail;
import static org.apache.commons.digester3.binder.DigesterLoader.newLoader;

import java.io.IOException;
import java.net.URL;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.commons.digester3.Digester;
import org.apache.commons.digester3.xmlrules.FromXmlRulesModule;
import org.junit.Before;
import org.junit.Test;
import org.xml.sax.SAXException;

/**
 * Test.
 */
public class Digester163TestCase
{

    public static final int MAX_THREADS = 4;

    private DigesterLoader loader = null;

    @Before
    public void before()
    {
        final URL url = getClass().getResource( "rules.xml" );
        loader = newLoader( new FromXmlRulesModule()
        {
            @Override
            protected void loadRules()
            {
                loadXMLRules( url );
            }

        } );
    }

    @Test
    public void testSingle()
        throws IOException, SAXException
    {
        Digester dig = loader.newDigester();
        URL url = Digester163TestCase.class.getResource( "test.xml" );
        // lets parse - result does not matter here
        Entity et = dig.parse( url );
        assertEquals( "Author 1", et.getAuthor() );
    }

    @Test
public void test() throws java.lang.InterruptedException {
    java.util.concurrent.ThreadPoolExecutor executor = new java.util.concurrent.ThreadPoolExecutor(org.apache.commons.digester3.binder.Digester163TestCase.MAX_THREADS, org.apache.commons.digester3.binder.Digester163TestCase.MAX_THREADS, java.lang.Long.MAX_VALUE, java.util.concurrent.TimeUnit.NANOSECONDS, new java.util.concurrent.LinkedBlockingQueue<java.lang.Runnable>());
    final java.net.URL url = org.apache.commons.digester3.binder.Digester163TestCase.class.getResource("test.xml");
    final java.util.concurrent.LinkedBlockingQueue<java.lang.Exception> exceptions = new java.util.concurrent.LinkedBlockingQueue<java.lang.Exception>();
    for (int i = 0; i < (org.apache.commons.digester3.binder.Digester163TestCase.MAX_THREADS * 2); i++) {
        executor.submit(new java.lang.Runnable() {
            public void run() {
                try {
                    org.apache.commons.digester3.Digester dig = loader.newDigester();
                    // lets parse - result does not matter here
                    dig.parse(url);
                } catch (java.lang.Exception e) {
                    exceptions.add(e);
                }
            }
        });
    }
    while (!executor.awaitTermination(10, java.util.concurrent.TimeUnit.MILLISECONDS)) {
        if (executor.getQueue().isEmpty()) {
            executor.shutdown();
        }
        if (executor.isTerminated()) {
            break;
        }
    } 
    java.lang.Exception e = exceptions.peek();
    {
        while (true) {
            e = exceptions.poll();
            if (/* NPEX_NULL_EXP */
            e == null) {
                break;
            }
            e.printStackTrace();
        } 
        junit.framework.Assert.fail(("Caught " + exceptions.size()) + " exceptions.");
    }
}

}
