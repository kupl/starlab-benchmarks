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
package org.apache.felix.configurator.impl.logger;

import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceReference;
import org.osgi.service.log.LogService;
import org.osgi.util.tracker.ServiceTracker;

public final class LogServiceLogger extends AbstractLogger {

    private final LogService defaultLogger;
    private final ServiceTracker<LogService, LogService> tracker;

    public LogServiceLogger(final BundleContext context) {
        this.defaultLogger = new ConsoleLogger();
        this.tracker = new ServiceTracker<LogService, LogService>(context, LogService.class, null);
        this.tracker.open();
    }

    public void close() {
        this.tracker.close();
    }

    @Override
    public void log(final @SuppressWarnings("rawtypes") ServiceReference ref,
            final int level,
            final String message,
            final Throwable cause) {
        LogService log = this.tracker.getService();
        if (log != null) {
            log.log(ref, level, message, cause);
        } else {
            this.defaultLogger.log(ref, level, message, cause);
        }
    }
}
