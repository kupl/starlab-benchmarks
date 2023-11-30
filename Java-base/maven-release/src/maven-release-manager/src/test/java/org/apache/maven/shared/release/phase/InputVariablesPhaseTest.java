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
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.fail;
import static org.mockito.Matchers.eq;
import static org.mockito.Matchers.isA;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import java.util.Collections;
import java.util.List;

import org.apache.maven.model.Model;
import org.apache.maven.project.MavenProject;
import org.apache.maven.shared.release.PlexusJUnit4TestCase;
import org.apache.maven.shared.release.ReleaseExecutionException;
import org.apache.maven.shared.release.config.ReleaseDescriptorBuilder;
import org.apache.maven.shared.release.config.ReleaseUtils;
import org.apache.maven.shared.release.env.DefaultReleaseEnvironment;
import org.codehaus.plexus.components.interactivity.Prompter;
import org.codehaus.plexus.components.interactivity.PrompterException;
import org.junit.Test;

/**
 * Test the variable input phase.
 *
 * @author <a href="mailto:brett@apache.org">Brett Porter</a>
 */
public class InputVariablesPhaseTest
    extends PlexusJUnit4TestCase
{
    private InputVariablesPhase phase;

    @Override
    public void setUp()
        throws Exception
    {
        super.setUp();
        phase = (InputVariablesPhase) lookup( ReleasePhase.class, "input-variables" );
    }

    @Test
    public void testInputVariablesInteractive()
        throws Exception
    {
        // prepare
        Prompter mockPrompter = mock( Prompter.class );
        when( mockPrompter.prompt( isA( String.class ), eq( "artifactId-1.0" ) ) ).thenReturn( "tag-value",
                                                                                               "simulated-tag-value" );
        phase.setPrompter( mockPrompter );

        List<MavenProject> reactorProjects = Collections.singletonList( createProject( "artifactId", "1.0" ) );

        ReleaseDescriptorBuilder builder = new ReleaseDescriptorBuilder();
        builder.addReleaseVersion( "groupId:artifactId", "1.0" );
        builder.setScmSourceUrl( "scm:svn:file://localhost/tmp/scm-repo" );

        // execute
        phase.execute( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "tag-value", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // prepare
        builder = new ReleaseDescriptorBuilder();
        builder.addReleaseVersion( "groupId:artifactId", "1.0" );
        builder.setScmSourceUrl( "scm:svn:file://localhost/tmp/scm-repo" );

        // execute
        phase.simulate( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "simulated-tag-value", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        verify( mockPrompter, times( 2 ) ).prompt( isA( String.class ), eq( "artifactId-1.0" ) );
        verifyNoMoreInteractions( mockPrompter );
    }

    @Test
    public void testUnmappedVersion()
        throws Exception
    {
        List<MavenProject> reactorProjects = Collections.singletonList( createProject( "artifactId", "1.0" ) );

        ReleaseDescriptorBuilder builder = new ReleaseDescriptorBuilder();

        try
        {
            phase.execute( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

            fail( "Expected an exception" );
        }
        catch ( ReleaseExecutionException e )
        {
            assertNull( "check no cause", e.getCause() );
        }

        builder = new ReleaseDescriptorBuilder();

        try
        {
            phase.simulate( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

            fail( "Expected an exception" );
        }
        catch ( ReleaseExecutionException e )
        {
            assertNull( "check no cause", e.getCause() );
        }
    }

    @Test
    public void testInputVariablesNonInteractive()
        throws Exception
    {
        // prepare
        Prompter mockPrompter = mock( Prompter.class );
        phase.setPrompter( mockPrompter );

        List<MavenProject> reactorProjects = Collections.singletonList( createProject( "artifactId", "1.0" ) );

        ReleaseDescriptorBuilder builder = new ReleaseDescriptorBuilder();
        builder.setInteractive( false );
        builder.addReleaseVersion( "groupId:artifactId", "1.0" );
        builder.setScmSourceUrl( "scm:svn:file://localhost/tmp/scm-repo" );

        // execute
        phase.execute( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "artifactId-1.0", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // prepare
        builder = new ReleaseDescriptorBuilder();
        builder.setInteractive( false );
        builder.addReleaseVersion( "groupId:artifactId", "1.0" );
        builder.setScmSourceUrl( "scm:svn:file://localhost/tmp/scm-repo" );

        // execute
        phase.simulate( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "artifactId-1.0", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // never use prompter
        verifyNoMoreInteractions( mockPrompter );
    }

    @Test
    public void testInputVariablesNonInteractiveConfigured()
        throws Exception
    {
        // prepare
        Prompter mockPrompter = mock( Prompter.class );
        phase.setPrompter( mockPrompter );

        List<MavenProject> reactorProjects = Collections.singletonList( createProject( "artifactId", "1.0" ) );

        ReleaseDescriptorBuilder builder = new ReleaseDescriptorBuilder();
        builder.setInteractive( false );
        builder.setScmReleaseLabel( "tag-value" );

        // execute
        phase.execute( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "tag-value", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // prepare
        builder = new ReleaseDescriptorBuilder();
        builder.setInteractive( false );
        builder.setScmReleaseLabel( "simulated-tag-value" );

        // execute
        phase.simulate( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "simulated-tag-value", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // never use prompter
        verifyNoMoreInteractions( mockPrompter );
    }

    @Test
    public void testInputVariablesInteractiveConfigured()
        throws Exception
    {
        // prepare
        Prompter mockPrompter = mock( Prompter.class );
        phase.setPrompter( mockPrompter );

        List<MavenProject> reactorProjects = Collections.singletonList( createProject( "artifactId", "1.0" ) );

        ReleaseDescriptorBuilder builder = new ReleaseDescriptorBuilder();
        builder.setScmReleaseLabel( "tag-value" );

        // execute
        phase.execute( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "tag-value", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // prepare
        builder = new ReleaseDescriptorBuilder();
        builder.setScmReleaseLabel( "simulated-tag-value" );

        // execute
        phase.simulate( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "simulated-tag-value", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // never use prompter
        verifyNoMoreInteractions( mockPrompter );
    }

    @Test
    public void testPrompterException()
        throws Exception
    {
        // prepare
        Prompter mockPrompter = mock( Prompter.class );
        when( mockPrompter.prompt( isA( String.class ),
                                   isA( String.class ) ) ).thenThrow( new PrompterException( "..." ) );
        phase.setPrompter( mockPrompter );

        List<MavenProject> reactorProjects = Collections.singletonList( createProject( "artifactId", "1.0" ) );

        ReleaseDescriptorBuilder builder = new ReleaseDescriptorBuilder();
        builder.addReleaseVersion( "groupId:artifactId", "1.0" );
        builder.setScmSourceUrl( "scm:svn:file://localhost/tmp/scm-repo" );

        // execute
        try
        {
            phase.execute( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

            fail( "Expected an exception" );
        }
        catch ( ReleaseExecutionException e )
        {
            assertEquals( "check cause", PrompterException.class, e.getCause().getClass() );
        }

        // prepare
        builder = new ReleaseDescriptorBuilder();
        builder.addReleaseVersion( "groupId:artifactId", "1.0" );
        builder.setScmSourceUrl( "scm:svn:file://localhost/tmp/scm-repo" );

        // execute
        try
        {
            phase.simulate( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

            fail( "Expected an exception" );
        }
        catch ( ReleaseExecutionException e )
        {
            assertEquals( "check cause", PrompterException.class, e.getCause().getClass() );
        }

        // verify
        verify( mockPrompter, times( 2 ) ).prompt( isA( String.class ), isA( String.class ) );
        verifyNoMoreInteractions( mockPrompter );
    }

    // MRELEASE-110
    @Test
    public void testCvsTag()
        throws Exception
    {
        // prepare
        Prompter mockPrompter = mock( Prompter.class );
        phase.setPrompter( mockPrompter );

        List<MavenProject> reactorProjects = Collections.singletonList( createProject( "artifactId", "1.0" ) );

        ReleaseDescriptorBuilder builder = new ReleaseDescriptorBuilder();
        builder.setInteractive( false );
        builder.addReleaseVersion( "groupId:artifactId", "1.0" );
        builder.setScmSourceUrl( "scm:cvs:pserver:anoncvs@localhost:/tmp/scm-repo:module" );

        // execute
        phase.execute( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "artifactId-1_0", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // prepare
        builder = new ReleaseDescriptorBuilder();
        builder.setInteractive( false );
        builder.addReleaseVersion( "groupId:artifactId", "1.0" );
        builder.setScmSourceUrl( "scm:cvs:pserver:anoncvs@localhost:/tmp/scm-repo:module" );

        // execute
        phase.simulate( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "artifactId-1_0", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // never use prompter
        verifyNoMoreInteractions( mockPrompter );
    }

    // MRELEASE-159
    @Test
    public void testCustomTagFormat()
        throws Exception
    {
        // prepare
        Prompter mockPrompter = mock( Prompter.class );
        phase.setPrompter( mockPrompter );

        List<MavenProject> reactorProjects = Collections.singletonList( createProject( "artifactId", "1.0" ) );
        ReleaseDescriptorBuilder builder = new ReleaseDescriptorBuilder();
        builder.setInteractive( false );
        builder.addReleaseVersion( "groupId:artifactId", "1.0" );
        builder.setScmSourceUrl( "scm:svn:file://localhost/tmp/scm-repo" );

        // execute
        phase.execute( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "artifactId-1.0", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // prepare
        builder = new ReleaseDescriptorBuilder();
        builder.setInteractive( false );
        builder.addReleaseVersion( "groupId:artifactId", "1.0" );
        builder.setScmSourceUrl( "scm:svn:file://localhost/tmp/scm-repo" );
        builder.setScmTagNameFormat( "simulated-@{artifactId}-@{version}" );

        // execute
        phase.simulate( ReleaseUtils.buildReleaseDescriptor( builder ), new DefaultReleaseEnvironment(), reactorProjects );

        // verify
        assertEquals( "Check tag", "simulated-artifactId-1.0", ReleaseUtils.buildReleaseDescriptor( builder ).getScmReleaseLabel() );

        // never use prompter
        verifyNoMoreInteractions( mockPrompter );
    }

    @Test
    public void testBranchOperation()
        throws Exception
    {
        assertFalse( phase.isBranchOperation() );
    }

    private static MavenProject createProject( String artifactId, String version )
    {
        Model model = new Model();
        model.setGroupId( "groupId" );
        model.setArtifactId( artifactId );
        model.setVersion( version );
        return new MavenProject( model );
    }
}
