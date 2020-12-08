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
package org.apache.felix.ipojo.webconsole;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.List;
import java.util.Properties;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.felix.ipojo.Factory;
import org.apache.felix.ipojo.HandlerFactory;
import org.apache.felix.ipojo.annotations.Component;
import org.apache.felix.ipojo.annotations.Instantiate;
import org.apache.felix.ipojo.annotations.Provides;
import org.apache.felix.ipojo.annotations.Requires;
import org.apache.felix.ipojo.annotations.ServiceProperty;
import org.apache.felix.ipojo.architecture.Architecture;
import org.apache.felix.ipojo.architecture.HandlerDescription;
import org.apache.felix.ipojo.architecture.InstanceDescription;
import org.apache.felix.ipojo.architecture.PropertyDescription;
import org.apache.felix.ipojo.handlers.dependency.DependencyDescription;
import org.apache.felix.ipojo.handlers.dependency.DependencyHandlerDescription;
import org.apache.felix.ipojo.handlers.providedservice.ProvidedServiceDescription;
import org.apache.felix.ipojo.handlers.providedservice.ProvidedServiceHandlerDescription;
import org.apache.felix.utils.json.JSONWriter;
import org.apache.felix.webconsole.AbstractWebConsolePlugin;
import org.apache.felix.webconsole.DefaultVariableResolver;
import org.apache.felix.webconsole.WebConsoleUtil;
import org.osgi.framework.Constants;
import org.osgi.framework.ServiceReference;

/**
 * iPOJO Web Console plugin.
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@SuppressWarnings("serial")
@Component(immediate=true)
@Provides
@Instantiate
public class IPOJOPlugin extends AbstractWebConsolePlugin {

    /**
     * Used CSS files.
     */
    private static final String CSS[] = { "/res/ui/bundles.css" , "/iPOJO/res/ui/ipojo.css" };

    /**
     * Template : Instance list.
     */
    private final String INSTANCES;

    /**
     * Template : Factory list.
     */
    private final String FACTORIES;

    /**
     * Template : Handler list.
     */
    private final String HANDLERS;

    /**
     * Template : Factory details.
     */
    private final String FACTORY_DETAILS;

    /**
     * Template : Instance details.
     */
    private final String INSTANCE_DETAILS;

    /**
     * Label used by the web console.
     */
    @ServiceProperty(name = "felix.webconsole.label")
    private String m_label = "iPOJO";

    /**
     * Title used by the web console.
     */
    @ServiceProperty(name = "felix.webconsole.title")
    private String m_title = "iPOJO";

    /**
     * CSS files used by the plugin.
     */
    @ServiceProperty(name= "felix.webconsole.css")
    protected String[] m_css = CSS;

    /**
     * List of available Architecture service.
     */
    @Requires(optional = true, specification = Architecture.class)
    private List<Architecture> m_archs;

    /**
     * List of available Factories.
     */
    @Requires(optional = true, specification = Factory.class)
    private List<Factory> m_factories;

    /**
     * List of available Handler Factories.
     */
    @Requires(optional = true, specification = HandlerFactory.class)
    private List<HandlerFactory> m_handlers;

    /**
     * Instantiates the plugin.
     * This method loads all template files.
     */
    public IPOJOPlugin() {
        INSTANCES = readTemplate("/res/instances.html" );
        FACTORIES = readTemplate("/res/factories.html" );
        HANDLERS = readTemplate("/res/handlers.html" );
        FACTORY_DETAILS = readTemplate("/res/factory.html" );
        INSTANCE_DETAILS = readTemplate("/res/instance.html" );
    }

    /**
     * Helper method loading a template file.
     * @param templateFile the template file name
     * @return the template
     */
    private String readTemplate(final String templateFile) {
        InputStream templateStream = getClass().getResourceAsStream(
                templateFile);
        if (templateStream != null) {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            byte[] data = new byte[1024];
            try {
                int len = 0;
                while ((len = templateStream.read(data)) > 0) {
                    baos.write(data, 0, len);
                }
                return baos.toString("UTF-8");
            } catch (IOException e) {
                // don't use new Exception(message, cause) because cause is 1.4+
                throw new RuntimeException("readTemplateFile: Error loading "
                        + templateFile + ": " + e);
            } finally {
                try {
                    templateStream.close();
                } catch (IOException e) {
                    /* ignore */
                }

            }
        }

        // template file does not exist, return an empty string
        log("readTemplateFile: File '" + templateFile
                + "' not found through class " + this.getClass());
        return "";
    }

    /**
     * This methods is called by the web console when the plugin is required.
     * This methods writes the corresponding page (loads template and set variables).
     * @param request the request
     * @param response the response
     * @throws ServletException something bad happened
     * @throws IOException something bad happened
     * @see org.apache.felix.webconsole.AbstractWebConsolePlugin#renderContent(
     *  javax.servlet.http.HttpServletRequest, javax.servlet.http.HttpServletResponse)
     */
    @Override
    protected void renderContent(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        // get request info from request attribute
        final RequestInfo reqInfo = new RequestInfo(request);
        // prepare variables
        DefaultVariableResolver vars = ( ( DefaultVariableResolver ) WebConsoleUtil.getVariableResolver( request ) );

        if (reqInfo.instances) { // Instance
            if (reqInfo.name == null) { // All
                response.getWriter().print( INSTANCES );
            } else { // Specific
                vars.put("name", reqInfo.name); // Inject the name variable.
                response.getWriter().print( INSTANCE_DETAILS );
            }
        } else if (reqInfo.factories) { // Factory
            if (reqInfo.name == null) { // All
                response.getWriter().print( FACTORIES );
            } else { // Specific
                vars.put("name", reqInfo.name); // Inject the name variable.
                response.getWriter().print( FACTORY_DETAILS );
            }
        } else if (reqInfo.handlers) { // Handlers
            response.getWriter().print( HANDLERS );
            // No detailed view for handlers.
        } else {
            // Default
            response.getWriter().print( INSTANCES );
        }
    }

    /**
     * Writes the JSON object containing the info for all instances.
     * @param pw the writer where the json object is printed.
     * @throws IOException the JSON object cannot be written
     */
    private void getAllInstances(JSONWriter pw) throws IOException {
        pw.object();
        // Statline:
        pw.key("count");
        pw.value( m_archs.size());
        pw.key("valid_count");
        pw.value( StateUtils.getValidInstancesCount(m_archs));
        pw.key("invalid_count");
        pw.value( StateUtils.getInvalidInstancesCount(m_archs));
        // End statline

        pw.key("data");
        pw.array();
        for (Architecture arch : m_archs) {
            pw.object();
            pw.key("name");
            pw.value(arch.getInstanceDescription().getName());
            pw.key("factory");
            pw.value(arch.getInstanceDescription().getComponentDescription().getName());
            pw.key("state");
            pw.value(StateUtils.getInstanceState(arch.getInstanceDescription().getState()));
            pw.endObject();
        }
        pw.endArray();

        pw.endObject();
    }

    /**
     * Writes the JSON object containing the info for all factories.
     * @param pw the writer when the json object is printed
     * @throws IOException the JSON object cannot be written
     */
    private void getAllFactories(JSONWriter pw) throws IOException {
        pw.object();
        // Statline:
        pw.key("count");
        pw.value(m_factories.size());
        pw.key("valid_count");
        pw.value(StateUtils.getValidFactoriesCount(m_factories));
        pw.key("invalid_count");
        pw.value(StateUtils.getInvalidFactoriesCount(m_factories));
        // End statline

        pw.key("data");
        pw.array();
        for (Factory factory : m_factories) {
            String version = factory.getVersion();
            String name = factory.getName();

            String state = StateUtils.getFactoryState(factory.getState());
            String bundle = factory.getBundleContext().getBundle().getSymbolicName()
                + " (" + factory.getBundleContext().getBundle().getBundleId() + ")";
            pw.object();
            pw.key("name");
            pw.value(name);
            if (version != null) {
                pw.key("version");
                pw.value(version);
            }
            pw.key("bundle");
            pw.value(bundle);
            pw.key("state");
            pw.value(state);
            pw.endObject();
        }
        pw.endArray();

        pw.endObject();
    }

    /**
     * Writes the JSON object containing the info for all handlers.
     * @param pw the writer when the json object is printed
     * @throws IOException the JSON object cannot be written
     */
    private void getAllHandlers(JSONWriter pw) throws IOException {
        pw.object();

        // Statline:
        pw.key("count");
        pw.value(m_handlers.size());
        pw.key("valid_count");
        pw.value(StateUtils.getValidHandlersCount(m_handlers));
        pw.key("invalid_count");
        pw.value(StateUtils.getInvalidHandlersCount(m_handlers));
        // End statline

        pw.key("data");
        pw.array();
        for (HandlerFactory factory : m_handlers) {
            String version = factory.getVersion();
            String name = factory.getHandlerName();

            String state = StateUtils.getFactoryState(factory.getState());
            String bundle = factory.getBundleContext().getBundle().getSymbolicName()
                + " (" + factory.getBundleContext().getBundle().getBundleId() + ")";
            pw.object();
            pw.key("name");
            pw.value( name);
            if (version != null) {
                pw.key("version");
                pw.value(version);
            }
            pw.key("bundle");
            pw.value(bundle);
            pw.key("state");
            pw.value(state);
            pw.key("type");
            pw.value(factory.getType());
            if (! factory.getMissingHandlers().isEmpty()) {
                pw.key("missing");
                pw.value(factory.getMissingHandlers().toString());
            }
            pw.endObject();
        }
        pw.endArray();
        pw.endObject();
    }

    /**
     * Writes the JSON object containing details about a specific factory.
     * @param pw the writer
     * @param name the factory name
     * @throws IOException if the json object cannot be written.
     */
    private void getFactoryDetail(JSONWriter pw, String name) throws IOException{
        // Find the factory
        Factory factory = null;
        for (Factory fact : m_factories) {
            if (fact.getName().equals(name)) {
                factory = fact;
            }
        }

        if (factory == null) {
            // This will be used a error message (cannot be interpreted as json)
            pw.value("The factory " + name + " does not exist or is private");
            return;
        }

        pw.object();

        // Statline.
        pw.key("count");
        pw.value(m_factories.size());
        pw.key("valid_count");
        pw.value(StateUtils.getValidFactoriesCount(m_factories));
        pw.key("invalid_count");
        pw.value(StateUtils.getInvalidFactoriesCount(m_factories));
        // End of the statline

        // Factory object
        pw.key("data");
        pw.object();
        pw.key("name");
        pw.value(factory.getName());
        pw.key("state");
        pw.value(StateUtils.getFactoryState(factory.getState()));

        String bundle = factory.getBundleContext().getBundle().getSymbolicName()
        + " (" + factory.getBundleContext().getBundle().getBundleId() + ")";
        pw.key("bundle");
        pw.value(bundle);

        // Provided service specifications
        if (factory.getComponentDescription().getprovidedServiceSpecification().length != 0) {
            pw.key("services");
            pw.value(factory.getComponentDescription().getprovidedServiceSpecification());
        }

        // Properties
        PropertyDescription[] props = factory.getComponentDescription().getProperties();
        if (props != null  && props.length != 0) {
            pw.key("properties");
            pw.array();
            for (int i = 0; i < props.length; i++) {
                pw.object();
                pw.key("name");
                pw.value(props[i].getName());
                pw.key("type");
                pw.value(props[i].getType());
                pw.key("mandatory");
                pw.value(props[i].isMandatory());
                pw.key("immutable");
                pw.value(props[i].isImmutable());
                if (props[i].getValue() != null) {
                    pw.key("value");
                    pw.value(props[i].getValue());
                }
                pw.endObject();
            }
            pw.key("properties");
            pw.array();
            pw.endArray();
        }

        if (! factory.getRequiredHandlers().isEmpty()) {
            pw.key("requiredHandlers");
            pw.value(factory.getRequiredHandlers());
        }

        if (! factory.getMissingHandlers().isEmpty()) {
            pw.key("missingHandlers");
            pw.value(factory.getMissingHandlers());
        }

        List<?> instances = StateUtils.getInstanceList(m_archs, name);
        if (! instances.isEmpty()) {
            pw.key("instances");
            pw.value(instances);
        }

        pw.key("architecture");
        pw.value(factory.getDescription().toString());
        pw.endObject();
        pw.endObject();
    }

    /**
     * Writes the JSON object containing details about a specific instance.
     * @param pw the writer
     * @param name the instance name
     * @throws IOException if the json object cannot be written.
     */
    private void getInstanceDetail(JSONWriter pw, String name) throws IOException {
        // Find the factory
        InstanceDescription instance = null;
        for (Architecture arch : m_archs) {
            if (arch.getInstanceDescription().getName().equals(name)) {
                instance = arch.getInstanceDescription();
            }
        }

        if (instance == null) {
            // This will be used a error message (cannot be interpreted as json)
            pw.value("The instance " + name + " does not exist or " +
            		"does not exposed its architecture");
            return;
        }

        pw.object();

        pw.key("count");
        pw.value(m_factories.size());
        pw.key("valid_count");
        pw.value(StateUtils.getValidFactoriesCount(m_factories));
        pw.key("invalid_count");
        pw.value(StateUtils.getInvalidFactoriesCount(m_factories));

        // instance object
        pw.key("data");
        pw.object();
        pw.key("name");
        pw.value(instance.getName());
        pw.key("state");
        pw.value(StateUtils.getInstanceState(instance.getState()));
        pw.key("factory");
        pw.value(instance.getComponentDescription().getName());

        getProvidedServiceDetail(pw, instance.getHandlerDescription("org.apache.felix.ipojo:provides"));

        getRequiredServiceDetail(pw, instance.getHandlerDescription("org.apache.felix.ipojo:requires"));

        pw.key("architecture");
        pw.value(instance.getDescription().toString());

        pw.endObject();
        pw.endObject();
    }

    /**
     * Endpoint dealing with JSON requests.
     * @param request the request
     * @param response the response
     * @throws ServletException if an error occurred
     * @throws IOException if an error occurred
     * @see org.apache.felix.webconsole.AbstractWebConsolePlugin#doGet
     *  (javax.servlet.http.HttpServletRequest, javax.servlet.http.HttpServletResponse)
     */
    @Override
    protected void doGet(HttpServletRequest request,
            HttpServletResponse response) throws ServletException, IOException {
        final RequestInfo reqInfo = new RequestInfo(request);

        if (reqInfo.extension.equals("json")) {
            response.setContentType("application/json");
            final JSONWriter writer = new JSONWriter(response.getWriter());
            if (reqInfo.instances) {
                if (reqInfo.name == null) {
                    this.getAllInstances(writer);
                    writer.flush();
                    return;
                } else {
                    this.getInstanceDetail(writer, reqInfo.name);
                    writer.flush();
                    return;
                }
            }

            if (reqInfo.factories) {
                if (reqInfo.name == null) {
                    this.getAllFactories(writer);
                    writer.flush();
                    return;
                } else {
                    this.getFactoryDetail(writer, reqInfo.name);
                    writer.flush();
                    return;
                }
            }

            if (reqInfo.handlers) {
                this.getAllHandlers(writer);
            }
            // nothing more to do
            writer.flush();
            return;
        }
        // Otherwise, delegate to super.
        super.doGet(request, response);
    }

    /**
     * Allows loading the 'ui' folder as web resource.
     * @param path the resource path
     * @return the internal resource url.
     */
    public URL getResource(String path) {
        if (path.contains("/res/ui/")) {
            return this.getClass().getResource(
                    path.substring(m_label.length() + 1));
        }
        return null;
    }

    /**
     * Creates the JSON Array describing the provided services.
     * @param hd the provided service handler
     * @return the JSON Array or null if no provided service
     * @throws JSONException if the array cannot be created.
     */
    private void getProvidedServiceDetail(final JSONWriter pw, HandlerDescription hd) throws IOException {
        if (hd == null) {
            return;
        }

        pw.key("services");
        pw.array();
        ProvidedServiceHandlerDescription desc = (ProvidedServiceHandlerDescription) hd;

        for (ProvidedServiceDescription ps : desc.getProvidedServices()) {
            pw.object();
            String spec = Arrays.toString(ps.getServiceSpecifications());
            if (spec.startsWith("[")) {
                spec = spec.substring(1, spec.length() - 1);
            }
            pw.key("specification");
            pw.value(spec);
            pw.key("state");
            pw.value(StateUtils.getProvidedServiceState(ps.getState()));

            if (ps.getServiceReference() != null) {
                pw.key("id");
                pw.value(ps.getServiceReference().getProperty(Constants.SERVICE_ID));
            }

            if (ps.getProperties() != null  && !ps.getProperties().isEmpty()) {
                pw.key("properties");
                getServiceProperties(pw, ps.getProperties());
            }

            pw.endObject();
        }
        pw.endArray();
    }

    /**
     * Builds the JSON Array containing object representing the given properties
     * (name / value pair).
     * @param properties the properties
     * @return the JSON Array
     * @throws JSONException if the array cannot be created correctly
     */
    private void getServiceProperties(JSONWriter pw, Properties properties) throws IOException {
        pw.array();
        Enumeration<Object> e = properties.keys();
        while (e.hasMoreElements()) {
            String key = (String) e.nextElement();
            Object value = properties.get(key);
            pw.object();
            pw.key("name");
            pw.value(key);
            if (value != null  && value.getClass().isArray()) {
                // TODO Test with primitive types
                pw.key("value");
                pw.value(Arrays.toString((Object[]) value));
            } else if (value != null) {
                pw.key("value");
                pw.value(value.toString());
            } else {
                pw.key("value");
                pw.value("no value");
            }
            pw.endObject();
        }
        pw.endArray();
    }

    /**
     * Builds the JSON Array representing the required services.
     * @param hd the dependency handler
     * @return the array containing JSON object representing service
     * dependencies, or null if there is no service dependency.
     * @throws JSONException if the JSON array cannot be created.
     */
    private void getRequiredServiceDetail(JSONWriter pw,
            HandlerDescription hd) throws IOException {
        if (hd == null) {
            return;
        }
        pw.key("reqs");
        pw.array();
        DependencyHandlerDescription desc = (DependencyHandlerDescription) hd;
        for (DependencyDescription dep : desc.getDependencies()) {
            pw.object();
            pw.key("specification");
            pw.value(dep.getSpecification());
            pw.key("id");
            pw.value(dep.getId());
            pw.key("state");
            pw.value(StateUtils.getDependencyState(dep.getState()));
            pw.key("policy");
            pw.value(StateUtils.getDependencyBindingPolicy(dep.getPolicy()));
            pw.key("optional");
            pw.value(dep.isOptional());
            pw.key("aggregate");
            pw.value(dep.isMultiple());
            if (dep.getFilter() != null) {
                pw.key("filter");
                pw.value(dep.getFilter());
            }
            if (dep.getServiceReferences() != null  && dep.getServiceReferences().size() != 0) {
                pw.key("matching");
                getServiceReferenceList(pw, dep.getServiceReferences());
            }

            if (dep.getUsedServices() != null  && dep.getUsedServices().size() != 0) {
                pw.key("used");
                getServiceReferenceList(pw, dep.getUsedServices());
            }

            pw.endObject();
        }

        pw.endArray();
    }

    /**
     * Builds the JSON Array representing the given service reference list.
     * The array contains JSON objects. Those object contains the service id (id)
     * as well as the instance name (instance) if the property is set in the service
     * reference.
     * @param refs the service reference list
     * @return the JSON Array
     * @throws JSONException if the array cannot be created.
     */
    private void getServiceReferenceList(JSONWriter pw, List<ServiceReference> refs) throws IOException {
        pw.array();
        if (refs != null) {
            for (ServiceReference ref : refs) {
                pw.object();
                if (ref.getProperty("instance.name") == null) {
                    pw.key("id");
                    pw.value(ref.getProperty(Constants.SERVICE_ID));
                } else {
                    pw.key("id");
                    pw.value(ref.getProperty(Constants.SERVICE_ID));
                    pw.key("instance");
                    pw.value(ref.getProperty("instance.name"));
                }
                pw.endObject();
            }
        }
        pw.endArray();
    }

    /**
     * Gets the plugin label.
     * @return the label.
     * @see org.apache.felix.webconsole.AbstractWebConsolePlugin#getLabel()
     */
    @Override
    public String getLabel() {
       return m_label;
    }

    /**
     * Gets the plugin title.
     * @return the title
     * @see org.apache.felix.webconsole.AbstractWebConsolePlugin#getTitle()
     */
    @Override
    public String getTitle() {
        return m_title;
    }

    /**
     * Get the CSS used by the plugin.
     * @return the list of CSS
     * @see org.apache.felix.webconsole.AbstractWebConsolePlugin#getCssReferences()
     */
    @Override
    protected String[] getCssReferences() {
        return CSS;
    }



    /**
     * Parse request to extract the query.
     */
    private final class RequestInfo {
        /**
         * The extension.
         */
        public final String extension;
        /**
         * The path.
         */
        public final String path;
        /**
         * The instances.
         */
        public final boolean instances;
        /**
         * The factories.
         */
        public final boolean factories;
        /**
         * The handlers.
         */
        public final boolean handlers;

        /**
         * The specific factory or instance name.
         */
        public final String name;


        /**
         * Creates a RequestInfo.
         * @param request the request
         */
        protected RequestInfo( final HttpServletRequest request ) {
            String info = request.getPathInfo();
            // remove label and starting slash
            info = info.substring(getLabel().length() + 1);

            // get extension
            if (info.endsWith(".json")) {
                extension = "json";
                info = info.substring(0, info.length() - 5);
            } else {
                extension = "html";
            }

            if (info.startsWith("/")) {
                path = info.substring(1);

                instances = path.startsWith("instances");
                factories = path.startsWith("factories");
                handlers = path.startsWith("handlers");

                if (instances  && path.startsWith("instances/")) {
                    name = path.substring("instances".length() + 1);
                } else if (factories  && path.startsWith("factories/")) {
                    name = path.substring("factories".length() + 1);
                } else {
                    name = null;
                }
            } else {
                path = null;
                name = null;
                instances = false;
                factories = false;
                handlers = false;
            }

            request.setAttribute(IPOJOPlugin.class.getName(), this);
        }

    }

}
