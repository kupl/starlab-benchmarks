package org.apache.commons.digester3;

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

import static org.junit.Assert.assertTrue;

import static org.apache.commons.digester3.binder.DigesterLoader.newLoader;

import org.apache.commons.digester3.binder.AbstractRulesModule;
import org.junit.Test;

public final class Digester162TestCase
{

    @Test
    public void allowCreateObjectsWichTypesAreSpecifiedInAttributeNameOnly()
        throws Exception
    {
        Digester digester = newLoader( new AbstractRulesModule()
        {

            @Override
            protected void configure()
            {
                forPattern( "toplevel" ).createObject().ofTypeSpecifiedByAttribute( "type" );
            }

        })
        .newDigester();

        Object object = digester.parse( getClass().getResource( "digester-162.xml" ) );

        assertTrue( BetaBean.class.isInstance( object ) );
    }

}
