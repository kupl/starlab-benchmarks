package org.apache.maven.shared.release.phase;

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

import org.apache.maven.project.MavenProject;
import org.apache.maven.scm.ScmBranchParameters;
import org.apache.maven.scm.ScmException;
import org.apache.maven.scm.ScmFileSet;
import org.apache.maven.scm.command.branch.BranchScmResult;
import org.apache.maven.scm.manager.NoSuchScmProviderException;
import org.apache.maven.scm.provider.ScmProvider;
import org.apache.maven.scm.repository.ScmRepository;
import org.apache.maven.scm.repository.ScmRepositoryException;
import org.apache.maven.shared.release.ReleaseExecutionException;
import org.apache.maven.shared.release.ReleaseFailureException;
import org.apache.maven.shared.release.ReleaseResult;
import org.apache.maven.shared.release.config.ReleaseDescriptor;
import org.apache.maven.shared.release.env.ReleaseEnvironment;
import org.apache.maven.shared.release.scm.ReleaseScmCommandException;
import org.apache.maven.shared.release.scm.ReleaseScmRepositoryException;
import org.apache.maven.shared.release.scm.ScmRepositoryConfigurator;
import org.apache.maven.shared.release.util.ReleaseUtil;
import org.codehaus.plexus.component.annotations.Component;
import org.codehaus.plexus.component.annotations.Requirement;

import java.io.File;
import java.util.List;

/**
 * Branch the SCM repository.
 *
 * @author <a href="mailto:evenisse@apache.org">Emmanuel Venisse</a>
 */
@Component( role = ReleasePhase.class, hint = "scm-branch" )
public class ScmBranchPhase
    extends AbstractReleasePhase
{
    /**
     * Tool that gets a configured SCM repository from release configuration.
     */
    @Requirement
    private ScmRepositoryConfigurator scmRepositoryConfigurator;

    @Override
    public ReleaseResult execute( ReleaseDescriptor releaseDescriptor, ReleaseEnvironment releaseEnvironment,
                                  List<MavenProject> reactorProjects )
        throws ReleaseExecutionException, ReleaseFailureException
    {
        ReleaseResult relResult = new ReleaseResult();

        validateConfiguration( releaseDescriptor );

        ReleaseDescriptor basedirAlignedReleaseDescriptor =
            ReleaseUtil.createBasedirAlignedReleaseDescriptor( releaseDescriptor, reactorProjects );

        logInfo( relResult, "Branching release with the label " + basedirAlignedReleaseDescriptor.getScmReleaseLabel()
            + "..." );

        ScmRepository repository;
        ScmProvider provider;
        try
        {
            repository =
                scmRepositoryConfigurator.getConfiguredRepository( basedirAlignedReleaseDescriptor.getScmSourceUrl(),
                                                                   releaseDescriptor,
                                                                   releaseEnvironment.getSettings() );

            repository.getProviderRepository().setPushChanges( releaseDescriptor.isPushChanges() );

            repository.getProviderRepository().setWorkItem( releaseDescriptor.getWorkItem() );

            provider = scmRepositoryConfigurator.getRepositoryProvider( repository );

        }
        catch ( ScmRepositoryException e )
        {
            throw new ReleaseScmRepositoryException( e.getMessage(), e.getValidationMessages() );
        }
        catch ( NoSuchScmProviderException e )
        {
            throw new ReleaseExecutionException( "Unable to configure SCM repository: " + e.getMessage(), e );
        }

        BranchScmResult result;
        try
        {
            ScmFileSet fileSet = new ScmFileSet( new File( basedirAlignedReleaseDescriptor.getWorkingDirectory() ) );
            String branchName = releaseDescriptor.getScmReleaseLabel();

            ScmBranchParameters scmBranchParameters = new ScmBranchParameters();
            scmBranchParameters.setMessage( releaseDescriptor.getScmCommentPrefix() + "copy for branch " + branchName );
            scmBranchParameters.setRemoteBranching( releaseDescriptor.isRemoteTagging() );
            scmBranchParameters.setScmRevision( releaseDescriptor.getScmReleasedPomRevision() );
            scmBranchParameters.setPinExternals( releaseDescriptor.isPinExternals() );

            result = provider.branch( repository, fileSet, branchName, scmBranchParameters );
        }
        catch ( ScmException e )
        {
            throw new ReleaseExecutionException( "An error is occurred in the branch process: " + e.getMessage(), e );
        }

        if ( !result.isSuccess() )
        {
            throw new ReleaseScmCommandException( "Unable to branch SCM", result );
        }

        relResult.setResultCode( ReleaseResult.SUCCESS );

        return relResult;
    }

    @Override
    public ReleaseResult simulate( ReleaseDescriptor releaseDescriptor, ReleaseEnvironment releaseEnvironment,
                                   List<MavenProject> reactorProjects )
        throws ReleaseExecutionException, ReleaseFailureException
    {
        ReleaseResult result = new ReleaseResult();

        validateConfiguration( releaseDescriptor );
        ReleaseDescriptor basedirAlignedReleaseDescriptor =
            ReleaseUtil.createBasedirAlignedReleaseDescriptor( releaseDescriptor, reactorProjects );

        logInfo( result, "Full run would branch " + basedirAlignedReleaseDescriptor.getWorkingDirectory() );
        if ( releaseDescriptor.getScmBranchBase() != null )
        {
            logInfo( result, "  to SCM URL " + releaseDescriptor.getScmBranchBase() );
        }
        logInfo( result, "  with label '" + releaseDescriptor.getScmReleaseLabel() + "'" );
        if ( releaseDescriptor.isPinExternals() )
        {
            logInfo( result, "  and pinned externals" );
        }

        result.setResultCode( ReleaseResult.SUCCESS );

        return result;
    }

    private static void validateConfiguration( ReleaseDescriptor releaseDescriptor )
        throws ReleaseFailureException
    {
        if ( releaseDescriptor.getScmReleaseLabel() == null )
        {
            throw new ReleaseFailureException( "A release label is required for committing" );
        }
    }
}
