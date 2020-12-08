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
package org.apache.felix.http.whiteboard.internal.tracker;

import org.apache.felix.http.base.internal.logger.SystemLogger;
import org.apache.felix.http.whiteboard.internal.manager.ExtenderManager;
import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceReference;
import org.osgi.service.http.HttpContext;

public final class HttpContextTracker
    extends AbstractTracker<HttpContext>
{
    private final ExtenderManager manager;

    public HttpContextTracker(BundleContext context, ExtenderManager manager)
    {
        super(context, HttpContext.class);
        this.manager = manager;
    }

    @Override
    protected void added(HttpContext service, ServiceReference ref)
    {
        SystemLogger.warning("Deprecation warning: " +
                "HttpContext registered through Apache Felix whiteboard service: " + ref +
                ". Please change your code to the OSGi Http Whiteboard Service.", null);

        this.manager.add(service, ref);
    }

    @Override
    protected void modified(HttpContext service, ServiceReference ref)
    {
        removed(service, ref);
        added(service, ref);
    }

    @Override
    protected void removed(HttpContext service, ServiceReference ref)
    {
        this.manager.remove(service);
    }
}
