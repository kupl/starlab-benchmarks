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
package org.apache.felix.cm.impl;


import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.lang.reflect.Field;
import java.util.Dictionary;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Map;
import java.util.Set;
import org.apache.felix.cm.MockBundleContext;
import org.apache.felix.cm.MockLogService;
import org.apache.felix.cm.MockNotCachablePersistenceManager;
import org.apache.felix.cm.MockPersistenceManager;
import org.apache.felix.cm.PersistenceManager;
import org.mockito.Mockito;
import org.osgi.framework.Bundle;
import org.osgi.framework.Constants;
import org.osgi.framework.ServiceReference;
import org.osgi.framework.ServiceRegistration;
import org.osgi.service.cm.ConfigurationEvent;
import org.osgi.service.cm.SynchronousConfigurationListener;
import org.osgi.service.log.LogService;
import org.osgi.util.tracker.ServiceTracker;
import junit.framework.TestCase;

public class ConfigurationManagerTest extends TestCase
{

    private PrintStream replacedStdErr;

    private ByteArrayOutputStream output;


    @Override
    protected void setUp() throws Exception
    {
        super.setUp();

        replacedStdErr = System.err;

        output = new ByteArrayOutputStream();
        System.setErr( new PrintStream( output ) );
    }


    @Override
    protected void tearDown() throws Exception
    {
        System.setErr( replacedStdErr );

        super.tearDown();
    }

    public void test_listConfigurations_cached() throws Exception
    {
        String pid = "testDefaultPersistenceManager";
        ConfigurationManager configMgr = new ConfigurationManager();
        setServiceTrackerField( configMgr, "persistenceManagerTracker" );
        
        Field field = configMgr.getClass().getDeclaredField( "persistenceManagers" );
        field.setAccessible( true );
        CachingPersistenceManagerProxy[] persistenceManagers = new CachingPersistenceManagerProxy [1];
        PersistenceManager pm =new MockPersistenceManager(); 
        Dictionary dictionary = new Hashtable();
        dictionary.put( "property1", "value1" );
        dictionary.put( Constants.SERVICE_PID, pid );
        pm.store( pid, dictionary );

        persistenceManagers[0] = new CachingPersistenceManagerProxy(pm);
        field.set(configMgr, persistenceManagers);
        
        ConfigurationImpl[] conf = configMgr.listConfigurations(new ConfigurationAdminImpl(configMgr, null), null);
        
        assertEquals(1, conf.length);
        assertEquals(2, conf[0].getProperties(true).size());
        
        dictionary = new Hashtable();
        dictionary.put( "property1", "value2" );
        pid = "testDefaultPersistenceManager";
        dictionary.put( Constants.SERVICE_PID, pid );
        pm.store( pid, dictionary );
 
        conf = configMgr.listConfigurations(new ConfigurationAdminImpl(configMgr, null), null);
        assertEquals(1, conf.length);
        assertEquals(2, conf[0].getProperties(true).size());

        // verify that the property in the configurations cache was used
        assertEquals("value1", conf[0].getProperties(true).get("property1"));
    }    
    
    public void test_listConfigurations_notcached() throws Exception
    {
        String pid = "testDefaultPersistenceManager";         
        ConfigurationManager configMgr = new ConfigurationManager();
        setServiceTrackerField( configMgr, "persistenceManagerTracker" );
        
        Field field = configMgr.getClass().getDeclaredField( "persistenceManagers" );
        field.setAccessible( true );
        CachingPersistenceManagerProxy[] persistenceManagers = new CachingPersistenceManagerProxy[1];
        PersistenceManager pm =new MockNotCachablePersistenceManager(); 
        Dictionary dictionary = new Hashtable();
        dictionary.put( "property1", "value1" );
        dictionary.put( Constants.SERVICE_PID, pid );
        pm.store( pid, dictionary );

        persistenceManagers[0] = new CachingPersistenceManagerProxy(pm);
        field.set(configMgr, persistenceManagers);
        
        ConfigurationImpl[] conf = configMgr.listConfigurations(new ConfigurationAdminImpl(configMgr, null), null);
        
        assertEquals(1, conf.length);
        assertEquals(2, conf[0].getProperties(true).size());
        
        dictionary = new Hashtable();
        dictionary.put("property1", "valueNotCached");
        pid = "testDefaultPersistenceManager";
        dictionary.put( Constants.SERVICE_PID, pid );
        pm.store( pid, dictionary );
 
        conf = configMgr.listConfigurations(new ConfigurationAdminImpl(configMgr, null), null);
        assertEquals(1, conf.length);
        assertEquals(2, conf[0].getProperties(true).size());

        // verify that the value returned was not the one from the cache
        assertEquals("valueNotCached", conf[0].getProperties(true).get("property1"));
    }

    public void testLogNoLogService()
    {
        ConfigurationManager configMgr = createConfigurationManager( null );

        setLogLevel( configMgr, LogService.LOG_WARNING );
        assertNoLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertNoLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        setLogLevel( configMgr, LogService.LOG_ERROR );
        assertNoLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertNoLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertNoLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        // lower than error -- no output
        setLogLevel( configMgr, LogService.LOG_ERROR - 1 );
        assertNoLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertNoLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertNoLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertNoLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        // minimal log level -- no output
        setLogLevel( configMgr, Integer.MIN_VALUE );
        assertNoLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertNoLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertNoLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertNoLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        setLogLevel( configMgr, LogService.LOG_INFO );
        assertNoLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        setLogLevel( configMgr, LogService.LOG_DEBUG );
        assertLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        // maximal log level -- all output
        setLogLevel( configMgr, Integer.MAX_VALUE );
        assertLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );
    }


    // this test always expects output since when using a LogService, the log
    // level property is ignored
    public void testLogWithLogService()
    {
        LogService logService = new MockLogService();
        ConfigurationManager configMgr = createConfigurationManager( logService );

        setLogLevel( configMgr, LogService.LOG_WARNING );
        assertLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        setLogLevel( configMgr, LogService.LOG_ERROR );
        assertLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        setLogLevel( configMgr, LogService.LOG_ERROR - 1 );
        assertLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        setLogLevel( configMgr, Integer.MIN_VALUE );
        assertLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        setLogLevel( configMgr, LogService.LOG_INFO );
        assertLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        setLogLevel( configMgr, LogService.LOG_DEBUG );
        assertLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );

        setLogLevel( configMgr, Integer.MAX_VALUE );
        assertLog( configMgr, LogService.LOG_DEBUG, "Debug Test Message", null );
        assertLog( configMgr, LogService.LOG_INFO, "Info Test Message", null );
        assertLog( configMgr, LogService.LOG_WARNING, "Warning Test Message", null );
        assertLog( configMgr, LogService.LOG_ERROR, "Error Test Message", null );
    }


    public void testLogSetup()
    {
        final MockBundleContext bundleContext = new MockBundleContext();
        ConfigurationManager configMgr = createConfigurationManager( null );

        // ensure the configuration data goes to target
        bundleContext.setProperty( "felix.cm.dir", "target/config" );

        // default value is 2
        bundleContext.setProperty( "felix.cm.loglevel", null );
        configMgr.start( bundleContext );
        assertEquals( 2, getLogLevel( configMgr ) );
        configMgr.stop( bundleContext );

        // illegal number yields default value
        bundleContext.setProperty( "felix.cm.loglevel", "not-a-number" );
        configMgr.start( bundleContext );
        assertEquals( 2, getLogLevel( configMgr ) );
        configMgr.stop( bundleContext );

        bundleContext.setProperty( "felix.cm.loglevel", "-100" );
        configMgr.start( bundleContext );
        assertEquals( -100, getLogLevel( configMgr ) );
        configMgr.stop( bundleContext );

        bundleContext.setProperty( "felix.cm.loglevel", "4" );
        configMgr.start( bundleContext );
        assertEquals( 4, getLogLevel( configMgr ) );
        configMgr.stop( bundleContext );
    }


    public void testEventsStartingBundle() throws Exception
    {
        final Set<String> result = new HashSet<String>();

        SynchronousConfigurationListener syncListener1 = new SynchronousConfigurationListener()
        {
            @Override
            public void configurationEvent(ConfigurationEvent event)
            {
                result.add("L1");
            }
        };
        SynchronousConfigurationListener syncListener2 = new SynchronousConfigurationListener()
        {
            @Override
            public void configurationEvent(ConfigurationEvent event)
            {
                result.add("L2");
            }
        };
        SynchronousConfigurationListener syncListener3 = new SynchronousConfigurationListener()
        {
            @Override
            public void configurationEvent(ConfigurationEvent event)
            {
                result.add("L3");
            }
        };

        ServiceReference mockRef = Mockito.mock( ServiceReference.class );
        ServiceRegistration mockReg = Mockito.mock( ServiceRegistration.class );
        Mockito.when( mockReg.getReference() ).thenReturn( mockRef );

        ConfigurationManager configMgr = new ConfigurationManager();

        setServiceTrackerField( configMgr, "logTracker" );
        setServiceTrackerField( configMgr, "configurationListenerTracker" );
        ServiceReference[] refs =
                setServiceTrackerField( configMgr, "syncConfigurationListenerTracker",
                        syncListener1, syncListener2, syncListener3 );
        for ( int i=0; i < refs.length; i++)
        {
            Bundle mockBundle = Mockito.mock( Bundle.class );

            switch (i)
            {
            case 0:
                Mockito.when( mockBundle.getState() ).thenReturn( Bundle.ACTIVE );
                break;
            case 1:
                Mockito.when( mockBundle.getState() ).thenReturn( Bundle.STARTING );
                break;
            case 2:
                Mockito.when( mockBundle.getState() ).thenReturn( Bundle.STOPPING );
                break;
            }

            Mockito.when( refs[i].getBundle() ).thenReturn( mockBundle );
        }

        Field srField = configMgr.getClass().getDeclaredField( "configurationAdminRegistration" );
        srField.setAccessible( true );
        srField.set( configMgr, mockReg );
        Field utField = configMgr.getClass().getDeclaredField( "updateThread" );
        utField.setAccessible( true );
        utField.set( configMgr, new UpdateThread( configMgr, null, "Test updater" ));

        Dictionary<String, Object> props = new Hashtable<String, Object>();
        props.put( Constants.SERVICE_PID, "org.acme.testpid" );
        ConfigurationImpl config = new ConfigurationImpl( configMgr, new MockPersistenceManager(), props );
        configMgr.updated( config, true );

        assertEquals("Both listeners should have been called, both in the STARTING and ACTIVE state, but not in the STOPPING state",
                2, result.size());
    }

    private void assertNoLog( ConfigurationManager configMgr, int level, String message, Throwable t )
    {
        try
        {
            configMgr.log( level, message, t );
            assertTrue( "Expecting no log output", output.size() == 0 );
        }
        finally
        {
            // clear the output for future data
            output.reset();
        }
    }


    private void assertLog( ConfigurationManager configMgr, int level, String message, Throwable t )
    {
        try
        {
            configMgr.log( level, message, t );
            assertTrue( "Expecting log output", output.size() > 0 );

            final String expectedLog = MockLogService.toMessageLine( level, message );
            final String actualLog = new String( output.toByteArray() );
            assertEquals( "Log Message not correct", expectedLog, actualLog );

        }
        finally
        {
            // clear the output for future data
            output.reset();
        }
    }


    private static void setLogLevel( ConfigurationManager configMgr, int level )
    {
        final String fieldName = "logLevel";
        try
        {
            Field field = configMgr.getClass().getDeclaredField( fieldName );
            field.setAccessible( true );
            field.setInt( configMgr, level );
        }
        catch ( Throwable ignore )
        {
            throw ( IllegalArgumentException ) new IllegalArgumentException( "Cannot set logLevel field value" )
                .initCause( ignore );
        }
    }


    private static int getLogLevel( ConfigurationManager configMgr )
    {
        final String fieldName = "logLevel";
        try
        {
            Field field = configMgr.getClass().getDeclaredField( fieldName );
            field.setAccessible( true );
            return field.getInt( configMgr );
        }
        catch ( Throwable ignore )
        {
            throw ( IllegalArgumentException ) new IllegalArgumentException( "Cannot get logLevel field value" )
                .initCause( ignore );
        }
    }


    private static ConfigurationManager createConfigurationManager( final LogService logService )
    {
        ConfigurationManager configMgr = new ConfigurationManager();

        try
        {
            Field field = configMgr.getClass().getDeclaredField( "logTracker" );
            field.setAccessible( true );
            field.set( configMgr, new ServiceTracker( new MockBundleContext(), "", null )
            {
                @Override
                public Object getService()
                {
                    return logService;
                }
            } );
        }
        catch ( Throwable ignore )
        {
            throw ( IllegalArgumentException ) new IllegalArgumentException( "Cannot set logTracker field value" )
                .initCause( ignore );
        }

        return configMgr;
    }

    private static ServiceReference[] setServiceTrackerField( ConfigurationManager configMgr,
            String fieldName, Object ... services ) throws Exception
    {
        final Map<ServiceReference, Object> refMap = new HashMap<ServiceReference, Object>();
        for ( Object svc : services )
        {
            ServiceReference sref = Mockito.mock( ServiceReference.class );
            Mockito.when( sref.getProperty( "objectClass" ) ).thenReturn(new String[] { "TestService" });
            refMap.put( sref, svc );
        }

        Field field = configMgr.getClass().getDeclaredField( fieldName );
        field.setAccessible( true );
        field.set( configMgr, new ServiceTracker( new MockBundleContext(), "", null )
        {
            @Override
            public ServiceReference[] getServiceReferences()
            {
                return refMap.keySet().toArray( new ServiceReference[0] );
            }

            @Override
            public Object getService(ServiceReference reference)
            {
                return refMap.get( reference );
            }
        } );

        return refMap.keySet().toArray(new ServiceReference[0]);
    }
}
