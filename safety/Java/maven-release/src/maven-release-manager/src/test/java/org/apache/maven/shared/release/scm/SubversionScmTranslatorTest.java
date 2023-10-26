package org.apache.maven.shared.release.scm;

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

import junit.framework.TestCase;

public class SubversionScmTranslatorTest
    extends TestCase
{

    private SubversionScmTranslator scmTranslator  = new SubversionScmTranslator();

    /**
     * @see org.apache.maven.model.Scm#getTag()
     */
    public void testResolveTag()
    {
        // "HEAD"is default
        assertNull( scmTranslator.resolveTag( "HEAD" ) );
        assertNull( scmTranslator.resolveTag( "project-1.0" ) );
    }

    public void testTranslateTagUrl()
    {
        assertEquals( "url/tags/tag", scmTranslator.translateTagUrl( "url", "tag", null )  );
        assertEquals( "tagBase/tag", scmTranslator.translateTagUrl( "url", "tag", "tagBase" )  );
    }

    public void testTranslateBranchUrl()
    {
        assertEquals( "url/branches/branchName", scmTranslator.translateBranchUrl( "url", "branchName", null )  );
        assertEquals( "tagBase/branchName", scmTranslator.translateBranchUrl( "url", "branchName", "tagBase" )  );
    }

    public void testGetRelativePath()
    {
        assertEquals( "a/b/c", "a/b/c" );
    }
}
