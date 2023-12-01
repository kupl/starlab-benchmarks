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

package org.apache.taglibs.standard.tag.common.core;

import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.JspTagException;
import jakarta.servlet.jsp.jstl.core.ConditionalTagSupport;
import jakarta.servlet.jsp.tagext.Tag;

import org.apache.taglibs.standard.resources.Resources;

/**
 * <p>WhenTagSupport is an abstract class that facilitates
 * implementation of &lt;when&gt;-style tags in both the rtexprvalue
 * and expression-evaluating libraries.  It also supports
 * &lt;otherwise&gt;.</p>
 * <p>In particular, this base class does the following:</p>
 * <ul>
 * <li> overrides ConditionalTagSupport.doStartTag() to implement the
 * appropriate semantics of subtags of &lt;choose&gt; </li>
 * </ul>
 *
 * @author Shawn Bayern
 */
public abstract class WhenTagSupport extends ConditionalTagSupport {
    //*********************************************************************
    // Implementation of exclusive-conditional behavior

    /*
     * Includes its body if condition() evalutes to true AND its parent
     * ChooseTag wants it to do so.  The condition will not even be
     * evaluated if ChooseTag instructs us not to run.
     */

    @Override
    public int doStartTag() throws JspException {

        Tag parent;

        // make sure we're contained properly
        if (!((parent = getParent()) instanceof ChooseTag)) {
            throw new JspTagException(
                    Resources.getMessage("WHEN_OUTSIDE_CHOOSE"));
        }

        // make sure our parent wants us to continue
        if (!((ChooseTag) parent).gainPermission()) {
            return SKIP_BODY;
        }                   // we've been reeled in

        // handle conditional behavior
        if (condition()) {
            ((ChooseTag) parent).subtagSucceeded();
            return EVAL_BODY_INCLUDE;
        } else {
            return SKIP_BODY;
        }
    }
}
