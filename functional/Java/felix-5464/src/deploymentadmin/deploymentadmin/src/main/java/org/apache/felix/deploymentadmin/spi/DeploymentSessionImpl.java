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
package org.apache.felix.deploymentadmin.spi;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;

import org.apache.felix.deploymentadmin.AbstractDeploymentPackage;
import org.apache.felix.deploymentadmin.Constants;
import org.apache.felix.deploymentadmin.DeploymentAdminConfig;
import org.apache.felix.deploymentadmin.DeploymentAdminImpl;
import org.osgi.framework.Bundle;
import org.osgi.framework.BundleContext;
import org.osgi.service.deploymentadmin.DeploymentException;
import org.osgi.service.deploymentadmin.DeploymentPackage;
import org.osgi.service.deploymentadmin.spi.DeploymentSession;
import org.osgi.service.log.LogService;
import org.osgi.service.packageadmin.PackageAdmin;

/**
 * Represents a running deployment session.
 */
public class DeploymentSessionImpl implements DeploymentSession, Constants {

    private final AbstractDeploymentPackage m_target;
    private final AbstractDeploymentPackage m_source;
    private final List m_commands;
    private final DeploymentAdminImpl m_admin;
    private final DeploymentAdminConfig m_config;

    private volatile Command m_currentCommand = null;
    private volatile boolean m_cancelled;

    public DeploymentSessionImpl(AbstractDeploymentPackage source, AbstractDeploymentPackage target, List commands, DeploymentAdminImpl admin, DeploymentAdminConfig config) {
        m_source = source;
        m_target = target;
        m_commands = commands;
        m_admin = admin;
        m_config = config;
    }

    /**
     * Calling this method will cause the commands specified for this session to
     * be executed. the commands will be rolled back if the session is canceled
     * or if an exception is caused by one of the commands.
     * 
     * @throws DeploymentException If the session was canceled (
     *             <code>DeploymentException.CODE_CANCELLED</code>) or if one of the
     *             commands caused an exception (<code>DeploymentException.*</code>)
     */
    public void call(boolean ignoreExceptions) throws DeploymentException {
        List executedCommands = new ArrayList();
        for (Iterator i = m_commands.iterator(); i.hasNext();) {
            if (m_cancelled) {
                // previous command did not pick up on cancel
                rollback(executedCommands);
                throw new DeploymentException(CODE_CANCELLED);
            }
            m_currentCommand = (Command) i.next();
            try {
                executedCommands.add(m_currentCommand);
                m_currentCommand.execute(this);
            }
            catch (DeploymentException de) {
                if (!ignoreExceptions) {
                    // XXX catch exception and verify whether it is possible to
                    // have exceptions during a rollback
                    rollback(executedCommands);
                    throw de;
                } else {
                    m_admin.getLog().log(LogService.LOG_DEBUG, "Ignoring exception as requested!", de);
                }
            }
        }
        for (Iterator i = m_commands.iterator(); i.hasNext();) {
            ((Command) i.next()).commit(this);
        }
        m_currentCommand = null;
    }

    /**
     * Cancels the session if it is in progress.
     * 
     * @return true if a session was in progress and now canceled, false
     *         otherwise.
     */
    public boolean cancel() {
        m_cancelled = true;

        Command currentCommand = m_currentCommand;
        if (currentCommand != null) {
            currentCommand.cancel();
            return true;
        }

        return false;
    }

    /**
     * Returns the bundle context of the bundle this class is part of.
     * 
     * @return The <code>BundleContext</code>.
     */
    public BundleContext getBundleContext() {
        return m_admin.getBundleContext();
    }

    /**
     * @return the configuration for this session, is guaranteed to remain stable during this session, never
     *         <code>null</code>.
     */
    public DeploymentAdminConfig getConfiguration() {
        return m_config;
    }

    /**
     * Retrieve the base directory of the persistent storage area according to
     * OSGi Core R4 6.1.6.10 for the given <code>BundleContext</code>.
     * 
     * @param bundle of which the storage area will be returned
     * @return a <code>File</code> that represents the base directory of the
     *         persistent storage area for the bundle
     */
    public File getDataFile(Bundle bundle) {
        File result = null;

        BundleContext context = bundle.getBundleContext();
        if (context != null) {
            result = context.getDataFile("");
        }
        else {
            // TODO this method should not return null or throw an exception; we
            // need to resolve this...
            throw new IllegalStateException("Could not retrieve valid bundle context from bundle " + bundle.getSymbolicName());
        }

        if (result == null) {
            throw new IllegalStateException("Could not retrieve base directory for bundle " + bundle.getSymbolicName());
        }
        return result;
    }

    /**
     * Returns the currently present log service.
     * 
     * @return The <code>LogService</code>.
     */
    public LogService getLog() {
        return m_admin.getLog();
    }

    /**
     * Returns the currently present package admin.
     * 
     * @return The <code>PackageAdmin</code>
     */
    public PackageAdmin getPackageAdmin() {
        return m_admin.getPackageAdmin();
    }

    /**
     * Returns the source deployment package as an
     * <code>AbstractDeploymentPackage</code>.
     * 
     * @return The source deployment packge of the session.
     */
    public AbstractDeploymentPackage getSourceAbstractDeploymentPackage() {
        return m_source;
    }

    public DeploymentPackage getSourceDeploymentPackage() {
        return m_source;
    }

    /**
     * Returns the target deployment package as an
     * <code>AbstractDeploymentPackage</code>.
     * 
     * @return The target deployment package of the session.
     */
    public AbstractDeploymentPackage getTargetAbstractDeploymentPackage() {
        return m_target;
    }

    public DeploymentPackage getTargetDeploymentPackage() {
        return m_target;
    }

    private void rollback(List executedCommands) {
        for (ListIterator i = executedCommands.listIterator(executedCommands.size()); i.hasPrevious();) {
            Command command = (Command) i.previous();
            command.rollback(this);
        }
    }
}