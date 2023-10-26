package org.apache.maven.shared.release.policies;

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

import org.apache.maven.shared.release.policy.PolicyException;
import org.apache.maven.shared.release.policy.naming.NamingPolicy;
import org.apache.maven.shared.release.policy.naming.NamingPolicyRequest;
import org.apache.maven.shared.release.policy.naming.NamingPolicyResult;
import org.codehaus.plexus.component.annotations.Component;

/**
 *
 * @author Robert Scholte
 * @since 3.0.0
 */
@Component( role = NamingPolicy.class, hint = "default" )
public class DefaultNamingPolicy implements NamingPolicy
{
    @Override
    public NamingPolicyResult getName( NamingPolicyRequest request )
        throws PolicyException
    {
        return new NamingPolicyResult().setName( request.getArtifactId() + "-" + request.getVersion() );
    }
}
