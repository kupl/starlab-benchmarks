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

package org.apache.taglibs.standard.tag.rt.core;

import jakarta.servlet.jsp.JspTagException;

import org.apache.taglibs.standard.tag.common.core.ImportSupport;

/**
 * <p>A handler for &lt;import&gt; that supports rtexprvalue-based
 * attributes.</p>
 *
 * @author Shawn Bayern
 */

public class ImportTag extends ImportSupport {

    //*********************************************************************
    // Accessor methods

    // for tag attribute

    public void setUrl(String url) throws JspTagException {
        this.url = url;
    }

    // for tag attribute

    public void setContext(String context) throws JspTagException {
        this.context = context;
    }

    // for tag attribute

    public void setCharEncoding(String charEncoding) throws JspTagException {
        this.charEncoding = charEncoding;
    }

}
