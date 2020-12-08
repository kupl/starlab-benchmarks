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
package org.apache.felix.bundlerepository.impl;

import java.util.Hashtable;

import org.apache.felix.bundlerepository.RepositoryAdmin;
import org.apache.felix.bundlerepository.impl.wrapper.Wrapper;
import org.apache.felix.utils.log.Logger;
import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;
import org.osgi.service.repository.Repository;
import org.osgi.service.url.URLConstants;
import org.osgi.service.url.URLStreamHandlerService;

public class Activator implements BundleActivator
{
    private static BundleContext context = null;
    private static Logger logger = new Logger(null);
    private transient RepositoryAdminImpl m_repoAdmin = null;


    public static BundleContext getContext()
    {
        return context;
    }

    static void setContext(BundleContext context)
    {
        Activator.context = context;
    }

    public static void log(int level, String message)
    {
        if (logger != null)
        {
            logger.log(level, message);
        }
    }

    public static void log(int level, String message, Throwable exception)
    {
        if (logger != null)
        {
            logger.log(level, message, exception);
        }
    }

    public void start(BundleContext context)
    {
        Activator.context = context;
        Activator.logger = new Logger(context);

        // Register bundle repository service.
        m_repoAdmin = new RepositoryAdminImpl(context, logger);
        context.registerService(
            RepositoryAdmin.class.getName(),
            m_repoAdmin, null);

        // Register the OSGi Repository-spec compliant facade
        context.registerService(
            Repository.class.getName(),
            new OSGiRepositoryImpl(m_repoAdmin), null);

        try
        {
            context.registerService(
                org.osgi.service.obr.RepositoryAdmin.class.getName(),
                Wrapper.wrap(m_repoAdmin), null);
        }
        catch (Throwable th)
        {
            // Ignore
        }

        // We dynamically import the impl service API, so it
        // might not actually be available, so be ready to catch
        // the exception when we try to register the command service.
        try
        {
            // Register "obr" impl command service as a
            // wrapper for the bundle repository service.
            context.registerService(
                org.apache.felix.shell.Command.class.getName(),
                new ObrCommandImpl(Activator.context, m_repoAdmin), null);
        }
        catch (Throwable th)
        {
            // Ignore.
        }

        try
        {
            Hashtable dict = new Hashtable();
            dict.put("osgi.command.scope", "obr");
            dict.put("osgi.command.function", new String[] {
                    "deploy", "info", "javadoc", "list", "repos", "source" });
            context.registerService(ObrGogoCommand.class.getName(),
                    new ObrGogoCommand(Activator.context, m_repoAdmin), dict);
        }
        catch (Throwable th)
        {
            // Ignore
        }

        try
        {
			Hashtable dict = new Hashtable();
			dict.put(URLConstants.URL_HANDLER_PROTOCOL, "obr");
			context.registerService(URLStreamHandlerService.class.getName(),
					new ObrURLStreamHandlerService(Activator.context, m_repoAdmin), dict);
		}
        catch (Exception e)
		{
			throw new RuntimeException("could not register obr url handler");
		}

    }

    public void stop(BundleContext context)
    {
        m_repoAdmin.dispose();
    }
}