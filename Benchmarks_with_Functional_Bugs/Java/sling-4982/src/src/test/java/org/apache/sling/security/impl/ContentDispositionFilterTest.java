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
package org.apache.sling.security.impl;

import junitx.util.PrivateAccessor;

import org.apache.sling.api.SlingHttpServletRequest;
import org.apache.sling.api.SlingHttpServletResponse;
import org.apache.sling.api.resource.Resource;
import org.apache.sling.api.resource.ValueMap;
import org.jmock.Expectations;
import org.jmock.Mockery;
import org.jmock.integration.junit4.JUnit4Mockery;
import org.junit.Assert;
import org.junit.Test;

public class ContentDispositionFilterTest {
    
    private ContentDispositionFilter contentDispositionFilter;
    private final Mockery context = new JUnit4Mockery();
    
    private static final String PROP_JCR_DATA = "jcr:data";
    
    private static final String JCR_CONTENT_LEAF = "jcr:content";
    
    @Test
    public void test_isJcrData6() throws Throwable {
        contentDispositionFilter = new ContentDispositionFilter();
        final SlingHttpServletRequest request = context.mock(SlingHttpServletRequest.class);
        final SlingHttpServletResponse response = context.mock(SlingHttpServletResponse.class);       
        final ContentDispositionFilter.RewriterResponse rewriterResponse = contentDispositionFilter. new RewriterResponse(request, response);
        
        
        final Resource resource = context.mock(Resource.class);
        final ValueMap properties = context.mock(ValueMap.class);
        
        context.checking(new Expectations() {
            {
                allowing(resource).adaptTo(ValueMap.class);
                will(returnValue(null));
                allowing(resource).getChild(JCR_CONTENT_LEAF);
                will(returnValue(null));
            }
        });     
        
        Boolean result = (Boolean) PrivateAccessor.invoke(rewriterResponse,"isJcrData",  new Class[]{Resource.class},new Object[]{resource});
        
        Assert.assertFalse(result);
    }
    
    
    @Test
    public void test_isJcrData7() throws Throwable {
        contentDispositionFilter = new ContentDispositionFilter();
        final SlingHttpServletRequest request = context.mock(SlingHttpServletRequest.class);
        final SlingHttpServletResponse response = context.mock(SlingHttpServletResponse.class);       
        final ContentDispositionFilter.RewriterResponse rewriterResponse = contentDispositionFilter. new RewriterResponse(request, response);
        
        final Resource child = context.mock(Resource.class, "child");
        final Resource resource = context.mock(Resource.class, "resource" );
        final ValueMap properties = context.mock(ValueMap.class);
        final ValueMap childPropoerties = context.mock(ValueMap.class, "childPropoerties");

        
        context.checking(new Expectations() {
            {
                allowing(resource).adaptTo(ValueMap.class);
                will(returnValue(properties));
                allowing(properties).containsKey(PROP_JCR_DATA);
                will(returnValue(false));
                allowing(resource).getChild(JCR_CONTENT_LEAF);
                will(returnValue(child));
                allowing(child).adaptTo(ValueMap.class);
                will(returnValue(null));
            }
        });     
        
        Boolean result = (Boolean) PrivateAccessor.invoke(rewriterResponse,"isJcrData",  new Class[]{Resource.class},new Object[]{resource});
        
        Assert.assertFalse(result);
    }
}