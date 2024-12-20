package org.apache.maven.shared.utils.xml;

/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

import java.io.IOException;
import java.io.PrintWriter;
import java.io.Writer;

/**
 * @author Brett Porter
 */
public class Xpp3DomWriter
{
    /**
     * @param writer {@link Writer}
     * @param dom {@link Xpp3Dom}
     * @throws IOException if writing fails.
     */
    public static void write( Writer writer, Xpp3Dom dom ) throws IOException
    {
        write( new PrettyPrintXMLWriter( writer ), dom );
    }

    /**
     * @param writer {@link PrintWriter}
     * @param dom {@link Xpp3Dom}
     * @throws IOException if writing fails.
     */
    public static void write( PrintWriter writer, Xpp3Dom dom ) throws IOException
    {
        write( new PrettyPrintXMLWriter( writer ), dom );
    }

    /**
     * @param xmlWriter {@link XMLWriter}
     * @param dom {@link Xpp3Dom}
     * @throws IOException if writing fails.
     */
    public static void write( XMLWriter xmlWriter, Xpp3Dom dom ) throws IOException
    {
        write( xmlWriter, dom, true );
    }

    /**
     * @param xmlWriter {@link XMLWriter}
     * @param dom {@link Xpp3Dom}
     * @param escape true/false.
     * @throws IOException if writing fails.
     */
/**
 *
 *
 * @param xmlWriter
 * 		{@link XMLWriter}
 * @param dom
 * 		{@link Xpp3Dom}
 * @param escape
 * 		true/false.
 * @throws IOException
 * 		if writing fails.
 */
public static void write(org.apache.maven.shared.utils.xml.XMLWriter xmlWriter, org.apache.maven.shared.utils.xml.Xpp3Dom dom, boolean escape) throws java.io.IOException {
    xmlWriter.startElement(dom.getName());
    java.lang.String[] attributeNames = dom.getAttributeNames();
    for (java.lang.String attributeName : attributeNames) {
        xmlWriter.addAttribute(attributeName, dom.getAttribute(attributeName));
    }
    org.apache.maven.shared.utils.xml.Xpp3Dom[] children = dom.getChildren();
    for (org.apache.maven.shared.utils.xml.Xpp3Dom aChildren : children) {
        org.apache.maven.shared.utils.xml.Xpp3DomWriter.write(xmlWriter, aChildren, escape);
    }
    java.lang.String value = dom.getValue();
    {
        if (escape) {
            xmlWriter.writeText(/* NPEX_NULL_EXP */
            value);
        } else {
            xmlWriter.writeMarkup(value);
        }
    }
    xmlWriter.endElement();
}

}
