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
package org.apache.felix.dm;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.osgi.framework.BundleContext;
import org.osgi.framework.BundleException;
import org.osgi.framework.FrameworkUtil;
import org.osgi.framework.InvalidSyntaxException;
import org.osgi.framework.ServiceEvent;
import org.osgi.framework.ServiceListener;
import org.osgi.framework.ServiceReference;
import org.osgi.service.log.LogService;

/**
 * This class mimics the standard OSGi <tt>LogService</tt> interface. An
 * instance of this class is used by the dependency manager for all logging. 
 * By default this class logs messages to standard out. The log level can be set to
 * control the amount of logging performed, where a higher number results in
 * more logging. A log level of zero turns off logging completely.
 * 
 * The log levels match those specified in the OSGi Log Service.
 * This class also tracks log services and will use the highest ranking 
 * log service, if present, as a back end instead of printing to standard
 * out. The class uses reflection to invoking the log service's method to 
 * avoid a dependency on the log interface, which is also why it does not
 * actually implement <code>LogService</code>. This class is in many ways 
 * similar to the one used in the system bundle for that same purpose.
 *
 * @author <a href="mailto:dev@felix.apache.org">Felix Project Team</a>
 */
@SuppressWarnings({"rawtypes", "unchecked"})
public class Logger implements ServiceListener {
	private static final String LOG_SINGLE_CONTEXT = "org.apache.felix.dependencymanager.singleContextLog";
    public static final int LOG_ERROR = 1;
    public static final int LOG_WARNING = 2;
    public static final int LOG_INFO = 3;
    public static final int LOG_DEBUG = 4;

    private final BundleContext m_context;

    private final static int LOGGER_OBJECT_IDX = 0;
    private final static int LOGGER_METHOD_IDX = 1;
    private static final String ENABLED_LOG_LEVEL = "org.apache.felix.dependencymanager.loglevel";
	private ServiceReference m_logRef = null;
    private Object[] m_logger = null;
    private int m_enabledLevel = LogService.LOG_WARNING;
    private String m_debugKey;

    public Logger(BundleContext context) {
    	if (context != null && "true".equals(context.getProperty(LOG_SINGLE_CONTEXT))) {
    		m_context = FrameworkUtil.getBundle(DependencyManager.class).getBundleContext();
    	} else {
    		m_context = context;
    	}
		if (m_context != null) {
		    String enabledLevel = m_context.getProperty(ENABLED_LOG_LEVEL);
		    if (enabledLevel != null) {
		        try {
		            m_enabledLevel = Integer.valueOf(enabledLevel);
		        } catch (NumberFormatException e) {}
		    }
		    startListeningForLogService();
		}
    }
    
    public final void log(int level, String msg) {
        _log(null, level, msg, null);
    }

    public final void log(int level, String msg, Throwable throwable) {
        _log(null, level, msg, throwable);
    }

    public final void log(ServiceReference sr, int level, String msg) {
        _log(sr, level, msg, null);
    }

    public final void log(ServiceReference sr, int level, String msg, Throwable throwable) {
        _log(sr, level, msg, throwable);
    }

    protected void doLog(ServiceReference sr, int level, String msg, Throwable throwable) {
        String s = (sr == null) ? null : "SvcRef " + sr;
        s = (s == null) ? msg : s + " " + msg;
        s = (throwable == null) ? s : s + " (" + throwable + ")";
        switch (level) {
            case LOG_DEBUG:
                System.out.println("DEBUG: " + s);
                break;
            case LOG_ERROR:
                System.out.println("ERROR: " + s);
                if (throwable != null) {
                    if ((throwable instanceof BundleException) && (((BundleException) throwable).getNestedException() != null)) {
                        throwable = ((BundleException) throwable).getNestedException();
                    }
                    throwable.printStackTrace();
                }
                break;
            case LOG_INFO:
                System.out.println("INFO: " + s);
                break;
            case LOG_WARNING:
                System.out.println("WARNING: " + s);
                break;
            default:
                System.out.println("UNKNOWN[" + level + "]: " + s);
        }
    }

    private void _log(ServiceReference sr, int level, String msg, Throwable throwable) {
        if (level <= m_enabledLevel) {
            StringBuilder sb = new StringBuilder("[");
            if (m_debugKey != null) {
                sb.append(m_debugKey).append(" - ");
            }
            sb.append(Thread.currentThread().getName());
            sb.append("] ");
            sb.append(msg);
            
            // Save our own copy just in case it changes. We could try to do
            // more conservative locking here, but let's be optimistic.
            Object[] logger = m_logger;
            // Use the log service if available.
            if (logger != null) {
                _logReflectively(logger, sr, level, sb.toString(), throwable);
            }
            // Otherwise, default logging action.
            else {
                doLog(sr, level, sb.toString(), throwable);
            }
        }
    }

    private void _logReflectively(Object[] logger, ServiceReference sr, int level, String msg, Throwable throwable) {
        if (logger != null) {
            Object[] params = { sr, new Integer(level), msg, throwable };
            try {
                ((Method) logger[LOGGER_METHOD_IDX]).invoke(logger[LOGGER_OBJECT_IDX], params);
            }
            catch (InvocationTargetException ex) {
                System.err.println("Logger: " + ex);
            }
            catch (IllegalAccessException ex) {
                System.err.println("Logger: " + ex);
            }
        }
    }

    /**
     * This method is called when the bundle context is set;
     * it simply adds a service listener so that the bundle can track
     * log services to be used as the back end of the logging mechanism. It also
     * attempts to get an existing log service, if present, but in general
     * there will never be a log service present since the system bundle is
     * started before every other bundle.
     */
	private synchronized void startListeningForLogService() {
        try {
            // add a service listener for log services, carefully avoiding any code dependency on it
            m_context.addServiceListener(this, "(objectClass=org.osgi.service.log.LogService)");
        }
        catch (InvalidSyntaxException ex) {
            // this will never happen since the filter is hard coded
        }
        // try to get an existing log service
        m_logRef = m_context.getServiceReference("org.osgi.service.log.LogService");
        // get the service object if available and set it in the logger
        if (m_logRef != null) {
            setLogger(m_context.getService(m_logRef));
        }
    }

    /**
     * This method implements the callback for the ServiceListener interface.
     * It is public as a byproduct of implementing the interface and should
     * not be called directly. This method tracks run-time changes to log
     * service availability. If the log service being used by the framework's
     * logging mechanism goes away, then this will try to find an alternative.
     * If a higher ranking log service is registered, then this will switch
     * to the higher ranking log service.
     */
    public final synchronized void serviceChanged(ServiceEvent event) {
        // if no logger is in use, then grab this one
        if ((event.getType() == ServiceEvent.REGISTERED) && (m_logRef == null)) {
            m_logRef = event.getServiceReference();
            // get the service object and set it in the logger
            setLogger(m_context.getService(m_logRef));
        }
        // if a logger is in use, but this one has a higher ranking, then swap
        // it for the existing logger
        else if ((event.getType() == ServiceEvent.REGISTERED) && (m_logRef != null)) {
            ServiceReference ref = m_context.getServiceReference("org.osgi.service.log.LogService");
            if (!ref.equals(m_logRef)) {
                m_context.ungetService(m_logRef);
                m_logRef = ref;
                setLogger(m_context.getService(m_logRef));
            }
        }
        // if the current logger is going away, release it and try to
        // find another one
        else if ((event.getType() == ServiceEvent.UNREGISTERING) && m_logRef != null && m_logRef.equals(event.getServiceReference())) {
            // Unget the service object.
            m_context.ungetService(m_logRef);
            // Try to get an existing log service.
            m_logRef = m_context.getServiceReference("org.osgi.service.log.LogService");
            // get the service object if available and set it in the logger
            if (m_logRef != null) {
                setLogger(m_context.getService(m_logRef));
            }
            else {
                setLogger(null);
            }
        }
    }

    /**
     * This method sets the new log service object. It also caches the method to
     * invoke. The service object and method are stored in array to optimistically
     * eliminate the need to locking when logging.
     */
    private void setLogger(Object logObj) {
        if (logObj == null) {
            m_logger = null;
        }
        else {
            Class<?>[] formalParams = { ServiceReference.class, Integer.TYPE, String.class, Throwable.class };
            try {
                Method logMethod = logObj.getClass().getMethod("log", formalParams);
                logMethod.setAccessible(true);
                m_logger = new Object[] { logObj, logMethod };
            }
            catch (NoSuchMethodException ex) {
                System.err.println("Logger: " + ex);
                m_logger = null;
            }
        }
    }

    public void setEnabledLevel(int enabledLevel) {
        m_enabledLevel = enabledLevel;
    }
    
    public void setDebugKey(String debugKey) {
        m_debugKey = debugKey;
    }
    
    public String getDebugKey() {
        return m_debugKey;
    }

    // --------------- Convenient helper log methods --------------------------------------------
    
    public void err(String format, Object... params) {
    	if (m_enabledLevel >= LOG_ERROR) {
    		log(LogService.LOG_ERROR, String.format(format, params));
    	}
    }

    public void err(String format, Throwable err, Object... params) {
    	if (m_enabledLevel >= LOG_ERROR) {
    		log(LogService.LOG_ERROR, String.format(format, params), err);
    	}
    }

    public void warn(String format, Object... params) {
    	if (m_enabledLevel >= LOG_WARNING) {
    		log(LogService.LOG_WARNING, String.format(format, params));   
    	}
    }

    public void warn(String format, Throwable err, Object... params) {
    	if (m_enabledLevel >= LOG_WARNING) {
    		log(LogService.LOG_WARNING, String.format(format, params), err);       
    	}
    }

    public boolean info() {
        return m_enabledLevel >= LogService.LOG_INFO;
    }

    public void info(String format, Object... params) {
    	if (info()) {
    		log(LogService.LOG_INFO, String.format(format, params));
    	}
    }

    public void info(String format, Throwable err, Object... params) {
    	if (info()) {
    		log(LogService.LOG_INFO, String.format(format, params), err);      
    	} 
    }

    public boolean debug() {
        return m_enabledLevel >= LogService.LOG_DEBUG;
    }

    public void debug(String format, Object... params) {
    	if (debug()) {
    		log(LogService.LOG_DEBUG, String.format(format, params));
    	}
    }

    public void debug(String format, Throwable err, Object... params) {
    	if (debug()) {
    		log(LogService.LOG_DEBUG, String.format(format, params), err);
    	}
    }
}