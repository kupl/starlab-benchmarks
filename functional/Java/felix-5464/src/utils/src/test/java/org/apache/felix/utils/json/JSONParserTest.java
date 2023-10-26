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
package org.apache.felix.utils.json;

import java.io.StringWriter;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class JSONParserTest {
    @Test
    public void testJsonSimple() {
        String json = "{\"hi\": \"ho\", \"ha\": true}";
        JSONParser jp = new JSONParser(json);
        Map<String, Object> m = jp.getParsed();
        assertEquals(2, m.size());
        assertEquals("ho", m.get("hi"));
        assertTrue((Boolean) m.get("ha"));
    }

    @Test
    @SuppressWarnings("unchecked")
    public void testJsonComplex() {
        String json = "{\"a\": [1,2,3,4,5], \"b\": {\"x\": 12, \"y\": 42, \"z\": {\"test test\": \"hello hello\"}}, \"ddd\": 12.34}";
        JSONParser jp = new JSONParser(json);
        Map<String, Object> m = jp.getParsed();
        assertEquals(3, m.size());
        assertEquals(Arrays.asList(1L, 2L, 3L, 4L, 5L), m.get("a"));
        Map<String, Object> mb = (Map<String, Object>) m.get("b");
        assertEquals(3, mb.size());
        assertEquals(12L, mb.get("x"));
        assertEquals(42L, mb.get("y"));
        Map<String, Object> mz = (Map<String, Object>) mb.get("z");
        assertEquals(1, mz.size());
        assertEquals("hello hello", mz.get("test test"));
        assertEquals(12.34d, ((Double) m.get("ddd")).doubleValue(), 0.0001d);
    }

    @Test
    public void testJsonArray() {
        String json = "{\"abc\": [\"x\", \"y\", \"z\"]}";
        JSONParser jp = new JSONParser(json);
        Map<String, Object> m = jp.getParsed();
        assertEquals(1, m.size());
        assertEquals(Arrays.asList("x", "y", "z"), m.get("abc"));
    }

    @Test
    public void testEmptyJsonArray() {
        String json = "{\"abc\": {\"def\": []}}";
        JSONParser jp = new JSONParser(json);
        Map<String, Object> m = jp.getParsed();
        assertEquals(1, m.size());
        Map<String, Object> result = new HashMap<String, Object>();
        result.put("def", Collections.emptyList());
        assertEquals(result, m.get("abc"));
    }

    @Test
    public void testEscapeChar() throws Exception{
        StringWriter sw = new StringWriter();
        JSONWriter js = new JSONWriter(sw);
        js.object().key("foo").value("/bar").endObject().flush();

        JSONParser jp = new JSONParser(sw.toString());
        assertEquals("/bar", jp.getParsed().get("foo"));
    }

    @Test
    public void testEscapeChar2() throws Exception{
        String s = "{\"tab\":\"\\t\","
                + "\"quotes\": \"\\'\\\"\\\'\","
                + "\"slashes\": \"\\/\\\\\\/\","
                + "\"back\\bspace\": \"form\\ffeed\","
                + "\"\\n\\n\\n\": \"\\r\\r\\r\","
                + "\"!\\u2708!\\u2708!\": \"\\u2202\"}";
        JSONParser jp = new JSONParser(s);
        Map<String, Object> parsed = jp.getParsed();
        assertEquals("\t", parsed.get("tab"));
        assertEquals("\'\"\'", parsed.get("quotes"));
        assertEquals("/\\/", parsed.get("slashes"));
        assertEquals("form\ffeed", parsed.get("back\bspace"));
        assertEquals("\r\r\r", parsed.get("\n\n\n"));
        assertEquals("\u2202", parsed.get("!\u2708!\u2708!"));
    }

    @Test
    public void testTopLevelArraySimple() throws Exception {
        String s = "[\"a one\", 5, true]";

        JSONParser jp = new JSONParser(s);
        List<Object> res = jp.getParsedList();
        assertEquals(3, res.size());
        assertEquals("a one", res.get(0));
        assertEquals(5L, res.get(1));
        assertEquals(true, res.get(2));
    }

    @SuppressWarnings("rawtypes")
    @Test
    public void testTopLevelArrayComplex() throws Exception{
        String s = "[{\"key1\": 123, \"key2\": 321}, [[true, false], \"hi \"]]";

        JSONParser jp = new JSONParser(s);
        List<Object> res = jp.getParsedList();
        assertEquals(2, res.size());

        Map m = (Map) res.get(0);
        assertEquals(2, m.size());
        assertEquals(123L, m.get("key1"));
        assertEquals(321L, m.get("key2"));

        List l = (List) res.get(1);
        assertEquals(2, l.size());

        List l2 = (List) l.get(0);
        assertEquals(2, l2.size());
        assertTrue((Boolean) l2.get(0));
        assertFalse((Boolean) l2.get(1));

        assertEquals("hi ", l.get(1));
    }
}
