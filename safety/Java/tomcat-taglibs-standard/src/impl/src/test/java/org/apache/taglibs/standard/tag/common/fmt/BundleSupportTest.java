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
package org.apache.taglibs.standard.tag.common.fmt;

import jakarta.servlet.jsp.tagext.BodyContent;

import org.easymock.EasyMock;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

/**
 */
public class BundleSupportTest {

    private BundleSupport tag;

    @Before
    public void setup() {
        tag = new BundleSupport() {
        };
    }

    // test for bugzilla #49900 that bodyContent is not released

    @Test
    public void testBodyContentIsRelease() {
        BodyContent b = EasyMock.createMock(BodyContent.class);
        tag.setBodyContent(b);
        tag.release();
        Assert.assertNull(tag.getBodyContent());
    }
}
