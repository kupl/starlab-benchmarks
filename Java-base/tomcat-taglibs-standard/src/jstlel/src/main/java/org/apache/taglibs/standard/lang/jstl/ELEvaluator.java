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

package org.apache.taglibs.standard.lang.jstl;

import java.io.Reader;
import java.io.StringReader;
import java.text.MessageFormat;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

import jakarta.servlet.jsp.PageContext;

import org.apache.taglibs.standard.lang.jstl.parser.ELParser;
import org.apache.taglibs.standard.lang.jstl.parser.ParseException;
import org.apache.taglibs.standard.lang.jstl.parser.Token;
import org.apache.taglibs.standard.lang.jstl.parser.TokenMgrError;

/**
 * <p>This is the main class for evaluating expression Strings.  An
 * expression String is a String that may contain expressions of the
 * form ${...}.  Multiple expressions may appear in the same
 * expression String.  In such a case, the expression String's value
 * is computed by concatenating the String values of those evaluated
 * expressions and any intervening non-expression text, then
 * converting the resulting String to the expected type using the
 * PropertyEditor mechanism.
 * <p>In the special case where the expression String is a single
 * expression, the value of the expression String is determined by
 * evaluating the expression, without any intervening conversion to a
 * String.
 * <p>The evaluator maintains a cache mapping expression Strings to
 * their parsed results.  For expression Strings containing no
 * expression elements, it maintains a cache mapping
 * ExpectedType/ExpressionString to parsed value, so that static
 * expression Strings won't have to go through a conversion step every
 * time they are used.  All instances of the evaluator share the same
 * cache.  The cache may be bypassed by setting a flag on the
 * evaluator's constructor.
 * <p>The evaluator must be passed a VariableResolver in its
 * constructor.  The VariableResolver is used to resolve variable
 * names encountered in expressions, and can also be used to implement
 * "implicit objects" that are always present in the namespace.
 * Different applications will have different policies for variable
 * lookups and implicit objects - these differences can be
 * encapsulated in the VariableResolver passed to the evaluator's
 * constructor.
 * <p>Most VariableResolvers will need to perform their resolution
 * against some context.  For example, a JSP environment needs a
 * PageContext to resolve variables.  The evaluate() method takes a
 * generic Object context which is eventually passed to the
 * VariableResolver - the VariableResolver is responsible for casting
 * the context to the proper type.
 * <p>Once an evaluator instance has been constructed, it may be used
 * multiple times, and may be used by multiple simultaneous Threads.
 * In other words, an evaluator instance is well-suited for use as a
 * singleton.
 *
 * @author Nathan Abramson - Art Technology Group
 * @author Shawn Bayern
 */
public class ELEvaluator {
    //-------------------------------------
    // Properties
    //-------------------------------------

    //-------------------------------------
    // Member variables
    //-------------------------------------

    /**
     * Name of configuration setting for maximum number of entries in the
     * cached expression string map
     */
    private static final String EXPR_CACHE_PARAM
            = "org.apache.taglibs.standard.lang.jstl.exprCacheSize";
    /**
     * Default maximum  cache size
     */
    private static final int MAX_SIZE = 100;

    /**
     * The mapping from expression String to its parsed form (String,
     * Expression, or ExpressionString)
     * <p>Using LRU Map with a maximum capacity to avoid out of bound map
     * growth.
     * <p>NOTE: use LinkedHashmap if a dependency on J2SE 1.4+ is ok
     */
    private static Map sCachedExpressionStrings = null;

    /**
     * The mapping from ExpectedType to Maps mapping literal String to
     * parsed value *
     */
    private static final Map sCachedExpectedTypes = new HashMap();

    /**
     * The static Logger *
     */
    static Logger sLogger = new Logger(System.out);

    /**
     * The VariableResolver *
     */
    VariableResolver mResolver;

    /**
     * Flag if the cache should be bypassed *
     */
    private volatile boolean mBypassCache;

    /**
     * The PageContext *
     */
    // TODO: Find a better way to override the expression cache size that does not need this field.
    PageContext pageContext;


    //-------------------------------------

    /**
     * Constructor
     *
     * @param pResolver the object that should be used to resolve
     *                  variable names encountered in expressions.  If null, all variable
     *                  references will resolve to null.
     */
    public ELEvaluator(VariableResolver pResolver) {
        mResolver = pResolver;
    }

    //-------------------------------------

    /**
     * Enable cache bypass
     *
     * @param pBypassCache flag indicating cache should be bypassed
     */
    public void setBypassCache(boolean pBypassCache) {
        mBypassCache = pBypassCache;
    }

    //-------------------------------------

    /**
     * Evaluates the given expression String
     *
     * @param pExpressionString the expression String to be evaluated
     * @param pContext          the context passed to the VariableResolver for
     *                          resolving variable names
     * @param pExpectedType     the type to which the evaluated expression
     *                          should be coerced
     * @return the expression String evaluated to the given expected
     *         type
     */
    public Object evaluate(String pExpressionString,
                           Object pContext,
                           Class pExpectedType,
                           Map functions,
                           String defaultPrefix)
            throws ELException {
        return evaluate(pExpressionString,
                pContext,
                pExpectedType,
                functions,
                defaultPrefix,
                sLogger);
    }

    //-------------------------------------

    /**
     * Evaluates the given expression string
     */
    Object evaluate(String pExpressionString,
                    Object pContext,
                    Class pExpectedType,
                    Map functions,
                    String defaultPrefix,
                    Logger pLogger)
            throws ELException {
        // Check for null expression strings
        if (pExpressionString == null) {
            throw new ELException
                    (Constants.NULL_EXPRESSION_STRING);
        }

        // Set the PageContext;
        pageContext = (PageContext) pContext;

        // Get the parsed version of the expression string
        Object parsedValue = parseExpressionString(pExpressionString);

        // Evaluate differently based on the parsed type
        if (parsedValue instanceof String) {
            // Convert the String, and cache the conversion
            String strValue = (String) parsedValue;
            return convertStaticValueToExpectedType(strValue,
                    pExpectedType,
                    pLogger);
        } else if (parsedValue instanceof Expression) {
            // Evaluate the expression and convert
            Object value =
                    ((Expression) parsedValue).evaluate(pContext,
                            mResolver,
                            functions,
                            defaultPrefix,
                            pLogger);
            return convertToExpectedType(value,
                    pExpectedType,
                    pLogger);
        } else if (parsedValue instanceof ExpressionString) {
            // Evaluate the expression/string list and convert
            String strValue =
                    ((ExpressionString) parsedValue).evaluate(pContext,
                            mResolver,
                            functions,
                            defaultPrefix,
                            pLogger);
            return convertToExpectedType(strValue,
                    pExpectedType,
                    pLogger);
        } else {
            // This should never be reached
            return null;
        }
    }

    //-------------------------------------

    /**
     * Gets the parsed form of the given expression string.  If the
     * parsed form is cached (and caching is not bypassed), return the
     * cached form, otherwise parse and cache the value.  Returns either
     * a String, Expression, or ExpressionString.
     */
    public Object parseExpressionString(String pExpressionString)
            throws ELException {
        // See if it's an empty String
        if (pExpressionString.length() == 0) {
            return "";
        }

        if (mBypassCache) {
            return parseExpressionUncached(pExpressionString);
        }

        Map cache = getOrCreateExpressionStringMap(pageContext);

        // See if it's in the cache
        Object ret = cache.get(pExpressionString);
        if (ret != null) {
            return ret;
        }

        ret = parseExpressionUncached(pExpressionString);
        cache.put(pExpressionString, ret);
        return ret;
    }

    /**
     * Parse an expression string bypassing the cache.
     *
     * This allows expressions to be validated at translation time without polluting the cache.
     *
     * @param pExpressionString the text to parse
     * @return the parse result
     * @throws ELException if there was a problem parsing the expression text
     */
    public Object parseExpressionUncached(String pExpressionString) throws ELException {
        try {
            Reader r = new StringReader(pExpressionString);
            ELParser parser = new ELParser(r);
            return parser.ExpressionString();
        } catch (ParseException exc) {
            throw new ELException(formatParseException(pExpressionString, exc));
        } catch (TokenMgrError exc) {
            // Note - this should never be reached, since the parser is
            // constructed to tokenize any input (illegal inputs get
            // parsed to <BADLY_ESCAPED_STRING_LITERAL> or
            // <ILLEGAL_CHARACTER>
            throw new ELException(exc.getMessage());
        }
    }

    //-------------------------------------

    /**
     * Converts the given value to the specified expected type.
     */
    Object convertToExpectedType(Object pValue,
                                 Class pExpectedType,
                                 Logger pLogger)
            throws ELException {
        return Coercions.coerce(pValue,
                pExpectedType,
                pLogger);
    }

    //-------------------------------------

    /**
     * Converts the given String, specified as a static expression
     * string, to the given expected type.  The conversion is cached.
     */
    Object convertStaticValueToExpectedType(String pValue,
                                            Class pExpectedType,
                                            Logger pLogger)
            throws ELException {
        // See if the value is already of the expected type
        if (pExpectedType == String.class ||
                pExpectedType == Object.class) {
            return pValue;
        }

        // Find the cached value
        Map valueByString = getOrCreateExpectedTypeMap(pExpectedType);
        if (!mBypassCache &&
                valueByString.containsKey(pValue)) {
            return valueByString.get(pValue);
        } else {
            // Convert from a String
            Object ret = Coercions.coerce(pValue, pExpectedType, pLogger);
            valueByString.put(pValue, ret);
            return ret;
        }
    }

    //-------------------------------------

    /**
     * Creates or returns the Map that maps string literals to parsed
     * values for the specified expected type.
     */
    static Map getOrCreateExpectedTypeMap(Class pExpectedType) {
        synchronized (sCachedExpectedTypes) {
            Map ret = (Map) sCachedExpectedTypes.get(pExpectedType);
            if (ret == null) {
                ret = Collections.synchronizedMap(new HashMap());
                sCachedExpectedTypes.put(pExpectedType, ret);
            }
            return ret;
        }
    }

    //-------------------------------------

    /**
     * Creates LRU map of expression strings. If context parameter
     * specifying cache size is present use that as the maximum size
     * of the LRU map otherwise use default.
     *
     * TODO: Using the context parameter means the cache is sized based on the configuration
     * of the first web application that calls this. This might be a problem if this jar is
     * installed in the application server's classpath rather than the application's.
     */
    private static synchronized Map getOrCreateExpressionStringMap(PageContext pageContext) {
        if (sCachedExpressionStrings == null) {

            final int maxSize;
            if ((pageContext != null) && (pageContext.getServletContext() != null)) {
                String value = pageContext.getServletContext().getInitParameter(EXPR_CACHE_PARAM);
                if (value != null) {
                    maxSize = Integer.valueOf(value);
                } else {
                    maxSize = MAX_SIZE;
                }
            } else {
                maxSize = MAX_SIZE;
            }

            // fall through if it couldn't find the parameter
            sCachedExpressionStrings = Collections.synchronizedMap(new LinkedHashMap() {
                @Override
                protected boolean removeEldestEntry(Map.Entry eldest) {
                    return size() > maxSize;
                }
            });
        }
        return sCachedExpressionStrings;
    }

    //-------------------------------------
    // Formatting ParseException
    //-------------------------------------

    /**
     * Formats a ParseException into an error message suitable for
     * displaying on a web page
     */
    static String formatParseException(String pExpressionString,
                                       ParseException pExc) {
        // Generate the String of expected tokens
        StringBuilder expectedBuf = new StringBuilder();
        int maxSize = 0;
        boolean printedOne = false;

        if (pExc.expectedTokenSequences == null) {
            return pExc.toString();
        }

        for (int i = 0; i < pExc.expectedTokenSequences.length; i++) {
            if (maxSize < pExc.expectedTokenSequences[i].length) {
                maxSize = pExc.expectedTokenSequences[i].length;
            }
            for (int j = 0; j < pExc.expectedTokenSequences[i].length; j++) {
                if (printedOne) {
                    expectedBuf.append(", ");
                }
                expectedBuf.append
                        (pExc.tokenImage[pExc.expectedTokenSequences[i][j]]);
                printedOne = true;
            }
        }
        String expected = expectedBuf.toString();

        // Generate the String of encountered tokens
        StringBuilder encounteredBuf = new StringBuilder();
        Token tok = pExc.currentToken.next;
        for (int i = 0; i < maxSize; i++) {
            if (i != 0) {
                encounteredBuf.append(" ");
            }
            if (tok.kind == 0) {
                encounteredBuf.append(pExc.tokenImage[0]);
                break;
            }
            encounteredBuf.append(addEscapes(tok.image));
            tok = tok.next;
        }
        String encountered = encounteredBuf.toString();

        // Format the error message
        return MessageFormat.format
                (Constants.PARSE_EXCEPTION,
                        new Object[]{
                                expected,
                                encountered,
                        });
    }

    //-------------------------------------

    /**
     * Used to convert raw characters to their escaped version when
     * these raw version cannot be used as part of an ASCII string
     * literal.
     */
    static String addEscapes(String str) {
        StringBuilder retval = new StringBuilder();
        char ch;
        for (int i = 0; i < str.length(); i++) {
            switch (str.charAt(i)) {
                case 0:
                    continue;
                case '\b':
                    retval.append("\\b");
                    continue;
                case '\t':
                    retval.append("\\t");
                    continue;
                case '\n':
                    retval.append("\\n");
                    continue;
                case '\f':
                    retval.append("\\f");
                    continue;
                case '\r':
                    retval.append("\\r");
                    continue;
                default:
                    if ((ch = str.charAt(i)) < 0x20 || ch > 0x7e) {
                        String s = "0000" + Integer.toString(ch, 16);
                        retval.append("\\u" + s.substring(s.length() - 4, s.length()));
                    } else {
                        retval.append(ch);
                    }
                    continue;
            }
        }
        return retval.toString();
    }

    //-------------------------------------
    // Testing methods
    //-------------------------------------

    /**
     * Parses the given expression string, then converts it back to a
     * String in its canonical form.  This is used to test parsing.
     */
    public String parseAndRender(String pExpressionString)
            throws ELException {
        Object val = parseExpressionString(pExpressionString);
        if (val instanceof String) {
            return (String) val;
        } else if (val instanceof Expression) {
            return "${" + ((Expression) val).getExpressionString() + "}";
        } else if (val instanceof ExpressionString) {
            return ((ExpressionString) val).getExpressionString();
        } else {
            return "";
        }
    }

    //-------------------------------------

}
