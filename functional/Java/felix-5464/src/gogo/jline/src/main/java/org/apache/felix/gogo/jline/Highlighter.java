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
package org.apache.felix.gogo.jline;

import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.felix.gogo.runtime.CommandSessionImpl;
import org.apache.felix.gogo.runtime.EOFError;
import org.apache.felix.gogo.runtime.Parser.Program;
import org.apache.felix.gogo.runtime.Parser.Statement;
import org.apache.felix.gogo.runtime.SyntaxError;
import org.apache.felix.gogo.runtime.Token;
import org.apache.felix.service.command.CommandSession;
import org.apache.felix.service.command.Function;
import org.jline.reader.LineReader;
import org.jline.reader.LineReader.RegionType;
import org.jline.reader.impl.DefaultHighlighter;
import org.jline.utils.AttributedString;
import org.jline.utils.AttributedStringBuilder;
import org.jline.utils.AttributedStyle;
import org.jline.utils.WCWidth;

public class Highlighter extends DefaultHighlighter {

    public static final String DEFAULT_HIGHLIGHTER_COLORS = "rs=35:st=32:nu=32:co=32:va=36:vn=36:fu=94:bf=91:re=90";

    private final CommandSession session;

    public Highlighter(CommandSession session) {
        this.session = session;
    }

    public AttributedString highlight(LineReader reader, String buffer) {
        try {
            Program program = null;
            List<Token> tokens = null;
            List<Statement> statements = null;
            String repaired = buffer;
            while (program == null) {
                try {
                    org.apache.felix.gogo.runtime.Parser parser = new org.apache.felix.gogo.runtime.Parser(repaired);
                    program = parser.program();
                    tokens = parser.tokens();
                    statements = parser.statements();
                } catch (EOFError e) {
                    repaired = repaired + " " + e.repair();
                    // Make sure we don't loop forever
                    if (repaired.length() > buffer.length() + 1024) {
                        return new AttributedStringBuilder().append(buffer).toAttributedString();
                    }
                }
            }

            Map<String, String> colors = Posix.getColorMap(session, "HIGHLIGHTER", DEFAULT_HIGHLIGHTER_COLORS);

            int underlineStart = -1;
            int underlineEnd = -1;
            int negativeStart = -1;
            int negativeEnd = -1;
            String search = reader.getSearchTerm();
            if (search != null && search.length() > 0) {
                underlineStart = buffer.indexOf(search);
                if (underlineStart >= 0) {
                    underlineEnd = underlineStart + search.length() - 1;
                }
            }
            if (reader.getRegionActive() != RegionType.NONE) {
                negativeStart = reader.getRegionMark();
                negativeEnd = reader.getBuffer().cursor();
                if (negativeStart > negativeEnd) {
                    int x = negativeEnd;
                    negativeEnd = negativeStart;
                    negativeStart = x;
                }
                if (reader.getRegionActive() == RegionType.LINE) {
                    while (negativeStart > 0 && reader.getBuffer().atChar(negativeStart - 1) != '\n') {
                        negativeStart--;
                    }
                    while (negativeEnd < reader.getBuffer().length() - 1 && reader.getBuffer().atChar(negativeEnd + 1) != '\n') {
                        negativeEnd++;
                    }
                }
            }

            Type[] types = new Type[repaired.length()];

            Arrays.fill(types, Type.Unknown);

            int cur = 0;
            for (Token token : tokens) {
                // We're on the repair side, so exit now
                if (token.start() >= buffer.length()) {
                    break;
                }
                if (token.start() > cur) {
                    cur = token.start();
                }
                // Find corresponding statement
                Statement statement = null;
                for (int i = statements.size() - 1; i >= 0; i--) {
                    Statement s = statements.get(i);
                    if (s.start() <= cur && cur < s.start() + s.length()) {
                        statement = s;
                        break;
                    }
                }

                // Reserved tokens
                Type type = Type.Unknown;
                if (Token.eq(token, "{")
                        || Token.eq(token, "}")
                        || Token.eq(token, "(")
                        || Token.eq(token, ")")
                        || Token.eq(token, "[")
                        || Token.eq(token, "]")
                        || Token.eq(token, "|")
                        || Token.eq(token, ";")
                        || Token.eq(token, "=")) {
                    type = Type.Reserved;
                } else if (token.charAt(0) == '\'' || token.charAt(0) == '"') {
                    type = Type.String;
                } else if (token.toString().matches("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$")) {
                    type = Type.Number;
                } else if (token.charAt(0) == '$') {
                    type = Type.Variable;
                } else if (((Set) session.get(CommandSessionImpl.CONSTANTS)).contains(token.toString())
                        || Token.eq(token, "null") || Token.eq(token, "false") || Token.eq(token, "true")) {
                    type = Type.Constant;
                } else {
                    boolean isFirst = statement != null && statement.tokens().size() > 0
                            && token == statement.tokens().get(0);
                    boolean isThirdWithNext = statement != null && statement.tokens().size() > 3
                            && token == statement.tokens().get(2);
                    boolean isAssign = statement != null && statement.tokens().size() > 1
                            && Token.eq(statement.tokens().get(1), "=");
                    if (isFirst && isAssign) {
                        type = Type.VariableName;
                    }
                    if (isFirst && !isAssign || isAssign && isThirdWithNext) {
                        Object v = session.get(Shell.resolve(session, token.toString()));
                        type = (v instanceof Function) ? Type.Function : Type.BadFunction;
                    }
                }
                Arrays.fill(types, token.start(), Math.min(token.start() + token.length(), types.length), type);
                cur = Math.min(token.start() + token.length(), buffer.length());
            }

            if (buffer.length() < repaired.length()) {
                Arrays.fill(types, buffer.length(), repaired.length(), Type.Repair);
            }

            AttributedStringBuilder sb = new AttributedStringBuilder();
            for (int i = 0; i < repaired.length(); i++) {
                sb.style(AttributedStyle.DEFAULT);
                applyStyle(sb, colors, types[i]);
                if (i >= underlineStart && i <= underlineEnd) {
                    sb.style(sb.style().underline());
                }
                if (i >= negativeStart && i <= negativeEnd) {
                    sb.style(sb.style().inverse());
                }
                char c = repaired.charAt(i);
                if (c == '\t' || c == '\n') {
                    sb.append(c);
                } else if (c < 32) {
                    sb.style(sb.style().inverseNeg())
                            .append('^')
                            .append((char) (c + '@'))
                            .style(sb.style().inverseNeg());
                } else {
                    int w = WCWidth.wcwidth(c);
                    if (w > 0) {
                        sb.append(c);
                    }
                }
            }

            return sb.toAttributedString();
        } catch (SyntaxError e) {
            return super.highlight(reader, buffer);
        }
    }

    private void applyStyle(AttributedStringBuilder sb, Map<String, String> colors, Type type) {
        String col = colors.get(type.color);
        if (col != null && !col.isEmpty()) {
            sb.appendAnsi("\033[" + col + "m");
        }
    }

    enum Type {
        Reserved("rs"),
        String("st"),
        Number("nu"),
        Variable("va"),
        VariableName("vn"),
        Function("fu"),
        BadFunction("bf"),
        Constant("co"),
        Unknown("un"),
        Repair("re");

        private final String color;

        Type(String color) {
            this.color = color;
        }
    }

}
