/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.skywalking.plugin.test.agent.tool.validator.assertor;

import org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.ElementAssertor;
import org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.EqualsAssertor;
import org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.GreatThanAssertor;
import org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.GreetEqualAssertor;
import org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.NoopAssertor;
import org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.NotEqualsAssertor;
import org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.NotNullAssertor;
import org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.NullAssertor;

public class ExpressParser {
public static org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.ElementAssertor parse(java.lang.String express) {
    {
        java.lang.String expressTrim = /* NPEX_NULL_EXP */
        express.trim();
        if (expressTrim.equals("not null")) {
            return new org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.NotNullAssertor();
        }
        if (expressTrim.equals("null")) {
            return new org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.NullAssertor();
        }
        java.lang.String[] expressSegment = expressTrim.split(" ");
        if (expressSegment.length == 1) {
            return new org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.EqualsAssertor(expressSegment[0]);
        } else if (expressSegment.length == 2) {
            java.lang.String exceptedValue = expressSegment[1];
            switch (expressSegment[0].trim()) {
                case "nq" :
                    return new org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.NotEqualsAssertor(exceptedValue);
                case "eq" :
                    return new org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.EqualsAssertor(exceptedValue);
                case "gt" :
                    return new org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.GreatThanAssertor(exceptedValue);
                case "ge" :
                    return new org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.GreetEqualAssertor(exceptedValue);
            }
        }
        return new org.apache.skywalking.plugin.test.agent.tool.validator.assertor.element.EqualsAssertor(express);
    }
}
}
