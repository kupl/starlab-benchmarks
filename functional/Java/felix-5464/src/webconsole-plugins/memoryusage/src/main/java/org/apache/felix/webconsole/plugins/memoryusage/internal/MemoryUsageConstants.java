/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
package org.apache.felix.webconsole.plugins.memoryusage.internal;

/**
 * The <code>MemoryUsageConstants</code> provides some basic constants for
 * the MemoryUsage support bundle
 */
final class MemoryUsageConstants
{

    /**
     * Service PID for the Configuration Admin configuration object to be
     * provided to configure the memory usage support.
     */
    static final String PID = "org.apache.felix.webconsole.plugins.memoryusage.internal.MemoryUsageConfigurator";

    /**
     * The label (or address) under which the Memory Usage Web Console Plugin
     * is accessible.
     */
    static final String LABEL = "memoryusage";

    /**
     * The category under which the Memory Usage Web Console Plugin
     * is listed.
     */
    static final String CATEGORY = "Web Console";

    /**
     * The name of the property providing the filesystem location where the
     * memory dumps should be placed. If this location is relative it is located
     * inside the bundle private data area.
     * <p>
     * This property may be set as a framework property or as a property of
     * configuration provided by the Configuration Admin Service for the service
     * {@link #PID}.
     */
    static final String PROP_DUMP_LOCATION = "felix.memoryusage.dump.location";

    /**
     * The name of the property providing threshold as a percentage of the
     * maximum available memory at which an automatic memory dumps should
     * be created.
     * <p>
     * This property may be set as a framework property or as a property of
     * configuration provided by the Configuration Admin Service for the service
     * {@link #PID}.
     * <p>
     * The property must be an integer value or be parseable to an integer
     * value. The value must be zero to disable automatic dump generation or in
     * the range [{@link #MIN_DUMP_THRESHOLD}..{@link #MAX_DUMP_THRESHOLD}].
     */
    static final String PROP_DUMP_THRESHOLD = "felix.memoryusage.dump.threshold";

    /**
     * The minimum interval between two consecutive memory dumps being taken
     * in seconds. This property allows the limitation of the number of memory
     * dumps being taken. The default value for the interval is 6 hours. This
     * means that
     * a memory threshold event is ignored unless the last memory dump has been
     * taken
     * at least 6 hours earlier.
     * <p>
     * This property allows limiting the number of memory dumps in case memory
     * consumption is oscillating around the threshold point.
     * <p>
     * The property must be an integer value or be parseable to an integer
     * value. This should be a positive value or zero to force each memory
     * threshold event to cause a memory dump (discouraged). If the value is
     * negative or not parseable to an int, the default value of
     * {@link #DEFAULT_DUMP_INTERVAL 6 hours} applies.
     */
    static final String PROP_DUMP_INTERVAL = "felix.memoryusage.dump.interval";

    /**
     * The default automatic heap dump threshold if none has been configured
     * (or no configuration has yet been provided).
     */
    static final int DEFAULT_DUMP_THRESHOLD = 0;

    /**
     * The maximum allowed automatic heap dump threshold.
     */
    static final int MAX_DUMP_THRESHOLD = 99;

    /**
     * The minimum allowed automatic heap dump threshold.
     */
    static final int MIN_DUMP_THRESHOLD = 50;

    /**
     * The default interval betwen consecutive dumps (6 hours).
     */
    static final long DEFAULT_DUMP_INTERVAL = 6 * 3600 * 1000;

    /**
     * Returns <code>true</code> if the given <code>threshold</code>value is
     * valid; that is if the vaue is either zero or in the range [
     * {@link #MIN_DUMP_THRESHOLD}..{@link #MAX_DUMP_THRESHOLD}].
     *
     * @param threshold The threshold value (percentage) to validate
     * @return <code>true</code> if the value is valid
     */
    static boolean isThresholdValid(final int threshold)
    {
        return threshold == 0 || (threshold >= MIN_DUMP_THRESHOLD && threshold <= MAX_DUMP_THRESHOLD);
    }
}
