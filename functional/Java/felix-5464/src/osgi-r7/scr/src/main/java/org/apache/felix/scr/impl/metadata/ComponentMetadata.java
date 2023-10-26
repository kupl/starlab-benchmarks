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
package org.apache.felix.scr.impl.metadata;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import org.apache.felix.scr.impl.metadata.ServiceMetadata.Scope;
import org.osgi.service.component.ComponentException;

/**
 * This class holds the information associated to a component in the descriptor
 */
public class ComponentMetadata
{
    // Configuration required for component activation (since DS 1.1)
    public static final String CONFIGURATION_POLICY_REQUIRE = "require";

    // Configuration not provided to component (since DS 1.1)
    public static final String CONFIGURATION_POLICY_IGNORE = "ignore";

    // Configuration optional (default) (since DS 1.1)
    public static final String CONFIGURATION_POLICY_OPTIONAL = "optional";

    // set of valid configuration policy settings
    private static final Set<String> CONFIGURATION_POLICY_VALID;

    // marker value indicating duplicate implementation class setting
    private static final String IMPLEMENTATION_CLASS_DUPLICATE = "icd";

    // marker value indicating duplicate service setting
    private static final ServiceMetadata SERVICE_DUPLICATE = new ServiceMetadata();

    // the namespace code of the namespace declaring this component
    private final DSVersion m_dsVersion;

    // 112.4.3: A Globally unique component name (required)
    private String m_name;

    // 112.4.3: Controls whether the component is enabled when the bundle is started. (optional, default is true).
    private boolean m_enabled = true;

    // 112.4.3: Factory identified. If set to a non empty string, it indicates that the component is a factory component (optional).
    private String m_factory;

    // 112.4.3: Controls whether component configurations must be immediately activated after becoming
    // satisfied or whether activation should be delayed. (optional, default value depends
    // on whether the component has a service element or not).
    private Boolean m_immediate;

    // 112.4.4 Implementation Element (required)
    private String m_implementationClassName;

    // 112.5.8 activate can be specified (since DS 1.1)
    private String m_activate;

    // 112.5.8 whether activate has been specified
    private boolean m_activateDeclared = false;

    // 112.5.12 deactivate can be specified (since DS 1.1)
    private String m_deactivate;

    // 112.5.12 whether deactivate has been specified
    private boolean m_deactivateDeclared = false;

    // 112.??.?? modified method (configuration update, since DS 1.1)
    private String m_modified;

    // 112.4.3 configuration-policy (since DS 1.1)
    private String m_configurationPolicy;

    // 112.4.4 configuration-pid (since DS 1.2)
    private List<String> m_configurationPid;

    // activation fields (since DS 1.4)
    private List<String> m_activationFields;

    // Associated properties (0..*)
    private final Map<String, Object> m_properties = new HashMap<>();

    // Associated factory properties (0..*)
    private final Map<String, Object> m_factoryProperties = new HashMap<>();

    // List of Property metadata - used while building the meta data
    // while validating the properties contained in the PropertyMetadata
    // instances are copied to the m_properties Dictionary while this
    // list will be cleared
    private final List<PropertyMetadata> m_propertyMetaData = new ArrayList<>();

    // List of Property metadata - used while building the meta data
    // while validating the properties contained in the PropertyMetadata
    // instances are copied to the m_factoryProperties Dictionary while this
    // list will be cleared
    private final List<PropertyMetadata> m_factoryPropertyMetaData = new ArrayList<>();

    // Provided services (0..1)
    private ServiceMetadata m_service;

    // List of service references, (required services 0..*)
    private final List<ReferenceMetadata> m_references = new ArrayList<>();

    private boolean m_configurableServiceProperties;
    private boolean m_persistentFactoryComponent;
    private boolean m_deleteCallsModify;
    private Boolean m_obsoleteFactoryComponentFactory;
    private boolean m_configureWithInterfaces;
    private boolean m_delayedKeepInstances;

    private String m_init;

    // Flag that is set once the component is verified (its properties cannot be changed)
    private boolean m_validated = false;

    static
    {
        CONFIGURATION_POLICY_VALID = new TreeSet<>();
        CONFIGURATION_POLICY_VALID.add( CONFIGURATION_POLICY_IGNORE );
        CONFIGURATION_POLICY_VALID.add( CONFIGURATION_POLICY_OPTIONAL );
        CONFIGURATION_POLICY_VALID.add( CONFIGURATION_POLICY_REQUIRE );
    }


    public ComponentMetadata( final DSVersion dsVersion )
    {
        this.m_dsVersion = dsVersion;
    }

    /////////////////////////////////////////// SETTERS //////////////////////////////////////

    /**
     * Setter for the configuration-pid component (since DS 1.2)
     * @param configurationPid
     */
    public void setConfigurationPid( String[] configurationPid )
    {
        if ( m_validated )
        {
            return;
        }
        m_configurationPid = new ArrayList<>( Arrays.asList( configurationPid ) );
    }

    /**
     * Setter for the name
     *
     * @param name
     */
    public void setName( String name )
    {
        if ( m_validated )
        {
            return;
        }
        m_name = name;
    }


    /**
     * Setter for the enabled property
     *
     * @param enabled
     */
    public void setEnabled( boolean enabled )
    {
        if ( m_validated )
        {
            return;
        }
        m_enabled = enabled;
    }


    /**
     *
     * @param factoryIdentifier
     */
    public void setFactoryIdentifier( String factoryIdentifier )
    {
        if ( m_validated )
        {
            return;
        }
        m_factory = factoryIdentifier;
    }


    /**
     * Setter for the immediate property
     *
     * @param immediate
     */
    public void setImmediate( boolean immediate )
    {
        if ( m_validated )
        {
            return;
        }
        m_immediate = immediate ? Boolean.TRUE : Boolean.FALSE;
    }


    /**
     * Sets the name of the implementation class
     *
     * @param implementationClassName a class name
     */
    public void setImplementationClassName( String implementationClassName )
    {
        if ( m_validated )
        {
            return;
        }

        // set special flag value if implementation class is already set
        if ( m_implementationClassName != null )
        {
            m_implementationClassName = IMPLEMENTATION_CLASS_DUPLICATE;
        }
        else
        {
            m_implementationClassName = implementationClassName;
        }
    }


    /**
     * Sets the configuration policy
     *
     * @param configurationPolicy configuration policy
     * @since 1.2.0 (DS 1.1)
     */
    public void setConfigurationPolicy( String configurationPolicy )
    {
        if ( m_validated )
        {
            return;
        }
        m_configurationPolicy = configurationPolicy;
    }


    /**
     * Sets the name of the activate method
     *
     * @param activate a method name
     * @since 1.2.0 (DS 1.1)
     */
    public void setActivate( String activate )
    {
        if ( m_validated )
        {
            return;
        }
        m_activate = activate;
        m_activateDeclared = true;
    }


    /**
     * Sets the name of the deactivate method
     *
     * @param deactivate a method name
     * @since 1.2.0 (DS 1.1)
     */
    public void setDeactivate( String deactivate )
    {
        if ( m_validated )
        {
            return;
        }
        m_deactivate = deactivate;
        m_deactivateDeclared = true;
    }


    /**
     * Sets the name of the modified method
     *
     * @param modified a method name
     * @since 1.2.0 (DS 1.1)
     */
    public void setModified( String modified )
    {
        if ( m_validated )
        {
            return;
        }
        m_modified = modified;
    }


    /**
     * Used to add a property to the instance
     *
     * @param newProperty a property metadata object
     */
    public void addProperty( PropertyMetadata newProperty )
    {
        if ( m_validated )
        {
            return;
        }
        if ( newProperty == null )
        {
            throw new IllegalArgumentException( "Cannot add a null property" );
        }
        m_propertyMetaData.add( newProperty );
    }


    /**
     * Used to add a factory property to the instance
     *
     * @param newProperty a property metadata object
     */
	public void addFactoryProperty( PropertyMetadata newProperty )
	{
        if ( m_validated )
        {
            return;
        }
        if ( newProperty == null )
        {
            throw new IllegalArgumentException( "Cannot add a null property" );
        }
        m_factoryPropertyMetaData.add( newProperty );
	}

    /**
     * Used to set a ServiceMetadata object.
     *
     * @param service a ServiceMetadata
     */
    public void setService( ServiceMetadata service )
    {
        if ( m_validated )
        {
            return;
        }

        // set special flag value if implementation class is already set
        if ( m_service != null )
        {
            m_service = SERVICE_DUPLICATE;
        }
        else
        {
            m_service = service;
        }
    }


    /**
     * Used to add a reference metadata to the component
     *
     * @param newReference a new ReferenceMetadata to be added
     */
    public void addDependency( ReferenceMetadata newReference )
    {
        if ( m_validated )
        {
            return;
        }
        if ( newReference == null )
        {
            throw new IllegalArgumentException( "Cannot add a null ReferenceMetadata" );
        }
        m_references.add( newReference );
    }

    public void setConfigurableServiceProperties( boolean configurableServiceProperties) {
        if ( m_validated )
        {
            return;
        }
		this.m_configurableServiceProperties = configurableServiceProperties;
	}

	public void setPersistentFactoryComponent(boolean persistentFactoryComponent) {
        if ( m_validated )
        {
            return;
        }
		this.m_persistentFactoryComponent = persistentFactoryComponent;
	}

	public void setDeleteCallsModify(boolean deleteCallsModify) {
        if ( m_validated )
        {
            return;
        }
		this.m_deleteCallsModify = deleteCallsModify;
	}

	public void setObsoleteFactoryComponentFactory( boolean obsoleteFactoryComponentFactory) {
        if ( m_validated )
        {
            return;
        }
		this.m_obsoleteFactoryComponentFactory = obsoleteFactoryComponentFactory;
	}

	public void setConfigureWithInterfaces(boolean configureWithInterfaces) {
		this.m_configureWithInterfaces = configureWithInterfaces;
	}

	public void setDelayedKeepInstances(boolean delayedKeepInstances) {
        if ( m_validated )
        {
            return;
        }
		this.m_delayedKeepInstances = delayedKeepInstances;
	}



	public void setActivationFields( final String[] fields )
	{
		if ( !m_validated )
		{
	        this.m_activationFields = new ArrayList<>( Arrays.asList( fields ) );
		}
	}

	public void setInit( final String value )
	{
	    if ( !m_validated )
	    {
	        this.m_init = value;
	    }
	}

    /////////////////////////////////////////// GETTERS //////////////////////////////////////

	/**
     * Returns the namespace code of the namespace of the component element
     * declaring this component. This is one of the XmlHandler.DS_VERSION_*
     * constants.
     */
    public DSVersion getDSVersion()
    {
        return m_dsVersion;
    }


    /**
     * Returns the name of the component
     *
     * @return A string containing the name of the component
     */
    public String getName()
    {
        // FELIX-2325: Be lenient here and return the name if set or
        // the implementation class name. This allows for the
        // BundleComponentActivator.loadComponents method to access the
        // name before validating the component, which then makes sure
        // that the name may only be unset for DS 1.1 and newer components

        if ( m_name != null )
        {
            return m_name;
        }

        // return the implementation class name if the name is not set
        return getImplementationClassName();
    }

    /**
     * Returns the configuration pid for the component. The pid is the one specified in the
     * component's configuration-pid DS 1.2 attribute, if specified. Else the component name is used
     * as the pid by default.
     */
    public List<String> getConfigurationPid()
    {
        if ( !m_validated )
        {
            throw new IllegalStateException("not yet validated");
        }
        return m_configurationPid;
    }

    public int getPidIndex(TargetedPID pid)
    {
        if ( !m_validated )
        {
            throw new IllegalStateException("not yet validated");
        }
        if (m_configurationPid == null )
        {
        	throw new IllegalStateException( "Apparently trying to configure a component " + m_name + " without a configurationPid using " + pid);
        }
    	return m_configurationPid.indexOf(pid.getServicePid());
    }

    /**
     * Returns whether the configuration-pid has been declared in the descriptor
     * or not.
     *
     * @return whether the configuration-pid has method has been declared in the descriptor
     *      or not.
     * @since DS 1.2
     */
    public boolean isConfigurationPidDeclared()
    {
        return m_configurationPid != null;
    }

    /**
     * Returns the value of the enabled flag
     *
     * @return a boolean containing the value of the enabled flag
     */
    public boolean isEnabled()
    {
        return m_enabled;
    }


    /**
     * Returns the factory identifier
     *
     * @return A string containing a factory identifier or null
     */
    public String getFactoryIdentifier()
    {
        return m_factory;
    }


    /**
     * Returns the flag that defines the activation policy for the component.
     * <p>
     * This method may only be trusted after this instance has been validated
     * by the {@link #validate( )} call. Else it will either return the value
     * of an explicitly set "immediate" attribute or return false if a service
     * element or the factory attribute is set or true otherwise. This latter
     * default value deduction may be unsafe while the descriptor has not been
     * completely read.
     *
     * @return a boolean that defines the activation policy
     */
    public boolean isImmediate()
    {
        // return explicit value if known
        if ( m_immediate != null )
        {
            return m_immediate.booleanValue();
        }

        // deduce default from service element and factory attribute presence
        return m_service == null && m_factory == null;
    }


    /**
     * Returns the name of the implementation class
     *
     * @return the name of the implementation class
     */
    public String getImplementationClassName()
    {
        return m_implementationClassName;
    }


    /**
     * Returns the configuration Policy
     *
     * @return the configuration policy
     * @since 1.2.0 (DS 1.1)
     */
    public String getConfigurationPolicy()
    {
        return m_configurationPolicy;
    }


    /**
     * Returns the name of the activate method
     *
     * @return the name of the activate method
     * @since 1.2.0 (DS 1.1)
     */
    public String getActivate()
    {
        return m_activate;
    }


    /**
     * Returns whether the activate method has been declared in the descriptor
     * or not.
     *
     * @return whether the activate method has been declared in the descriptor
     *      or not.
     * @since 1.2.0 (DS 1.1)
     */
    public boolean isActivateDeclared()
    {
        return m_activateDeclared;
    }

    /**
     * Returns the number of constructor parameters (0 is default)
     * @return The number of constructor parameters
     * @since 2.1.0 (DS 1.4)
     */
    public int getNumberOfConstructorParameters()
    {
        // validate() ensures this is a valid integer
      	return m_init == null ? 0 : Integer.valueOf(m_init);
    }

    /**
     * Returns the names of the activation fields
     *
     * @return the list of activation fields or {@code null}
     * @since 2.1.0 (DS 1.4)
     */
    public List<String> getActivationFields()
    {
        return m_activationFields;
    }


    /**
     * Returns the name of the deactivate method
     *
     * @return the name of the deactivate method
     * @since 1.2.0 (DS 1.1)
     */
    public String getDeactivate()
    {
        return m_deactivate;
    }


    /**
     * Returns whether the deactivate method has been declared in the descriptor
     * or not.
     *
     * @return whether the deactivate method has been declared in the descriptor
     *      or not.
     * @since 1.2.0 (DS 1.1)
     */
    public boolean isDeactivateDeclared()
    {
        return m_deactivateDeclared;
    }


    /**
     * Returns the name of the modified method
     *
     * @return the name of the modified method
     * @since 1.2.0 (DS 1.1)
     */
    public String getModified()
    {
        return m_modified;
    }


    /**
     * Returns the associated ServiceMetadata
     *
     * @return a ServiceMetadata object or null if the Component does not provide any service
     */
    public ServiceMetadata getServiceMetadata()
    {
        return m_service;
    }

    public Scope getServiceScope()
    {
    	if (m_service == null)
    	{
    		return Scope.singleton;
    	}
    	return m_service.getScope();
    }


    /**
     * Returns the properties.
     *
     * @return the properties as a Dictionary
     */
    public Map<String, Object> getProperties()
    {
        return m_properties;
    }


    /**
     * Returns the factory properties.
     *
     * @return the factory properties as a Dictionary
     */
    public Map<String, Object> getFactoryProperties()
    {
        return m_factoryProperties;
    }


    /**
     * Returns the list of property meta data.
     * <b>Note: This method is intended for unit testing only</b>
     *
     * @return the list of property meta data.
     */
    List<PropertyMetadata> getPropertyMetaData()
    {
        return m_propertyMetaData;
    }


    /**
     * Returns the list of factory property meta data.
     * <b>Note: This method is intended for unit testing only</b>
     *
     * @return the list of property meta data.
     */
    List<PropertyMetadata> getFactoryPropertyMetaData()
    {
        return m_factoryPropertyMetaData;
    }


    /**
     * Returns the dependency descriptors
     *
     * @return a Collection of dependency descriptors
     */
    public List<ReferenceMetadata> getDependencies()
    {
        return m_references;
    }


    /**
     * Test to see if this service is a factory
     *
     * @return true if it is a factory, false otherwise
     */
    public boolean isFactory()
    {
        return m_factory != null;
    }


    /**
     * Returns <code>true</code> if the configuration policy is configured to
     * {@link #CONFIGURATION_POLICY_REQUIRE}.
     */
    public boolean isConfigurationRequired()
    {
        return CONFIGURATION_POLICY_REQUIRE.equals( m_configurationPolicy );
    }


    /**
     * Returns <code>true</code> if the configuration policy is configured to
     * {@link #CONFIGURATION_POLICY_IGNORE}.
     */
    public boolean isConfigurationIgnored()
    {
        return CONFIGURATION_POLICY_IGNORE.equals( m_configurationPolicy );
    }


    /**
     * Returns <code>true</code> if the configuration policy is configured to
     * {@link #CONFIGURATION_POLICY_OPTIONAL}.
     */
    public boolean isConfigurationOptional()
    {
        return CONFIGURATION_POLICY_OPTIONAL.equals( m_configurationPolicy );
    }


    public boolean isConfigurableServiceProperties() {
		return m_configurableServiceProperties;
	}

	public boolean isPersistentFactoryComponent() {
		return m_persistentFactoryComponent;
	}

	public boolean isDeleteCallsModify() {
		return m_deleteCallsModify;
	}

	public boolean isObsoleteFactoryComponentFactory() {
		return m_obsoleteFactoryComponentFactory == null ? false : m_obsoleteFactoryComponentFactory;
	}

	public boolean isConfigureWithInterfaces() {
		return m_configureWithInterfaces;
	}

	public boolean isDelayedKeepInstances() {
		return m_delayedKeepInstances;
	}

	/**
     * Method used to verify if the semantics of this metadata are correct
     */
    public void validate( )
    {
        // nothing to do if already validated
        if ( m_validated )
        {
            return;
        }

        // 112.10 The name of the component is required
        if ( m_name == null )
        {
            // 112.4.3 name is optional defaulting to implementation class name since DS 1.1
            if ( !m_dsVersion.isDS11() )
            {
                throw new ComponentException( "The component name has not been set" );
            }
            setName( getImplementationClassName() );
        }

        // 112.10 There must be one implementation element and the class atribute is required
        if ( m_implementationClassName == null )
        {
            throw validationFailure( "Implementation class name missing" );
        }
        else if ( m_implementationClassName == IMPLEMENTATION_CLASS_DUPLICATE )
        {
            throw validationFailure( "Implementation element must occur exactly once" );
        }

        // 112.4.3 configuration-policy (since DS 1.1)
        if ( m_configurationPolicy == null )
        {
            // default if not specified or pre DS 1.1
            m_configurationPolicy = CONFIGURATION_POLICY_OPTIONAL;
        }
        else if ( !m_dsVersion.isDS11() )
        {
            throw validationFailure( "configuration-policy declaration requires DS 1.1 or later namespace " );
        }
        else if ( !CONFIGURATION_POLICY_VALID.contains( m_configurationPolicy ) )
        {
            throw validationFailure( "configuration-policy must be one of " + CONFIGURATION_POLICY_VALID );
        }

        // 112.5.8 activate can be specified (since DS 1.1)
        if ( m_activate == null )
        {
            // default if not specified or pre DS 1.1
            m_activate = "activate";
        }
        else if ( !m_dsVersion.isDS11() )
        {
            throw validationFailure( "activate method declaration requires DS 1.1 or later namespace " );
        }

        // 112.5.12 deactivate can be specified (since DS 1.1)
        if ( m_deactivate == null )
        {
            // default if not specified or pre DS 1.1
            m_deactivate = "deactivate";
        }
        else if ( !m_dsVersion.isDS11() )
        {
            throw validationFailure( "deactivate method declaration requires DS 1.1 or later namespace " );
        }

        // 112.??.?? modified can be specified (since DS 1.1)
        if ( m_modified != null && !m_dsVersion.isDS11() )
        {
            throw validationFailure( "modified method declaration requires DS 1.1 or later namespace " );
        }

        // 112.4.4 configuration-pid can be specified since DS 1.2
        if ( m_configurationPid == null )
        {
            m_configurationPid = Collections.singletonList( getName() );
        }
        else
        {
            if ( !m_dsVersion.isDS12() )
            {
                throw validationFailure( "configuration-pid attribute requires DS 1.2 or later namespace " );
            }
            if (m_configurationPid.isEmpty())
            {
                throw validationFailure( "configuration-pid nust not be empty string " );
            }
            if (m_configurationPid.size() > 1 && !m_dsVersion.isDS13())
            {
                throw validationFailure( "multiple configuration-pid requires DS 1.3 or later namespace " );
            }
            for (int i = 0; i < m_configurationPid.size(); i++)
            {
                if ("$".equals( m_configurationPid.get(i)))
                {
                    if (!m_dsVersion.isDS13())
                    {
                        throw validationFailure( "Use of '$' configuration-pid wildcard requires DS 1.3 or later namespace " );
                    }
                    m_configurationPid.set( i, getName() );
                }
            }
            if ( new HashSet<>( m_configurationPid ).size() != m_configurationPid.size())
            {
                throw validationFailure( "Duplicate pids not allowed: " + m_configurationPid );
            }
        }

        // Next check if the properties are valid (and extract property values)
        for ( PropertyMetadata propMeta: m_propertyMetaData )
        {
            propMeta.validate( this );
            m_properties.put( propMeta.getName(), propMeta.getValue() );
        }
        m_propertyMetaData.clear();

        // Next check if the factory properties are valid (and extract property values)
        if ( !m_dsVersion.isDS14() && !m_factoryPropertyMetaData.isEmpty() )
        {
            throw validationFailure( "Use of factory properties requires DS 1.4 or later namespace " );
        }
        if ( m_dsVersion.isDS14() && isFactory() )
        {
        	    for ( PropertyMetadata propMeta: m_factoryPropertyMetaData )
        	    {
        		    propMeta.validate( this );
        		    m_factoryProperties.put( propMeta.getName(), propMeta.getValue() );
        	    }
        }
        // if this is not a factory, these props are ignored, so nothing else to do
        m_factoryPropertyMetaData.clear();

        // Check that the provided services are valid too
        if ( m_service == SERVICE_DUPLICATE )
        {
            throw validationFailure( "Service element must occur at most once" );
        }
        else if ( m_service != null )
        {
            m_service.validate( this );
        }

        // Check that the references are ok
        Set<String> refs = new HashSet<>();
        for ( ReferenceMetadata refMeta: m_references )
        {
            refMeta.validate( this );

            // flag duplicates
            if ( !refs.add( refMeta.getName() ) )
            {
                throw validationFailure( "Detected duplicate reference name: ''" + refMeta.getName() + "''" );
            }
        }

        // verify value of immediate attribute if set
        if ( m_immediate != null )
        {
            if ( isImmediate() )
            {
                // FELIX-593: 112.4.3 clarification, immediate is false for factory
                if ( isFactory() )
                {
                    throw validationFailure( "Factory cannot be immediate" );
                }
            }
            else
            {
                // 112.2.3 A delayed component specifies a service, is not specified to be a factory component
                // and does not have the immediate attribute of the component element set to true.
                // FELIX-593: 112.4.3 clarification, immediate may be true for factory
                if ( m_service == null && !isFactory() )
                {
                    throw validationFailure( "Delayed must provide a service or be a factory" );
                }
            }
        }

        // 112.4.6 The serviceFactory attribute (of a provided service) must not be true if
        // the component is a factory component or an immediate component
        if ( m_service != null )
        {
            if ( (m_service.getScope() != ServiceMetadata.Scope.singleton) && ( isFactory() || isImmediate() ) )
            {
                throw validationFailure( "factory or immediate must be scope singleton not " +  m_service.getScope());
            }
        }

        // activation fields require DS 1.4
        if ( m_activationFields != null && !m_dsVersion.isDS14() )
        {
            throw validationFailure( "Activation fields require version 1.4 or later");
        }

        // constructor injection requires DS 1.4
        if ( this.m_init != null )
        {
            if ( !m_dsVersion.isDS14() )
            {
                throw validationFailure( "Constructor injection requires version 1.4 or later");
            }
            int constructorParameters = 0;
            try
            {
                constructorParameters = Integer.valueOf(m_init);
                if ( constructorParameters < 0)
                {
                    throw validationFailure( "Init parameter must have non negative value: " + m_init);
                }
            }
            catch ( final NumberFormatException nfe)
            {
                throw validationFailure( "Init parameter is not a number: " + m_init);
            }
        }

        if (m_dsVersion == DSVersion.DS12Felix)
        {
            m_configurableServiceProperties = true;
        }
        if ( m_configurableServiceProperties && getServiceScope() != Scope.singleton )
        {
            throw validationFailure( "configurable service properties only allowed with singleton scope" );
        }
        if (m_dsVersion.isDS13())
        {
        	    m_deleteCallsModify = true; //spec behavior as of 1.3
        }
        if ( !m_dsVersion.isDS13() && m_configureWithInterfaces)
        {
        	    throw validationFailure("Configuration with interfaces or annotations only possible with version 1.3 or later");
        }
        if (m_dsVersion.isDS13() && m_obsoleteFactoryComponentFactory != null)
        {
          	throw validationFailure("Configuration of component factory instances through config admin factory pids supported only through the 1.2 namespace");
        }
        if (m_persistentFactoryComponent && !isFactory())
        {
         	throw validationFailure("Only a factory component can be a persistent factory component");
        }

        m_validated = true;
    }


    /**
     * Returns a <code>ComponentException</code> for this component with the
     * given explanation for failure.
     *
     * @param reason The explanation for failing to validate this component.
     */
    ComponentException validationFailure( String reason )
    {
        return new ComponentException( "Component " + getName() + " validation failed: " + reason );
    }
}
