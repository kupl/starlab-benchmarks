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
package org.apache.felix.metatype;


import java.net.URL;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeSet;

import org.apache.felix.metatype.internal.LocalizedObjectClassDefinition;
import org.apache.felix.metatype.internal.l10n.BundleResources;
import org.apache.felix.metatype.internal.l10n.Resources;
import org.osgi.framework.Bundle;
import org.osgi.framework.Constants;
import org.osgi.service.metatype.MetaTypeProvider;
import org.osgi.service.metatype.ObjectClassDefinition;


/**
 * The <code>DefaultMetaTypeProvider</code> class is an implementation of the
 * <code>MetaTypeProvider</code> interface which is configured for a given
 * bundle using a {@link MetaData} object.
 * <p>
 * This class may be used by clients, e.g. <code>ManagedService</code> or
 * <code>ManagedServiceFactory</code> implementations to easily also implement
 * the <code>MetaTypeProvider</code> interface.
 *
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
public class DefaultMetaTypeProvider implements MetaTypeProvider
{

    private final Bundle bundle;
    private final String localePrefix;

    private Map /* <String, OCD> */ objectClassDefinitions;
    private List /* <Designate> */ designates;
    private String[] locales;


    public DefaultMetaTypeProvider( Bundle bundle, MetaData metadata )
    {
        this.bundle = bundle;

        // copy from holder
        if ( metadata.getObjectClassDefinitions() == null )
        {
            objectClassDefinitions = Collections.EMPTY_MAP;
        }
        else
        {
            Map copy = new HashMap( metadata.getObjectClassDefinitions() );
            objectClassDefinitions = Collections.unmodifiableMap( copy );
        }
        if ( metadata.getDesignates() == null )
        {
            designates = Collections.EMPTY_LIST;
        }
        else
        {
            List copy = new ArrayList( metadata.getDesignates() );
            designates = Collections.unmodifiableList( copy );
        }

        String metaDataLocalePrefix = metadata.getLocalePrefix();
        if ( metaDataLocalePrefix == null )
        {
            metaDataLocalePrefix = ( String ) bundle.getHeaders().get( Constants.BUNDLE_LOCALIZATION );
            if ( metaDataLocalePrefix == null )
            {
                metaDataLocalePrefix = Constants.BUNDLE_LOCALIZATION_DEFAULT_BASENAME;
            }
        }
        this.localePrefix = metaDataLocalePrefix;
    }


    /**
     * Returns the <code>Bundle</code> to which this instance belongs.
     */
    public Bundle getBundle()
    {
        return bundle;
    }


    /* (non-Javadoc)
     * @see org.osgi.service.metatype.MetaTypeProvider#getLocales()
     */
    public String[] getLocales()
    {
        if ( locales == null )
        {
            String path;
            String pattern;
            int lastSlash = localePrefix.lastIndexOf( '/' );
            if ( lastSlash < 0 )
            {
                path = "/";
                pattern = localePrefix;
            }
            else
            {
                path = localePrefix.substring( 0, lastSlash );
                pattern = localePrefix.substring( lastSlash + 1 );
            }

            TreeSet localeSet = new TreeSet();

            Enumeration entries = getBundle().findEntries( path, pattern + "*.properties", false );
            if ( entries != null )
            {
                while ( entries.hasMoreElements() )
                {
                    URL url = ( URL ) entries.nextElement();
                    String name = url.getPath();
                    name = name.substring( name.lastIndexOf( '/' ) + 1 + pattern.length(), name.length()
                        - ".properties".length() );
                    if ( name.startsWith( "_" ) )
                    {
                        name = name.substring( 1 );
                    }
                    if ( name.length() > 0 )
                    {
                        localeSet.add( name );
                    }
                }
            }

            locales = ( String[] ) localeSet.toArray( new String[localeSet.size()] );
        }

        return ( locales.length == 0 ) ? null : locales;
    }


    /* (non-Javadoc)
     * @see org.osgi.service.metatype.MetaTypeProvider#getObjectClassDefinition(java.lang.String, java.lang.String)
     */
    public ObjectClassDefinition getObjectClassDefinition( String id, String locale )
    {
        Designate designate = getDesignate( id );
        if ( designate == null || designate.getObject() == null )
        {
            return null;
        }

        String ocdRef = designate.getObject().getOcdRef();
        OCD ocd = ( OCD ) objectClassDefinitions.get( ocdRef );
        if ( ocd == null )
        {
            return null;
        }

        Resources resources = BundleResources.getResources( bundle, localePrefix, locale );
        return new LocalizedObjectClassDefinition( bundle, ocd, resources );
    }

    public Designate getDesignate(String pid)
    {
        int size = this.designates.size();
        for (int i = 0; i < size; i++)
        {
            Designate designate = (Designate) this.designates.get(i);
            String factoryPid = designate.getFactoryPid();
            if (factoryPid != null && pid.equals(factoryPid))
            {
                return designate;
            }
            else if (factoryPid == null && pid.equals(designate.getPid()))
            {
                return designate;
            }
        }
        return null;
    }

    protected Map /* <String, OCD> */ getObjectClassDefinitions()
    {
        return objectClassDefinitions;
    }

    protected List /* <Designate> */ getDesignates()
    {
        return designates;
    }
}
