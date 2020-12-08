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

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.apache.felix.deploymentadmin.AbstractDeploymentPackage;
import org.apache.felix.deploymentadmin.AbstractInfo;
import org.apache.felix.deploymentadmin.ResourceInfoImpl;
import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceReference;
import org.osgi.service.deploymentadmin.DeploymentException;
import org.osgi.service.deploymentadmin.spi.ResourceProcessor;
import org.osgi.service.deploymentadmin.spi.ResourceProcessorException;
import org.osgi.service.log.LogService;

/**
 * Command that processes all the processed resources in the source deployment
 * package of a deployment session by finding their Resource Processors and
 * having those process the resources. System property
 * <code>org.apache.felix.deploymentadmin.allowforeigncustomizers</code> allows
 * you to skip the source handling of resource processors, allowing the use of
 * processors already on the system. Defaults to <code>false</code>.
 */
public class ProcessResourceCommand extends Command {

    private final CommitResourceCommand m_commitCommand;

    /**
     * Creates an instance of this command, the <code>CommitCommand</code> is
     * used to ensure that all used <code>ResourceProcessor</code>s will be
     * committed at a later stage in the deployment session.
     * 
     * @param commitCommand The <code>CommitCommand</code> that will commit all
     *            resource processors used in this command.
     */
    public ProcessResourceCommand(CommitResourceCommand commitCommand) {
        m_commitCommand = commitCommand;
    }

    protected void doExecute(DeploymentSessionImpl session) throws Exception {
        // Allow proper rollback in case the drop fails...
        addRollback(new RollbackCommitAction(session));

        AbstractDeploymentPackage source = session.getSourceAbstractDeploymentPackage();
        BundleContext context = session.getBundleContext();

        Map expectedResources = new HashMap();
        AbstractInfo[] resourceInfos = (AbstractInfo[]) source.getResourceInfos();
        for (int i = 0; i < resourceInfos.length; i++) {
            AbstractInfo resourceInfo = resourceInfos[i];
            if (!resourceInfo.isMissing()) {
                expectedResources.put(resourceInfo.getPath(), resourceInfo);
            }
        }

        try {
            while (!expectedResources.isEmpty()) {
                AbstractInfo jarEntry = source.getNextEntry();
                if (jarEntry == null) {
                    throw new DeploymentException(CODE_OTHER_ERROR, "Expected more resources in the stream: " + expectedResources.keySet());
                }

                String name = jarEntry.getPath();

                ResourceInfoImpl resourceInfo = (ResourceInfoImpl) expectedResources.remove(name);
                if (resourceInfo == null) {
                    throw new DeploymentException(CODE_OTHER_ERROR, "Resource '" + name + "' is not described in the manifest.");
                }
                // FELIX-4491: only resources that need to be processed should be handled...
                if (!resourceInfo.isProcessedResource()) {
                    session.getLog().log(LogService.LOG_INFO, "Ignoring non-processed resource: " + resourceInfo.getPath());
                    continue;
                }

                ServiceReference ref = source.getResourceProcessor(name);
                if (ref == null) {
                    throw new DeploymentException(CODE_PROCESSOR_NOT_FOUND, "No resource processor for resource: '" + name + "'");
                }
                if (!isValidCustomizer(session, ref)) {
                    throw new DeploymentException(CODE_FOREIGN_CUSTOMIZER, "Resource processor for resource '" + name + "' belongs to foreign deployment package");
                }

                ResourceProcessor resourceProcessor = (ResourceProcessor) context.getService(ref);
                if (resourceProcessor == null) {
                    throw new DeploymentException(CODE_PROCESSOR_NOT_FOUND, "No resource processor for resource: '" + name + "'");
                }

                try {
                    if (m_commitCommand.addResourceProcessor(resourceProcessor)) {
                        resourceProcessor.begin(session);
                    }
                    resourceProcessor.process(name, source.getCurrentEntryStream());
                }
                catch (ResourceProcessorException rpe) {
                    if (rpe.getCode() == ResourceProcessorException.CODE_RESOURCE_SHARING_VIOLATION) {
                        throw new DeploymentException(CODE_RESOURCE_SHARING_VIOLATION, "Sharing violation while processing resource '" + name + "'", rpe);
                    }
                    else {
                        throw new DeploymentException(CODE_OTHER_ERROR, "Error while processing resource '" + name + "'", rpe);
                    }
                }
            }
        }
        catch (IOException e) {
            throw new DeploymentException(CODE_OTHER_ERROR, "Problem while reading stream", e);
        }
    }

    private boolean isValidCustomizer(DeploymentSessionImpl session, ServiceReference ref) {
        if (session.getConfiguration().isAllowForeignCustomizers()) {
            // If foreign customizers are allowed, any non-null customizer will do...
            return ref != null;
        }

        AbstractDeploymentPackage source = session.getSourceAbstractDeploymentPackage();
        String serviceOwnerSymName = ref.getBundle().getSymbolicName();
        // If only local customizers are allowed, we must be able to find this customizer in our DP...
        return source.getBundleInfoByName(serviceOwnerSymName) != null;
    }

    private class RollbackCommitAction extends AbstractAction {
        private final DeploymentSessionImpl m_session;

        public RollbackCommitAction(DeploymentSessionImpl session) {
            m_session = session;
        }

        protected void doRun() {
            m_commitCommand.rollback(m_session);
        }
    }
}
