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
package org.apache.felix.dm.annotation.api;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Annotates a method or a field for injecting a Service Dependency. When applied on a class 
 * field, optional unavailable dependencies are injected with a NullObject.
 * 
 * <p> For "add", "change", "remove" callbacks, the following method signatures are supported:
 * 
 * <pre>{@code
 * (Component comp, ServiceReference ref, Service service)
 * (Component comp, ServiceReference ref, Object service)
 * (Component comp, ServiceReference ref)
 * (Component comp, Service service)
 * (Component comp, Object service)
 * (Component comp)
 * (Component comp, Map properties, Service service)
 * (ServiceReference ref, Service service)
 * (ServiceReference ref, Object service)
 * (ServiceReference ref)
 * (Service service)
 * (Service service, Map propeerties)
 * (Map properties, Service, service)
 * (Service service, Dictionary properties)
 * (Dictionary properties, Service service)
 * (Object service)
 * }</pre>
 * 
 * <p> For "swap" callbacks, the following method signatures are supported:
 * 
 * <pre>{@code
 * (Service old, Service replace)
 * (Object old, Object replace)
 * (ServiceReference old, Service old, ServiceReference replace, Service replace)
 * (ServiceReference old, Object old, ServiceReference replace, Object replace)
 * (Component comp, Service old, Service replace)
 * (Component comp, Object old, Object replace)
 * (Component comp, ServiceReference old, Service old, ServiceReference replace, Service replace)
 * (Component comp, ServiceReference old, Object old, ServiceReference replace, Object replace)
 * (ServiceReference old, ServiceReference replace)
 * (Component comp, ServiceReference old, ServiceReference replace)
 * }</pre>
 * 
 * <h3>Usage Examples</h3>
 * Here, the MyComponent component is injected with a dependency over a "MyDependency" service
 * 
 * <blockquote><pre>
 * &#64;Component
 * class MyComponent {
 *     &#64;ServiceDependency(timeout=15000)
 *     MyDependency dependency;
 * </pre></blockquote>
 * 
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@Retention(RetentionPolicy.CLASS)
@Target({ElementType.METHOD, ElementType.FIELD})
public @interface ServiceDependency
{
	/**
	 * Marker interface used to match any service types. When you set the {@link ServiceDependency#service} attribute to this class,
	 * it means that the dependency will return any services (matching the {@link ServiceDependency#filter()} attribute if it is specified).
	 */
	public interface Any { }
	
    /**
     * The type if the service this dependency is applying on. By default, the method parameter 
     * (or the class field) is used as the type. If you want to match all available services, you can set
     * this attribute to the {@link Any} class. In this case, all services (matching the {@link ServiceDependency#filter()} attribute if it is specified) will
     * be returned.
     * @return the service dependency
     */
    Class<?> service() default Object.class;

    /**
     * The Service dependency OSGi filter.
     * @return the service filter
     */
    String filter() default "";

    /**
     * The class for the default implementation, if the dependency is not available.
     * @return the default implementation class
     */
    Class<?> defaultImpl() default Object.class;

    /**
     * Whether the Service dependency is required or not.
     * @return the required flag
     */
    boolean required() default true;

    /**
     * The callback method to be invoked when the service is available. This attribute is only meaningful when 
     * the annotation is applied on a class field.
     * 
     * @return the add callback
     */
    String added() default "";

    /**
     * The callback method to be invoked when the service properties have changed.
     * @return the change callback
     */
    String changed() default "";

    /**
     * The callback method to invoke when the service is lost.
     * @return the remove callback
     */
    String removed() default "";
    
    /**
     * the method to call when the service was swapped due to addition or removal of an aspect
     * @return the swap callback
     */
    String swap() default "";

    /** 
     * The max time in millis to wait for the dependency availability. 
     * Specifying a positive number allow to block the caller thread between service updates. Only
     * useful for required stateless dependencies that can be replaced transparently.
     * A Dynamic Proxy is used to wrap the actual service dependency (which must be an interface). 
     * When the dependency goes away, an attempt is made to replace it with another one which satisfies 
     * the service dependency criteria. If no service replacement is available, then any method invocation 
     * (through the dynamic proxy) will block during a configurable timeout. On timeout, an unchecked 
     * <code>IllegalStateException</code> exception is raised (but the service is not deactivated).<p>
     * Notice that the changed/removed callbacks are not used when the timeout parameter is greater than -1. 
     * 
     * -1 means no timeout at all (default). 0 means that invocation on a missing service will fail 
     * immediately. A positive number represents the max timeout in millis to wait for the service availability.
     * 
     * Sample Code:
     * <blockquote><pre>
     * &#64;Component
     * class MyServer implements Runnable {
     *   &#64;ServiceDependency(timeout=15000)
     *   MyDependency dependency;.
     *   
     *   &#64;Start
     *   void start() {
     *     (new Thread(this)).start();
     *   }
     *   
     *   public void run() {
     *     try {
     *       dependency.doWork();
     *     } catch (IllegalStateException e) {
     *       t.printStackTrace();
     *     }
     *   }   
     * </pre></blockquote>
     * @return the wait time when the dependency is unavailable
     */
    long timeout() default -1;
    
    /**
     * The name used when dynamically configuring this dependency from the init method.
     * Specifying this attribute allows to dynamically configure the dependency 
     * <code>filter</code> and <code>required</code> flag from the Service's init method.
     * All unnamed dependencies will be injected before the init() method; so from the init() method, you can
     * then pick up whatever information needed from already injected (unnamed) dependencies, and configure dynamically
     * your named dependencies, which will then be calculated once the init() method returns.
     * 
     * <p> See {@link Init} annotation for an example usage of a dependency dynamically configured from the init method.
     * @return the dependency name used to dynamically configure the filter and required flag from the init callback.
     */
    String name() default "";
    
    /**
     * Returns true if the dependency service properties must be published along with the service. 
     * Any additional service properties specified directly are merged with these.
     * @return true if dependency service properties must be published along with the service, false if not.
     */
    boolean propagate() default false;
    
    /**
     * Configures whether or not this dependency should internally obtain the service object for all tracked service references.
     * 
     * By default, DM internally dereferences all discovered service references (using 
     * <code>BundleContext.getService(ServiceReference ref)</code> methods. 
     * However, sometimes, your callback only needs the ServiceReference, and sometimes you don't want to dereference the service.
     * So, in this case you can use the <code>dereference(false)</code> method in order to tell to DM 
     * that it should never internally dereference the service dependency internally.
     * 
     * @return false if the service must never be dereferenced by dependency manager (internally).
     */
    boolean dereference() default true;
}
