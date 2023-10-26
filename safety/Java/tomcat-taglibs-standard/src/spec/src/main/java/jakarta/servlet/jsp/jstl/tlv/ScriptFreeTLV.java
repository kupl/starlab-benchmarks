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
package jakarta.servlet.jsp.jstl.tlv;

import java.io.IOException;
import java.util.Map;

import jakarta.servlet.jsp.tagext.PageData;
import jakarta.servlet.jsp.tagext.TagLibraryValidator;
import jakarta.servlet.jsp.tagext.ValidationMessage;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

/**
 * <p>A TagLibraryValidator for enforcing restrictions against
 * the use of JSP scripting elements.</p>
 * <p>This TLV supports four initialization parameters, for controlling
 * which of the four types of scripting elements are allowed or prohibited:</p>
 * <ul>
 * <li><b>allowDeclarations</b>: if true, indicates that declaration elements
 * are not prohibited.
 * <li><b>allowScriptlets</b>: if true, indicates that scriptlets are not
 * prohibited
 * <li><b>allowExpressions</b>: if true, indicates that top-level expression
 * elements (i.e., expressions not associated with request-time attribute
 * values) are not prohibited.
 * <li><b>allowRTExpressions</b>: if true, indicates that expression elements
 * associated with request-time attribute values are not prohibited.
 * </ul>
 * <p>The default value for all for initialization parameters is false,
 * indicating all forms of scripting elements are to be prohibited.</p>
 *
 * @author <a href="mailto:mak@taglib.com">Mark A. Kolb</a>
 * @author Shawn Bayern (minor changes)
 */
public class ScriptFreeTLV extends TagLibraryValidator {

    private static final PageParser parser = new PageParser(true);

    private boolean allowDeclarations = false;
    private boolean allowScriptlets = false;
    private boolean allowExpressions = false;
    private boolean allowRTExpressions = false;

    /**
     * Sets the values of the initialization parameters, as supplied in the TLD.
     *
     * @param initParms a mapping from the names of the initialization parameters
     *                  to their values, as specified in the TLD.
     */
    @Override
    public void setInitParameters(Map<java.lang.String, java.lang.Object> initParms) {
        super.setInitParameters(initParms);
        String declarationsParm = (String) initParms.get("allowDeclarations");
        String scriptletsParm = (String) initParms.get("allowScriptlets");
        String expressionsParm = (String) initParms.get("allowExpressions");
        String rtExpressionsParm = (String) initParms.get("allowRTExpressions");

        allowDeclarations = "true".equalsIgnoreCase(declarationsParm);
        allowScriptlets = "true".equalsIgnoreCase(scriptletsParm);
        allowExpressions = "true".equalsIgnoreCase(expressionsParm);
        allowRTExpressions = "true".equalsIgnoreCase(rtExpressionsParm);
    }

    /**
     * Validates a single JSP page.
     *
     * @param prefix the namespace prefix specified by the page for the
     *               custom tag library being validated.
     * @param uri    the URI specified by the page for the TLD of the
     *               custom tag library being validated.
     * @param page   a wrapper around the XML representation of the page
     *               being validated.
     * @return null, if the page is valid; otherwise, a ValidationMessage[]
     *         containing one or more messages indicating why the page is not valid.
     */
    @Override
    public ValidationMessage[] validate(String prefix, String uri, PageData page) {
        try {
            MyContentHandler handler = new MyContentHandler();
            parser.parse(page, handler);
            return handler.reportResults();
        } catch (ParserConfigurationException e) {
            return vmFromString(e.toString());
        } catch (SAXException e) {
            return vmFromString(e.toString());
        } catch (IOException e) {
            return vmFromString(e.toString());
        }
    }

    /**
     * Handler for SAX events.
     * Four counters are provided as instance variables,
     * for counting occurrences of prohibited scripting elements.
     */
    private class MyContentHandler extends DefaultHandler {
        private int declarationCount = 0;
        private int scriptletCount = 0;
        private int expressionCount = 0;
        private int rtExpressionCount = 0;

        /**
         * This event is received whenever a new element is encountered.
         * The qualified name of each such element is compared against
         * the names of any prohibited scripting elements. When found, the
         * corresponding counter is incremented.
         * If expressions representing request-time attribute values are
         * prohibited, it is also necessary to check the values of all
         * attributes specified by the element. (Trying to figure out
         * which attributes actually support request-time attribute values
         * and checking only those is far more trouble than it's worth.)
         */
        @Override
        public void startElement(String namespaceUri,
                                 String localName, String qualifiedName,
                                 Attributes atts) {
            if ((!allowDeclarations)
                    && qualifiedName.equals("jsp:declaration")) {
                ++declarationCount;
            } else if ((!allowScriptlets)
                    && qualifiedName.equals("jsp:scriptlet")) {
                ++scriptletCount;
            } else if ((!allowExpressions)
                    && qualifiedName.equals("jsp:expression")) {
                ++expressionCount;
            }
            if (!allowRTExpressions) {
                countRTExpressions(atts);
            }
        }

        /**
         * Auxiliary method for checking attribute values to see if
         * are specified via request-time attribute values.
         * Expressions representing request-time attribute values are
         * recognized by their "%=" and "%" delimiters. When found, the
         * corresponding counter is incremented.
         */
        private void countRTExpressions(Attributes atts) {
            int stop = atts.getLength();
            for (int i = 0; i < stop; ++i) {
                String attval = atts.getValue(i);
                if (attval.startsWith("%=") && attval.endsWith("%")) {
                    ++rtExpressionCount;
                }
            }
        }

        /**
         * Constructs a String reporting the number(s) of prohibited
         * scripting elements that were detected, if any.
         * Returns null if no violations were found, making the result
         * of this method suitable for the return value of the
         * TagLibraryValidator.validate() method.
         * <p>TODO:  The update from 7/13/2001 merely makes this validator
         * compliant with the new TLV API, but does not fully take advantage
         * of this API.  In the future, we should do so... but because
         * of the possibility that anti-script checking will be incorporated
         * into the base TLV, I've held off for now and just changed this
         * class to use the new API.  -- SB.
         */
        public ValidationMessage[] reportResults() {
            if (declarationCount + scriptletCount + expressionCount
                    + rtExpressionCount > 0) {
                StringBuilder results = new StringBuilder("JSP page contains ");
                boolean first = true;
                if (declarationCount > 0) {
                    results.append(Integer.toString(declarationCount));
                    results.append(" declaration");
                    if (declarationCount > 1) {
                        results.append('s');
                    }
                    first = false;
                }
                if (scriptletCount > 0) {
                    if (!first) {
                        results.append(", ");
                    }
                    results.append(Integer.toString(scriptletCount));
                    results.append(" scriptlet");
                    if (scriptletCount > 1) {
                        results.append('s');
                    }
                    first = false;
                }
                if (expressionCount > 0) {
                    if (!first) {
                        results.append(", ");
                    }
                    results.append(Integer.toString(expressionCount));
                    results.append(" expression");
                    if (expressionCount > 1) {
                        results.append('s');
                    }
                    first = false;
                }
                if (rtExpressionCount > 0) {
                    if (!first) {
                        results.append(", ");
                    }
                    results.append(Integer.toString(rtExpressionCount));
                    results.append(" request-time attribute value");
                    if (rtExpressionCount > 1) {
                        results.append('s');
                    }
                    first = false;
                }
                results.append(".");
                return vmFromString(results.toString());
            } else {
                return null;
            }
        }
    }


    // constructs a ValidationMessage[] from a single String and no ID

    private static ValidationMessage[] vmFromString(String message) {
        return new ValidationMessage[]{
                new ValidationMessage(null, message)
        };
    }

}
