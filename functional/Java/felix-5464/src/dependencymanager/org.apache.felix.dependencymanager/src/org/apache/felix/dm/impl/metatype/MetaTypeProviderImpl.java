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
package org.apache.felix.dm.impl.metatype;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.Dictionary;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Locale;
import java.util.Properties;
import java.util.StringTokenizer;
import java.util.TreeSet;

import org.apache.felix.dm.Logger;
import org.apache.felix.dm.PropertyMetaData;
import org.osgi.framework.BundleContext;
import org.osgi.framework.Constants;
import org.osgi.service.cm.ConfigurationException;
import org.osgi.service.cm.ManagedService;
import org.osgi.service.cm.ManagedServiceFactory;
import org.osgi.service.log.LogService;
import org.osgi.service.metatype.MetaTypeProvider;
import org.osgi.service.metatype.ObjectClassDefinition;

/**
 * When a ConfigurationDepdendency is configured with properties metadata, we provide
 * a specific ManagedService which also implements the MetaTypeProvider interface. This interface
 * allows the MetaTypeService to retrieve our properties metadata, which will then be handled by webconsole.
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
public class MetaTypeProviderImpl implements MetaTypeProvider, ManagedService, ManagedServiceFactory {
    private ManagedService m_managedServiceDelegate;
    private ManagedServiceFactory m_managedServiceFactoryDelegate;
    private ArrayList<PropertyMetaData> m_propertiesMetaData = new ArrayList<>();
    private String m_description;
    private String m_heading;
    private String m_localization;
    private HashMap<String, Properties> m_localesProperties = new HashMap<>();
    private Logger m_logger;
    private BundleContext m_bctx;
    private String m_pid;

    public MetaTypeProviderImpl(String pid, BundleContext ctx, Logger logger, ManagedService msDelegate, ManagedServiceFactory msfDelegate) {
        m_pid = pid;
        m_bctx = ctx;
        m_logger = logger;
        m_managedServiceDelegate = msDelegate;
        m_managedServiceFactoryDelegate = msfDelegate;
        // Set the default localization file base name (see core specification, in section Localization on page 68).
        // By default, this file can be stored in OSGI-INF/l10n/bundle.properties (and corresponding localized version
        // in OSGI-INF/l10n/bundle_en_GB_welsh.properties,  OSGI-INF/l10n/bundle_en_GB.properties, etc ...
        // This default localization property file name can be overriden using the PropertyMetaData.setLocalization method.
        m_localization = (String) m_bctx.getBundle().getHeaders().get(Constants.BUNDLE_LOCALIZATION);
        if (m_localization == null) {
            m_localization = Constants.BUNDLE_LOCALIZATION_DEFAULT_BASENAME;
        }
    }
    
    @SuppressWarnings("unchecked")
    public MetaTypeProviderImpl(MetaTypeProviderImpl prototype, ManagedService msDelegate, ManagedServiceFactory msfDelegate) {
        m_pid = prototype.m_pid;
        m_bctx = prototype.m_bctx;
        m_logger = prototype.m_logger;
        m_localization = prototype.m_localization;
        m_propertiesMetaData = prototype.m_propertiesMetaData != null ? (ArrayList<PropertyMetaData>) prototype.m_propertiesMetaData.clone() : null;
        m_description = prototype.m_description;
        m_heading = prototype.m_heading;
        m_localization = prototype.m_localization;
        m_localesProperties = prototype.m_localesProperties != null ? (HashMap<String, Properties>) prototype.m_localesProperties.clone() : null;
        m_managedServiceDelegate = msDelegate;
        m_managedServiceFactoryDelegate = msfDelegate;
    }


    /**
     * Registers the metatype information of a given configuration property
     * @param property
     */
    public void add(PropertyMetaData property) {
        m_propertiesMetaData.add(property);
    }

    /**
     * A human readable description of the PID this annotation is associated with. Example: "Configuration for the PrinterService bundle".
     * @return A human readable description of the PID this annotation is associated with (may be localized)
     */
    public void setDescription(String description) {
        m_description = description;
    }

    /**
     * The label used to display the tab name (or section) where the properties are displayed. Example: "Printer Service".
     * @return The label used to display the tab name where the properties are displayed (may be localized)
     */
    public void setName(String heading) {
        m_heading = heading;
    }

    /**
     * Points to the basename of the Properties file that can localize the Meta Type informations.
     * By default, (e.g. <code>setLocalization("person")</code> would match person_du_NL.properties in the root bundle directory.
     * The default localization base name for the properties is OSGI-INF/l10n/bundle, but can
     * be overridden by the manifest Bundle-Localization header (see core specification, in section Localization on page 68).
     */
    public void setLocalization(String path) {
        if (path.endsWith(".properties")) {
            throw new IllegalArgumentException(
                "path must point to the base name of the propertie file, "
                + "excluding local suffixes. For example: "
                + "foo/bar/person is valid and matches the property file \"foo/bar/person_bundle_en_GB_welsh.properties\"");
        }
        m_localization = path.startsWith("/") ? path.substring(1) : path;
    }

    // --------------- MetaTypeProvider interface -------------------------------------------------

    /**
     * Returns all the Locales our bundle is containing. For instance, if our bundle contains the following localization files:
     * OSGI-INF/l10n/bundle_en_GB_welsh.properties and OSGI-INF/l10n/bundle_en_GB.properties, then this method will return
     * "en_GB", "en_GB_welsh" ...
     * @return the list of Locale supported by our bundle.
     */
    @SuppressWarnings("rawtypes")
    public String[] getLocales() {
        int lastSlash = m_localization.lastIndexOf("/");
        String path = (lastSlash == -1) ? "/" : ("/" + m_localization.substring(0, lastSlash - 1));
        String base = (lastSlash == -1) ? m_localization : m_localization.substring(lastSlash + 1);
        Enumeration e = m_bctx.getBundle().findEntries(path,
            base + "*.properties", false);
        if (e == null) {
            return null;
        }
        
        TreeSet<String> set = new TreeSet<>();
        while (e.hasMoreElements()) {
            // We have found a locale property file in the form of "path/file[_language[_ country[_variation]].properties"
            // And now, we have to get the "language[_country[_variation]]" part ...
            URL url = (URL) e.nextElement();
            String name = url.getPath();
            name = name.substring(name.lastIndexOf("/") + 1);
            int underscore = name.indexOf("_");
            if (underscore != -1) {
                name = name.substring(underscore + 1, name.length() - ".properties".length());
            }
            if (name.length() > 0) {
                set.add(name);
            }
        }

        String[] locales = (String[]) set.toArray(new String[set.size()]);
        return locales.length == 0 ? null : locales;
    }

    /**
     * Returns the ObjectClassDefinition for a given Pid/Locale.
     */
    public ObjectClassDefinition getObjectClassDefinition(String id, String locale) {
        try {
            // Check if the id matches our PID
            if (!id.equals(m_pid)) {
                m_logger.log(LogService.LOG_ERROR, "id " + id + " does not match pid " + m_pid);
                return null;
            }

            Properties localeProperties = getLocaleProperties(locale);
            return new ObjectClassDefinitionImpl(m_pid, m_heading,
                m_description, m_propertiesMetaData, new Resource(localeProperties));
        }

        catch (Throwable t) {
            m_logger.log(
                Logger.LOG_ERROR,
                "Unexpected exception while geting ObjectClassDefinition for " + id + " (locale="
                    + locale + ")", t);
            return null;
        }
    }

    /**
     * We also implements the ManagedService and we just delegates the configuration handling to
     * our associated ConfigurationDependency.
     */
    public void updated(Dictionary<String, ?> properties) throws ConfigurationException {
        m_managedServiceDelegate.updated(properties);
    }

    /**
     * Gets the properties for a given Locale.
     * @param locale
     * @return
     * @throws IOException
     */
    private synchronized Properties getLocaleProperties(String locale) throws IOException {
        locale = locale == null ? Locale.getDefault().toString() : locale;
        Properties properties = (Properties) m_localesProperties.get(locale);
        if (properties == null) {
            properties = new Properties();
            URL url = m_bctx.getBundle().getEntry(m_localization + ".properties");
            if (url != null) {
                loadLocale(properties, url);
            }

            String path = m_localization;
            StringTokenizer tok = new StringTokenizer(locale, "_");
            while (tok.hasMoreTokens()) {
                path += "_" + tok.nextToken();
                url = m_bctx.getBundle().getEntry(path + ".properties");
                if (url != null) {
                    properties = new Properties(properties);
                    loadLocale(properties, url);
                }
            }
            m_localesProperties.put(locale, properties);
        }
        return properties;
    }

    /**
     * Loads a Locale Properties file.
     * @param properties
     * @param url
     * @throws IOException
     */
    private void loadLocale(Properties properties, URL url) throws IOException {
        InputStream in = null;
        try {
            in = url.openStream();
            properties.load(in);
        }
        finally {
            if (in != null) {
                try {
                    in.close();
                }
                catch (IOException ignored) {
                }
            }
        }
    }

    // ManagedServiceFactory implementation
    public void deleted(String pid) {
        m_managedServiceFactoryDelegate.deleted(pid);
    }

    public String getName() {
        return m_pid;
    }

    public void updated(String pid, Dictionary<String, ?> properties) throws ConfigurationException {
        m_managedServiceFactoryDelegate.updated(pid, properties);
    }
}
