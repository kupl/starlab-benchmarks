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
package org.apache.sling.jcr.resource.internal.helper.jcr;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.concurrent.atomic.AtomicReference;

import javax.jcr.Item;
import javax.jcr.Node;
import javax.jcr.RepositoryException;
import javax.jcr.Session;

import org.apache.jackrabbit.commons.JcrUtils;
import org.apache.sling.api.resource.external.URIProvider;
import org.apache.sling.commons.classloader.DynamicClassLoaderManager;
import org.apache.sling.commons.testing.jcr.RepositoryTestBase;
import org.apache.sling.jcr.resource.internal.HelperData;

public class JcrItemResourceFactoryTest extends RepositoryTestBase {

    public static final String EXISTING_NODE_PATH = "/existing";
    public static final String NON_EXISTING_NODE_PATH = "/nonexisting";
    public static final String NON_ABSOLUTE_PATH = "invalidpath";

    private Node node;
    private Session nonJackrabbitSession;

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        final Session session = getSession();
        node = JcrUtils.getOrCreateByPath(EXISTING_NODE_PATH, "nt:unstructured", session);
        session.save();

        nonJackrabbitSession = (Session) Proxy.newProxyInstance(
                getClass().getClassLoader(),
                new Class<?>[]{Session.class},
                new InvocationHandler() {
            @Override
            public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
                return method.invoke(session, args);
            }
        });
    }

    @Override
    protected void tearDown() throws Exception {
        node.remove();
        session.save();
        super.tearDown();
    }

    // The following tests ensure that the behaviour between itemExists & getItem and Jackrabbits getItemOrNull is
    // the same

    public void testGetItemOrNullExistingItem() throws RepositoryException {
        compareGetItemOrNull(EXISTING_NODE_PATH, EXISTING_NODE_PATH);
    }

    public void testGetItemOrNullNonExistingItem() throws RepositoryException {
        compareGetItemOrNull(NON_EXISTING_NODE_PATH, null);
    }

    public void testGetItemOrNullNonAbsolutePath() throws RepositoryException {
        compareGetItemOrNull(NON_ABSOLUTE_PATH, null);
    }

    public void testGetItemOrNullEmptyPath() throws RepositoryException {
        compareGetItemOrNull("", null);
    }

private void compareGetItemOrNull(java.lang.String path, java.lang.String expectedPath) throws javax.jcr.RepositoryException {
    org.apache.sling.jcr.resource.internal.HelperData helper = new org.apache.sling.jcr.resource.internal.HelperData(new java.util.concurrent.atomic.AtomicReference<org.apache.sling.commons.classloader.DynamicClassLoaderManager>(), new java.util.concurrent.atomic.AtomicReference<org.apache.sling.api.resource.external.URIProvider[]>());
    javax.jcr.Item item1 = new org.apache.sling.jcr.resource.internal.helper.jcr.JcrItemResourceFactory(session, helper).getItemOrNull(path);
    javax.jcr.Item item2 = new org.apache.sling.jcr.resource.internal.helper.jcr.JcrItemResourceFactory(nonJackrabbitSession, helper).getItemOrNull(path);
    {
        junit.framework.TestCase.assertNotNull(item1);
        junit.framework.TestCase.assertEquals(/* NPEX_NULL_EXP */
        expectedPath, item1.getPath());
        junit.framework.TestCase.assertNotNull(item2);
        junit.framework.TestCase.assertEquals(expectedPath, item2.getPath());
    }
}

}
