/*
 * $Id$
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.apache.tiles.template;

import java.io.IOException;
import java.util.Deque;

import org.apache.tiles.Attribute;
import org.apache.tiles.TilesContainer;
import org.apache.tiles.access.TilesAccess;
import org.apache.tiles.autotag.core.runtime.ModelBody;
import org.apache.tiles.request.Request;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * <p>
 * <strong>Inserts the value of an attribute into the page.</strong>
 * </p>
 * <p>
 * This tag can be flexibly used to insert the value of an attribute into a
 * page. As in other usages in Tiles, every attribute can be determined to have
 * a "type", either set explicitly when it was defined, or "computed". If the
 * type is not explicit, then if the attribute value is a valid definition, it
 * will be inserted as such. Otherwise, if it begins with a "/" character, it
 * will be treated as a "template". Finally, if it has not otherwise been
 * assigned a type, it will be treated as a String and included without any
 * special handling.
 * </p>
 *
 * <p>
 * <strong>Example : </strong>
 * </p>
 *
 * <pre>
 * &lt;code&gt;
 *           &lt;tiles:insertAttribute name=&quot;body&quot; /&gt;
 *         &lt;/code&gt;
 * </pre>
 *
 * @version $Rev$ $Date$
 * @since 2.2.0
 */
public class InsertAttributeModel {

    /**
     * The logging object.
     */
    private Logger log = LoggerFactory.getLogger(getClass());

    /**
     * The attribute resolver to use.
     */
    private AttributeResolver attributeResolver;

    /**
     * Constructor that uses the defaut attribute resolver.
     *
     * @since 3.0.0
     */
    public InsertAttributeModel() {
        this(new DefaultAttributeResolver());
    }

    /**
     * Constructor.
     *
     * @param attributeResolver The attribute resolver to use.
     * @since 2.2.0
     */
    public InsertAttributeModel(AttributeResolver attributeResolver) {
        this.attributeResolver = attributeResolver;
    }

    /**
     * Executes the operation.
     * @param ignore If <code>true</code>, if an exception happens during
     * rendering, of if the attribute is null, the problem will be ignored.
     * @param preparer The preparer to invoke before rendering the attribute.
     * @param role A comma-separated list of roles. If present, the attribute
     * will be rendered only if the current user belongs to one of the roles.
     * @param defaultValue The default value of the attribute. To use only if
     * the attribute was not computed.
     * @param defaultValueRole The default comma-separated list of roles. To use
     * only if the attribute was not computed.
     * @param defaultValueType The default type of the attribute. To use only if
     * the attribute was not computed.
     * @param name The name of the attribute.
     * @param value The attribute to use immediately, if not null.
     * @param flush If <code>true</code>, the response will be flushed after the insert.
     * @param request The request.
     * @param modelBody The body.
     * @throws IOException If an I/O error happens during rendering.
     * @since 2.2.0
     */
    public void execute(boolean ignore, String preparer,
            String role, Object defaultValue, String defaultValueRole,
            String defaultValueType, String name, Attribute value,
            boolean flush, Request request, ModelBody modelBody) throws IOException {
        TilesContainer container = TilesAccess.getCurrentContainer(request);
        Deque<Object> composeStack = ComposeStackUtil.getComposeStack(request);
        Attribute attribute = resolveAttribute(container, ignore, preparer,
                role, defaultValue, defaultValueRole, defaultValueType, name,
                value, request);
        if (attribute != null) {
            composeStack.push(attribute);
        }
        modelBody.evaluateWithoutWriting();
        container = TilesAccess.getCurrentContainer(request);
        if (attribute != null) {
            attribute = (Attribute) composeStack.pop();
        }
        renderAttribute(container, ignore, attribute, request);
        if (flush) {
            request.getWriter().flush();
        }
    }

    /**
     * Resolves the attribute. and starts the context.
     *
     * @param container The Tiles container to use.
     * @param ignore If <code>true</code>, if an exception happens during
     * rendering, of if the attribute is null, the problem will be ignored.
     * @param preparer The preparer to invoke before rendering the attribute.
     * @param role A comma-separated list of roles. If present, the attribute
     * will be rendered only if the current user belongs to one of the roles.
     * @param defaultValue The default value of the attribute. To use only if
     * the attribute was not computed.
     * @param defaultValueRole The default comma-separated list of roles. To use
     * only if the attribute was not computed.
     * @param defaultValueType The default type of the attribute. To use only if
     * the attribute was not computed.
     * @param name The name of the attribute.
     * @param value The attribute to use immediately, if not null.
     * @param request The request.
     * @return The resolved attribute.
     */
    private Attribute resolveAttribute(TilesContainer container,
            boolean ignore, String preparer, String role, Object defaultValue,
            String defaultValueRole, String defaultValueType, String name,
            Attribute value, Request request) {
        if (preparer != null) {
            container.prepare(preparer, request);
        }
        Attribute attribute = attributeResolver.computeAttribute(container,
                value, name, role, ignore, defaultValue, defaultValueRole,
                defaultValueType, request);
        container.startContext(request);
        return attribute;
    }

    /**
     * Renders the attribute as a string.
     * @param container The Tiles container to use.
     * @param ignore If <code>true</code>, if an exception happens during
     * rendering, of if the attribute is null, the problem will be ignored.
     * @param attribute The attribute to use, previously resolved.
     * @param request The request.
     *
     * @throws IOException If an I/O error happens during rendering.
     */
    private void renderAttribute(TilesContainer container, boolean ignore,
            Attribute attribute, Request request) throws IOException {
        try {
            if (attribute == null && ignore) {
                return;
            }
            container.render(attribute, request);
        } catch (IOException e) {
            if (!ignore) {
                throw e;
            } else if (log.isDebugEnabled()) {
                log.debug("Ignoring exception", e);
            }
        } catch (RuntimeException e) {
            if (!ignore) {
                throw e;
            } else if (log.isDebugEnabled()) {
                log.debug("Ignoring exception", e);
            }
        } finally {
            container.endContext(request);
        }
    }
}
