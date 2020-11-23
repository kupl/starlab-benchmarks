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

package org.apache.sling.tracer.internal;

import org.osgi.service.metatype.annotations.AttributeDefinition;
import org.osgi.service.metatype.annotations.ObjectClassDefinition;

@ObjectClassDefinition(name = "Apache Sling Log Tracer",
        description =  "Provides support for enabling log for specific loggers on per request basis. " +
                "Refer to http://sling.apache.org/documentation/bundles/log-tracers.html for " +
                "more details")
public @interface Configuration {

    @AttributeDefinition(
            name = "Tracer Sets",
            description = "Default list of tracer sets configured. Tracer Set config confirms " +
                    "to following format. <set name> : <logger name>;level=<level name>, other loggers")
    String[] tracerSets() default {
            "oak-query : org.apache.jackrabbit.oak.query.QueryEngineImpl;level=debug",
            "oak-writes : org.apache.jackrabbit.oak.jcr.operations.writes;level=trace"
    };

    @AttributeDefinition(
            name = "Enabled",
            description = "Enable the Tracer")
    boolean enabled();

    @AttributeDefinition(
            name = "Recording Servlet Enabled",
            description = "Enable the Tracer Servlet. This servlet is required for the tracer recording feature " +
                    "to work and provides access to the json dump of the recording performed")
    boolean servletEnabled();

    @AttributeDefinition(
            name = "Recording Cache Size",
            description = "Recording cache size in MB which would be used to temporary cache the recording data")
    int recordingCacheSizeInMB() default 50;

    @AttributeDefinition(
            name = "Recording Cache Duration",
            description = "Time in seconds upto which the recording data would be held in memory before expiry")
    long recordingCacheDurationInSecs() default 60 * 15;

    @AttributeDefinition(
            name = "Compress Recording",
            description = "Enable compression for recoding held in memory")
    boolean recordingCompressionEnabled() default true;

    @AttributeDefinition(
            name = "GZip Response",
            description = "If enabled the response sent would be compressed")
    boolean gzipResponse() default true;

}
