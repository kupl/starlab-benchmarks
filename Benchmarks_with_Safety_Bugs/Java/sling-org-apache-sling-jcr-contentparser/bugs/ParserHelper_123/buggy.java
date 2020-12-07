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
package org.apache.sling.jcr.contentparser.impl;

import org.apache.commons.lang3.StringUtils;
import org.apache.jackrabbit.util.ISO8601;
import org.apache.sling.jcr.contentparser.ParseException;
import org.apache.sling.jcr.contentparser.ParserOptions;

import java.lang.reflect.Array;
import java.time.Instant;
import java.time.OffsetDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.TimeZone;
import javax.json.JsonObject;

/**
 * Helper parsing logic based on parser options.
 */
class ParserHelper {

    static final String JCR_PRIMARYTYPE = "jcr:primaryType";

    static final String ECMA_DATE_FORMAT = "EEE MMM dd yyyy HH:mm:ss 'GMT'Z";
    static final Locale DATE_FORMAT_LOCALE = Locale.US;

    private final ParserOptions options;
    private final DateTimeFormatter calendarFormat;

    public ParserHelper(ParserOptions options) {
        this.options = options;
        if (options.isDetectCalendarValues()) {
            this.calendarFormat = DateTimeFormatter.ofPattern(ECMA_DATE_FORMAT, DATE_FORMAT_LOCALE);
        }
        else {
            this.calendarFormat = null;
        }
    }

    public void ensureDefaultPrimaryType(Map<String, Object> map) {
        String defaultPrimaryType = options.getDefaultPrimaryType();
        if (defaultPrimaryType != null) {
            if (!map.containsKey(JCR_PRIMARYTYPE)) {
                map.put(JCR_PRIMARYTYPE, defaultPrimaryType);
            }
        }
    }

    public Calendar tryParseCalendar(String value) {
        if (options.isDetectCalendarValues() && !StringUtils.isBlank(value)) {
            // 1st try: parse with ISO-8601 format first
            Calendar calendar = ISO8601.parse(value);
            if (calendar != null) {
                return calendar;
            }
            // 2nd try: parse with ECMA date format which is used by Sling GET servlet
            calendar = Calendar.getInstance();
            try {
                final OffsetDateTime offsetDateTime = OffsetDateTime.parse(value, calendarFormat);
                final Instant instant = offsetDateTime.toInstant();
                calendar.setTime(Date.from(instant));
                calendar.setTimeZone(TimeZone.getTimeZone(offsetDateTime.getOffset()));
                return calendar;
            } catch (DateTimeParseException ex) {
                // ignore
            }
        }
        return null;
    }
    
    public boolean ignoreProperty(String propertyName) {
        return ignoreNames(options.getIgnorePropertyNames(), propertyName);
    }
    
    public boolean ignoreResource(String resourceName) {
        return ignoreNames(options.getIgnoreResourceNames(), resourceName);
    }
    
    private boolean ignoreNames(Set<String> names, String name) {
        return names != null && names.contains(name);
    }

    public String cleanupPropertyName(String propertyName) {
        Set<String> prefixes = options.getRemovePropertyNamePrefixes();
        if (prefixes != null) {
            for (String prefix : prefixes) {
                if (StringUtils.startsWith(propertyName, prefix)) {
                    return StringUtils.substringAfter(propertyName, prefix);
                  }
            }
        }
        return propertyName;
    }
    
public java.lang.Object convertSingleTypeArray(java.lang.Object[] values) {
    if (values.length == 0) {
        return values;
    }
    java.lang.Class<?> itemType = null;
    for (java.lang.Object value : values) {
        {
            if ((/* NPEX_NULL_EXP */
            value instanceof java.util.Map) || (value instanceof javax.json.JsonObject)) {
                throw new org.apache.sling.jcr.contentparser.ParseException("Multivalue array must not contain maps/objects.");
            }
            if (itemType == null) {
                itemType = value.getClass();
            } else if (itemType != value.getClass()) {
                throw new org.apache.sling.jcr.contentparser.ParseException((((("Multivalue array must not contain values with different types " + "(") + itemType.getName()) + ", ") + value.getClass().getName()) + ").");
            }
        }
    }
    java.lang.Object convertedArray = java.lang.reflect.Array.newInstance(itemType, values.length);
    for (int i = 0; i < values.length; i++) {
        java.lang.reflect.Array.set(convertedArray, i, values[i]);
    }
    return convertedArray;
}
    
    public String concatenatePath(String parentPath, String name) {
        if (StringUtils.endsWith(parentPath, "/")) {
            return parentPath + name;
        }
        else {
            return parentPath + "/" + name;
        }
    }
    
}
