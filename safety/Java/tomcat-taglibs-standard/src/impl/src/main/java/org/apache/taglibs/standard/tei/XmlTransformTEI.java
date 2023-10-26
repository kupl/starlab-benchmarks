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

package org.apache.taglibs.standard.tei;

import jakarta.servlet.jsp.tagext.TagData;
import jakarta.servlet.jsp.tagext.TagExtraInfo;

/**
 * <p>An implementation of TagExtraInfo that implements validation for
 * &lt;x:transform&gt;'s attributes</p>
 *
 * @author Shawn Bayern
 */
public class XmlTransformTEI extends TagExtraInfo {

    final private static String XSLT = "xslt";
    final private static String RESULT = "result";
    final private static String VAR = "var";

    @Override
    public boolean isValid(TagData us) {
        // require XSLT
        if (!Util.isSpecified(us, XSLT)) {
            return false;
        }

        // disallow both VAR and RESULT
        if (Util.isSpecified(us, VAR) && Util.isSpecified(us, RESULT)) {
            return false;
        }
        return true;
    }

}
