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
package org.apache.commons.lang3.time;

import java.text.DateFormatSymbols;
import java.text.ParseException;
import java.util.Date;
import java.util.Locale;
import java.util.TimeZone;

import org.junit.Assert;
import org.junit.Test;

public class FastDateParser_TimeZoneStrategyTest {

    @Test
public void testTimeZoneStrategyPattern() {
    for (final java.util.Locale locale : java.util.Locale.getAvailableLocales()) {
        final org.apache.commons.lang3.time.FastDateParser parser = new org.apache.commons.lang3.time.FastDateParser("z", java.util.TimeZone.getDefault(), locale);
        final java.lang.String[][] zones = java.text.DateFormatSymbols.getInstance(locale).getZoneStrings();
        for (final java.lang.String[] zone : zones) {
            for (int t = 1; t < zone.length; ++t) {
                final java.lang.String tzDisplay = zone[t];
                {
                    try {
                        parser.parse(/* NPEX_NULL_EXP */
                        tzDisplay);
                    } catch (java.lang.Exception ex) {
                        org.junit.Assert.fail((((((((((((("'" + tzDisplay) + "'") + " Locale: '") + locale.getDisplayName()) + "'") + " TimeZone: ") + zone[0]) + " offset: ") + t) + " defaultLocale: ") + java.util.Locale.getDefault()) + " defaultTimeZone: ") + java.util.TimeZone.getDefault().getDisplayName());
                    }
                }
            }
        }
    }
}

    @Test
    public void testLang1219() throws ParseException {
        FastDateParser parser = new FastDateParser("dd.MM.yyyy HH:mm:ss z", TimeZone.getDefault(), Locale.GERMAN);

        Date summer = parser.parse("26.10.2014 02:00:00 MESZ");
        Date standard = parser.parse("26.10.2014 02:00:00 MEZ");
        Assert.assertNotEquals(summer.getTime(), standard.getTime());
    }
}
