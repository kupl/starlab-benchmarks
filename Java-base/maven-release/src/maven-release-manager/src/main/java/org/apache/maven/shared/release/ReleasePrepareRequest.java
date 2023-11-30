package org.apache.maven.shared.release;

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

import java.util.Properties;

import org.apache.maven.shared.release.env.ReleaseEnvironment;

/**
 *
 * @author Robert Scholte
 * @since 2.3
 */
public class ReleasePrepareRequest
    extends AbstractReleaseRequest
{
    // using Boolean to detect if has been set explicitly
    private Boolean dryRun;

    // using Boolean to detect if has been set explicitly
    private Boolean resume;

    private ReleaseEnvironment releaseEnvironment;
    
    private Properties userProperties;

    /**
     * @return the dryRun
     */
    public Boolean getDryRun()
    {
        return dryRun;
    }

    /**
     * @param dryRun the dryRun to set
     */
    public void setDryRun( Boolean dryRun )
    {
        this.dryRun = dryRun;
    }

    /**
     * @return the resume
     */
    public Boolean getResume()
    {
        return resume;
    }

    /**
     * @param resume the resume to set
     */
    public void setResume( Boolean resume )
    {
        this.resume = resume;
    }

    /**
     * @return the releaseEnvironment
     */
    public ReleaseEnvironment getReleaseEnvironment()
    {
        return releaseEnvironment;
    }

    /**
     * @param releaseEnvironment the releaseEnvironment to set
     */
    public void setReleaseEnvironment( ReleaseEnvironment releaseEnvironment )
    {
        this.releaseEnvironment = releaseEnvironment;
    }
    
    public Properties getUserProperties()
    {
        return userProperties;
    }
    
    public void setUserProperties( Properties userProperties )
    {
        this.userProperties = userProperties;
    }
}