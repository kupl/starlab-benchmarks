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
package org.apache.sling.jcr.resource.internal.helper.jcr;

import static org.apache.sling.api.resource.ResourceResolverFactory.NEW_PASSWORD;

import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.atomic.AtomicReference;

import javax.jcr.Credentials;
import javax.jcr.RepositoryException;
import javax.jcr.Session;
import javax.jcr.SimpleCredentials;

import org.apache.sling.api.resource.LoginException;
import org.apache.sling.api.resource.ResourceResolver;
import org.apache.sling.api.resource.ResourceResolverFactory;
import org.apache.sling.api.resource.external.URIProvider;
import org.apache.sling.commons.classloader.DynamicClassLoaderManager;
import org.apache.sling.jcr.api.SlingRepository;
import org.apache.sling.jcr.resource.api.JcrResourceConstants;
import org.apache.sling.jcr.resource.internal.HelperData;
import org.apache.sling.spi.resource.provider.ResourceProvider;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import org.osgi.framework.Bundle;
import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceReference;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class JcrProviderStateFactory {

    private final Logger logger = LoggerFactory.getLogger(JcrProviderStateFactory.class);

    private final ServiceReference<SlingRepository> repositoryReference;

    private final SlingRepository repository;

    private final AtomicReference<DynamicClassLoaderManager> dynamicClassLoaderManagerReference;
    private final AtomicReference<URIProvider[]> uriProviderReference;

    public JcrProviderStateFactory(final ServiceReference<SlingRepository> repositoryReference,
            final SlingRepository repository,
            final AtomicReference<DynamicClassLoaderManager> dynamicClassLoaderManagerReference,
                                   final AtomicReference<URIProvider[]> uriProviderReference) {
        this.repository = repository;
        this.repositoryReference = repositoryReference;
        this.dynamicClassLoaderManagerReference = dynamicClassLoaderManagerReference;
        this.uriProviderReference = uriProviderReference;
    }

    /** Get the calling Bundle from auth info, fail if not provided
     *  @throws LoginException if no calling bundle info provided
     */
    @Nullable
    private Bundle extractCallingBundle(@NotNull Map<String, Object> authenticationInfo) throws LoginException {
        final Object obj = authenticationInfo.get(ResourceProvider.AUTH_SERVICE_BUNDLE);
        if(obj != null && !(obj instanceof Bundle)) {
            throw new LoginException("Invalid calling bundle object in authentication info");
        }
        return (Bundle)obj;
    }

    @SuppressWarnings("deprecation")
org.apache.sling.jcr.resource.internal.helper.jcr.JcrProviderState createProviderState(@org.jetbrains.annotations.NotNull
final java.util.Map<java.lang.String, java.lang.Object> authenticationInfo) throws org.apache.sling.api.resource.LoginException {
    boolean isLoginAdministrative = java.lang.Boolean.TRUE.equals(authenticationInfo.get(org.apache.sling.spi.resource.provider.ResourceProvider.AUTH_ADMIN));
    // check whether a session is provided in the authenticationInfo
    javax.jcr.Session session = org.apache.sling.jcr.resource.internal.helper.jcr.JcrProviderStateFactory.getSession(authenticationInfo);
    if ((session != null) && (!isLoginAdministrative)) {
        // by default any session used by the resource resolver returned is
        // closed when the resource resolver is closed, except when the session
        // was provided in the authenticationInfo
        return createJcrProviderState(session, false, authenticationInfo, null);
    }
    org.osgi.framework.BundleContext bc = null;
    try {
        final org.osgi.framework.Bundle bundle = extractCallingBundle(authenticationInfo);
        {
            bc = /* NPEX_NULL_EXP */
            bundle.getBundleContext();
            final org.apache.sling.jcr.api.SlingRepository repo = bc.getService(repositoryReference);
            if (repo == null) {
                logger.warn("Cannot login {} because cannot get SlingRepository on behalf of bundle {} ({})", isLoginAdministrative ? "admin" : "service", bundle.getSymbolicName(), bundle.getBundleId());
                throw new org.apache.sling.api.resource.LoginException("Repository unavailable");
            }
            try {
                if (isLoginAdministrative) {
                    session = repo.loginAdministrative(null);
                } else {
                    final java.lang.Object subService = authenticationInfo.get(org.apache.sling.api.resource.ResourceResolverFactory.SUBSERVICE);
                    final java.lang.String subServiceName = (subService instanceof java.lang.String) ? ((java.lang.String) (subService)) : null;
                    session = repo.loginService(subServiceName, null);
                }
            } catch (java.lang.Throwable t) {
                // unget the repository if the service cannot
                // login to it, otherwise the repository service
                // is let go off when the resource resolver is
                // closed and the session logged out
                if (session == null) {
                    bc.ungetService(repositoryReference);
                }
                throw t;
            }
        }
    } catch (final javax.jcr.RepositoryException re) {
        throw org.apache.sling.jcr.resource.internal.helper.jcr.JcrProviderStateFactory.getLoginException(re);
    }
    return createJcrProviderState(session, true, authenticationInfo, bc);
}

    private JcrProviderState createJcrProviderState(
            @NotNull final Session session,
            final boolean logoutSession,
            @NotNull final Map<String, Object> authenticationInfo,
            @Nullable final BundleContext ctx
    ) throws LoginException {
        boolean explicitSessionUsed = (getSession(authenticationInfo) != null);
        final Session impersonatedSession = handleImpersonation(session, authenticationInfo, logoutSession, explicitSessionUsed);
        if (impersonatedSession != session && explicitSessionUsed) {
            // update the session in the auth info map in case the resolver gets cloned in the future
            authenticationInfo.put(JcrResourceConstants.AUTHENTICATION_INFO_SESSION, impersonatedSession);
        }
        // if we're actually impersonating, we're responsible for closing the session we've created, regardless
        // of what the original logoutSession value was.
        boolean doLogoutSession = logoutSession || (impersonatedSession != session);
        final HelperData data = new HelperData(this.dynamicClassLoaderManagerReference, this.uriProviderReference);
        return new JcrProviderState(impersonatedSession, data, doLogoutSession, ctx, ctx == null ? null : repositoryReference);
    }

    /**
     * Handle the sudo if configured. If the authentication info does not
     * contain a sudo info, this method simply returns the passed in session. If
     * a sudo user info is available, the session is tried to be impersonated.
     * The new impersonated session is returned. The original session is closed.
     * The session is also closed if the impersonation fails.
     *
     * @param session
     *            The session.
     * @param authenticationInfo
     *            The optional authentication info.
     * @param logoutSession
     *            whether to logout the <code>session</code> after impersonation
     *            or not.
     * @param explicitSessionUsed
     *            whether the JCR session was explicitly given in the auth info or not.
     * @return The original session or impersonated session.
     * @throws LoginException
     *             If something goes wrong.
     */
    private static Session handleImpersonation(final Session session, final Map<String, Object> authenticationInfo,
            final boolean logoutSession, boolean explicitSessionUsed) throws LoginException {
        final String sudoUser = getSudoUser(authenticationInfo);
        // Do we need session.impersonate() because we are asked to impersonate another user?
        boolean needsSudo = (sudoUser != null) && !session.getUserID().equals(sudoUser);
        // Do we need session.impersonate() to get an independent copy of the session we were given in the auth info?
        boolean needsCloning = !needsSudo && explicitSessionUsed && authenticationInfo.containsKey(ResourceProvider.AUTH_CLONE);
        if (!needsSudo && !needsCloning) {
            // Nothing to do, but we need to make sure not to enter the try-finally below because it could close the session.
            return session;
        }
        try {
            if (needsSudo) {
                SimpleCredentials creds = new SimpleCredentials(sudoUser, new char[0]);
                copyAttributes(creds, authenticationInfo);
                creds.setAttribute(ResourceResolver.USER_IMPERSONATOR, session.getUserID());
                return session.impersonate(creds);
            } else {
                assert needsCloning;
                SimpleCredentials creds = new SimpleCredentials(session.getUserID(), new char[0]);
                copyAttributes(creds, authenticationInfo);
                return session.impersonate(creds);
            }
        } catch (final RepositoryException re) {
            throw getLoginException(re);
        } finally {
            if (logoutSession) {
                session.logout();
            }
        }
    }

    /**
     * Create a login exception from a repository exception. If the repository
     * exception is a {@link javax.jcr.LoginException} a {@link LoginException}
     * is created with the same information. Otherwise a {@link LoginException}
     * is created which wraps the repository exception.
     *
     * @param re
     *            The repository exception.
     * @return The login exception.
     */
    private static LoginException getLoginException(final RepositoryException re) {
        if (re instanceof javax.jcr.LoginException) {
            return new LoginException(re.getMessage(), re.getCause());
        }
        return new LoginException("Unable to login " + re.getMessage(), re);
    }

    /**
     * Create a credentials object from the provided authentication info. If no
     * map is provided, <code>null</code> is returned. If a map is provided and
     * contains a credentials object, this object is returned. If a map is
     * provided but does not contain a credentials object nor a user,
     * <code>null</code> is returned. if a map is provided with a user name but
     * without a credentials object a new credentials object is created and all
     * values from the authentication info are added as attributes.
     *
     * @param authenticationInfo
     *            Optional authentication info
     * @return A credentials object or <code>null</code>
     */
    private static Credentials getCredentials(final Map<String, Object> authenticationInfo) {

        Credentials creds = null;
        if (authenticationInfo != null) {

            final Object credentialsObject = authenticationInfo
                    .get(JcrResourceConstants.AUTHENTICATION_INFO_CREDENTIALS);

            if (credentialsObject instanceof Credentials) {
                creds = (Credentials) credentialsObject;
            } else {
                // otherwise try to create SimpleCredentials if the userId is
                // set
                final Object userId = authenticationInfo.get(ResourceResolverFactory.USER);
                if (userId instanceof String) {
                    final Object password = authenticationInfo.get(ResourceResolverFactory.PASSWORD);
                    final SimpleCredentials credentials = new SimpleCredentials((String) userId,
                            ((password instanceof char[]) ? (char[]) password : new char[0]));

                    // add attributes
                    copyAttributes(credentials, authenticationInfo);

                    creds = credentials;
                }
            }
        }

        if (creds instanceof SimpleCredentials && authenticationInfo.get(NEW_PASSWORD) instanceof String) {
            ((SimpleCredentials) creds).setAttribute(NEW_PASSWORD, authenticationInfo.get(NEW_PASSWORD));
        }

        return creds;
    }

    /**
     * Copies the contents of the source map as attributes into the target
     * <code>SimpleCredentials</code> object with the exception of the
     * <code>user.jcr.credentials</code> and <code>user.password</code>
     * attributes to prevent leaking passwords into the JCR Session attributes
     * which might be used for break-in attempts.
     *
     * @param target
     *            The <code>SimpleCredentials</code> object whose attributes are
     *            to be augmented.
     * @param source
     *            The map whose entries (except the ones listed above) are
     *            copied as credentials attributes.
     */
    private static void copyAttributes(final SimpleCredentials target, final Map<String, Object> source) {
        final Iterator<Map.Entry<String, Object>> i = source.entrySet().iterator();
        while (i.hasNext()) {
            final Map.Entry<String, Object> current = i.next();
            if (isAttributeVisible(current.getKey())) {
                target.setAttribute(current.getKey(), current.getValue());
            }
        }
    }

    /**
     * Returns <code>true</code> unless the name is
     * <code>user.jcr.credentials</code> (
     * {@link JcrResourceConstants#AUTHENTICATION_INFO_CREDENTIALS}) or contains
     * the string <code>password</code> as in <code>user.password</code> (
     * {@link org.apache.sling.api.resource.ResourceResolverFactory#PASSWORD})
     *
     * @param name
     *            The name to check whether it is visible or not
     * @return <code>true</code> if the name is assumed visible
     * @throws NullPointerException
     *             if <code>name</code> is <code>null</code>
     */
    private static boolean isAttributeVisible(final String name) {
        return !name.equals(JcrResourceConstants.AUTHENTICATION_INFO_CREDENTIALS) && !name.contains("password");
    }

    /**
     * Return the sudo user information. If the sudo user info is provided, it
     * is returned, otherwise <code>null</code> is returned.
     *
     * @param authenticationInfo
     *            Authentication info (not {@code null}).
     * @return The configured sudo user information or <code>null</code>
     */
    private static String getSudoUser(final Map<String, Object> authenticationInfo) {
        final Object sudoObject = authenticationInfo.get(ResourceResolverFactory.USER_IMPERSONATION);
        if (sudoObject instanceof String) {
            return (String) sudoObject;
        }
        return null;
    }

    /**
     * Returns the session provided as the user.jcr.session property of the
     * <code>authenticationInfo</code> map or <code>null</code> if the property
     * is not contained in the map or is not a <code>javax.jcr.Session</code>.
     *
     * @param authenticationInfo
     *            Authentication info (not {@code null}).
     * @return The user.jcr.session property or <code>null</code>
     */
    private static Session getSession(final Map<String, Object> authenticationInfo) {
        final Object sessionObject = authenticationInfo.get(JcrResourceConstants.AUTHENTICATION_INFO_SESSION);
        if (sessionObject instanceof Session) {
            return (Session) sessionObject;
        }
        return null;
    }

}
