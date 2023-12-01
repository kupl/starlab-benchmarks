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

package org.apache.taglibs.standard.examples.taglib;

import java.lang.reflect.Array;

import jakarta.servlet.jsp.JspTagException;

/**
 * <p>Exmaples taglib Functions</p>
 *
 * @author Pierre Delisle
 */

public class Functions {

    /**
     * Display the collection types supported by c:forEach.
     */
    public static String display(Object obj) throws JspTagException {
        if (obj == null) {
            return "";
        }
        if (obj instanceof String) {
            return obj.toString();
        }
        /*
        if (obj instanceof Collection) {
            return "FIXME";
        }
        if (obj instanceof Map) {
            return "FIXME";
        }        
        if (obj instanceof Iterator) {
            Iterator iter = (Iterator)obj;
            while (iter.hasNext()) {
                iter.next();
            }
            return "FIXME";
        }            
        if (obj instanceof Enumeration) {
            Enumeration enum_ = (Enumeration)obj;
            while (enum_.hasMoreElements()) {
                enum_.nextElement();
            }
            return "FIXME";
        }
        */
        try {
            StringBuilder buf = new StringBuilder();
            int count = Array.getLength(obj);
            for (int i = 0; i < count; i++) {
                buf.append(Array.get(obj, i).toString());
                if (i < count - 1) {
                    buf.append("<font color='red'> &#149; </font>");
                }
            }
            return buf.toString();
        } catch (IllegalArgumentException ex) {
        }
        throw new JspTagException("Bad Item");
    }
}
