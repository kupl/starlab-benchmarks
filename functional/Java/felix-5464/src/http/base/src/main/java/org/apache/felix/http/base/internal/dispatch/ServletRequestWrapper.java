/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.felix.http.base.internal.dispatch;

import static javax.servlet.RequestDispatcher.FORWARD_CONTEXT_PATH;
import static javax.servlet.RequestDispatcher.FORWARD_PATH_INFO;
import static javax.servlet.RequestDispatcher.FORWARD_QUERY_STRING;
import static javax.servlet.RequestDispatcher.FORWARD_REQUEST_URI;
import static javax.servlet.RequestDispatcher.FORWARD_SERVLET_PATH;
import static javax.servlet.RequestDispatcher.INCLUDE_CONTEXT_PATH;
import static javax.servlet.RequestDispatcher.INCLUDE_PATH_INFO;
import static javax.servlet.RequestDispatcher.INCLUDE_QUERY_STRING;
import static javax.servlet.RequestDispatcher.INCLUDE_REQUEST_URI;
import static javax.servlet.RequestDispatcher.INCLUDE_SERVLET_PATH;
import static org.apache.felix.http.base.internal.util.UriUtils.concat;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import javax.servlet.AsyncContext;
import javax.servlet.DispatcherType;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletRequestAttributeEvent;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;
import javax.servlet.http.HttpSession;
import javax.servlet.http.Part;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.commons.fileupload.servlet.ServletRequestContext;
import org.apache.felix.http.base.internal.context.ExtServletContext;
import org.apache.felix.http.base.internal.handler.HttpSessionWrapper;
import org.osgi.service.http.HttpContext;
import org.osgi.service.useradmin.Authorization;

final class ServletRequestWrapper extends HttpServletRequestWrapper
{
    private final DispatcherType type;
    private final RequestInfo requestInfo;
    private final ExtServletContext servletContext;
    private final long contextId;
    private final boolean asyncSupported;
    private final MultipartConfig multipartConfig;
    private Collection<Part> parts;

    public ServletRequestWrapper(HttpServletRequest req,
            ExtServletContext servletContext,
            RequestInfo requestInfo,
            DispatcherType type,
            final Long contextId,
            final boolean asyncSupported,
            final MultipartConfig multipartConfig)
    {
        super(req);

        this.asyncSupported = asyncSupported;
        this.multipartConfig = multipartConfig;
        this.servletContext = servletContext;
        this.requestInfo = requestInfo;
        this.type = type;
        this.contextId = contextId;
    }

    @Override
    public Object getAttribute(String name)
    {
        HttpServletRequest request = (HttpServletRequest) getRequest();
        if (isInclusionDispatcher())
        {
            // The javax.servlet.include.* attributes refer to the information of the *included* request,
            // meaning that the request information comes from the *original* request...
            if (INCLUDE_REQUEST_URI.equals(name))
            {
                return this.requestInfo.requestURI;
            }
            else if (INCLUDE_CONTEXT_PATH.equals(name))
            {
                return request.getContextPath();
            }
            else if (INCLUDE_SERVLET_PATH.equals(name))
            {
                return this.requestInfo.servletPath;
            }
            else if (INCLUDE_PATH_INFO.equals(name))
            {
                return this.requestInfo.pathInfo;
            }
            else if (INCLUDE_QUERY_STRING.equals(name))
            {
                return this.requestInfo.queryString;
            }
        }
        else if (isForwardingDispatcher())
        {
            // The javax.servlet.forward.* attributes refer to the information of the *original* request,
            // meaning that the request information comes from the *forwarded* request...
            if (FORWARD_REQUEST_URI.equals(name))
            {
                return super.getRequestURI();
            }
            else if (FORWARD_CONTEXT_PATH.equals(name))
            {
                return request.getContextPath();
            }
            else if (FORWARD_SERVLET_PATH.equals(name))
            {
                return super.getServletPath();
            }
            else if (FORWARD_PATH_INFO.equals(name))
            {
                return super.getPathInfo();
            }
            else if (FORWARD_QUERY_STRING.equals(name))
            {
                return super.getQueryString();
            }
        }
        return super.getAttribute(name);
    }

    @Override
    public String getAuthType()
    {
        String authType = (String) getAttribute(HttpContext.AUTHENTICATION_TYPE);
        if (authType == null)
        {
            authType = super.getAuthType();
        }
        return authType;
    }

    @Override
    public String getContextPath()
    {
        return this.getServletContext().getContextPath();
    }

    @Override
    public DispatcherType getDispatcherType()
    {
        return (this.type == null) ? super.getDispatcherType() : this.type;
    }

    @Override
    public String getPathInfo()
    {
        if ( this.isInclusionDispatcher() )
        {
            return super.getPathInfo();
        }
        return this.requestInfo.pathInfo;
    }

    @Override
    @SuppressWarnings("deprecation")
    public String getPathTranslated()
    {
        final String info = getPathInfo();
        return (null == info) ? null : getRealPath(info);
    }

    @Override
    public String getRemoteUser()
    {
        String remoteUser = (String) getAttribute(HttpContext.REMOTE_USER);
        if (remoteUser != null)
        {
            return remoteUser;
        }

        return super.getRemoteUser();
    }

    @Override
    public RequestDispatcher getRequestDispatcher(String path)
    {
        // See section 9.1 of Servlet 3.0 specification...
        if (path == null)
        {
            return null;
        }
        // Handle relative paths, see Servlet 3.0 spec, section 9.1 last paragraph.
        boolean relPath = !path.startsWith("/") && !"".equals(path);
        if (relPath)
        {
            path = concat(getServletPath(), path);
        }
        return this.servletContext.getRequestDispatcher(path);
    }

    @Override
    public String getRequestURI()
    {
        if ( isInclusionDispatcher() )
        {
            return super.getRequestURI();
        }
        return this.requestInfo.requestURI;
    }

    @Override
    public ServletContext getServletContext()
    {
        return this.servletContext;
    }

    @Override
    public String getServletPath()
    {
        if ( isInclusionDispatcher() )
        {
            return super.getServletPath();
        }
        return this.requestInfo.servletPath;
    }

    @Override
    public HttpSession getSession() {
        return this.getSession(true);
    }

    @Override
    public HttpSession getSession(boolean create)
    {
        // FELIX-2797: wrap the original HttpSession to provide access to the correct ServletContext...
        final HttpSession session = super.getSession(create);
        if (session == null)
        {
            return null;
        }
        // check if internal session is available
        if ( !create && !HttpSessionWrapper.hasSession(this.contextId, session) )
        {
            return null;
        }
        return new HttpSessionWrapper(this.contextId, session, this.servletContext, false);
    }

    @Override
    public boolean isUserInRole(String role)
    {
        Authorization authorization = (Authorization) getAttribute(HttpContext.AUTHORIZATION);
        if (authorization != null)
        {
            return authorization.hasRole(role);
        }

        return super.isUserInRole(role);
    }

    @Override
    public void setAttribute(final String name, final Object value)
    {
        if ( value == null )
        {
            this.removeAttribute(name);
        }
        final Object oldValue = this.getAttribute(name);
        super.setAttribute(name, value);
        if ( this.servletContext.getServletRequestAttributeListener() != null )
        {
            if ( oldValue == null )
            {
                this.servletContext.getServletRequestAttributeListener().attributeAdded(new ServletRequestAttributeEvent(this.servletContext, this, name, value));
            }
            else
            {
                this.servletContext.getServletRequestAttributeListener().attributeReplaced(new ServletRequestAttributeEvent(this.servletContext, this, name, oldValue));
            }
        }
    }

    @Override
    public void removeAttribute(final String name) {
        final Object oldValue = this.getAttribute(name);
        if ( oldValue != null )
        {
            super.removeAttribute(name);
            if ( this.servletContext.getServletRequestAttributeListener() != null )
            {
                this.servletContext.getServletRequestAttributeListener().attributeRemoved(new ServletRequestAttributeEvent(this.servletContext, this, name, oldValue));
            }
        }
    }

    @Override
    public String toString()
    {
        return getClass().getSimpleName() + "->" + super.getRequest();
    }

    private boolean isForwardingDispatcher()
    {
        return (DispatcherType.FORWARD == this.type) && (this.requestInfo != null);
    }

    private boolean isInclusionDispatcher()
    {
        return (DispatcherType.INCLUDE == this.type) && (this.requestInfo != null);
    }

    @Override
    public AsyncContext startAsync() throws IllegalStateException
    {
        if ( !this.asyncSupported )
        {
            throw new IllegalStateException();
        }
        return super.startAsync();
    }

    @Override
    public AsyncContext startAsync(final ServletRequest servletRequest,
            final ServletResponse servletResponse) throws IllegalStateException
    {
        if ( !this.asyncSupported )
        {
            throw new IllegalStateException();
        }
        return super.startAsync(servletRequest, servletResponse);
    }

    @Override
    public boolean isAsyncSupported()
    {
        return this.asyncSupported;
    }

    private Collection<Part> checkMultipart() throws IOException, ServletException
    {
        if ( parts == null )
        {
            if ( ServletFileUpload.isMultipartContent(this) )
            {
                if ( this.multipartConfig == null)
                {
                    throw new IllegalStateException("Multipart not enabled for servlet.");
                }
                // Create a new file upload handler
                final ServletFileUpload upload = new ServletFileUpload();
                upload.setSizeMax(this.multipartConfig.multipartMaxRequestSize);
                upload.setFileSizeMax(this.multipartConfig.multipartMaxFileSize);
                upload.setFileItemFactory(new DiskFileItemFactory(this.multipartConfig.multipartThreshold,
                        new File(this.multipartConfig.multipartLocation)));

                // Parse the request
                List<FileItem> items = null;
                try
                {
                    items = upload.parseRequest(new ServletRequestContext(this));
                }
                catch (final FileUploadException fue)
                {
                    throw new IOException("Error parsing multipart request", fue);
                }
                parts = new ArrayList<Part>();
                for(final FileItem item : items)
                {
                    parts.add(new Part() {

                        @Override
                        public InputStream getInputStream() throws IOException
                        {
                            return item.getInputStream();
                        }

                        @Override
                        public String getContentType()
                        {
                            return item.getContentType();
                        }

                        @Override
                        public String getName()
                        {
                            return item.getFieldName();
                        }

                        @Override
                        public String getSubmittedFileName()
                        {
                            return item.getName();
                        }

                        @Override
                        public long getSize()
                        {
                            return item.getSize();
                        }

                        @Override
                        public void write(String fileName) throws IOException
                        {
                            try
                            {
                                item.write(new File(fileName));
                            }
                            catch (IOException e)
                            {
                                throw e;
                            }
                            catch (Exception e)
                            {
                                throw new IOException(e);
                            }
                        }

                        @Override
                        public void delete() throws IOException
                        {
                            item.delete();
                        }

                        @Override
                        public String getHeader(String name)
                        {
                            return item.getHeaders().getHeader(name);
                        }

                        @Override
                        public Collection<String> getHeaders(String name)
                        {
                            final List<String> values = new ArrayList<String>();
                            final Iterator<String> iter = item.getHeaders().getHeaders(name);
                            while ( iter.hasNext() )
                            {
                                values.add(iter.next());
                            }
                            return values;
                        }

                        @Override
                        public Collection<String> getHeaderNames()
                        {
                            final List<String> names = new ArrayList<String>();
                            final Iterator<String> iter = item.getHeaders().getHeaderNames();
                            while ( iter.hasNext() )
                            {
                                names.add(iter.next());
                            }
                            return names;
                        }
                    });
                }
            }
            else
            {
                throw new ServletException("Not a multipart request");
            }
        }
        return parts;
    }

    @Override
    public Collection<Part> getParts() throws IOException, ServletException
    {
        return checkMultipart();

    }

    @Override
    public Part getPart(String name) throws IOException, ServletException
    {
        Collection<Part> parts = this.checkMultipart();
        for(final Part p : parts)
        {
            if ( p.getName().equals(name) )
            {
                return p;
            }
        }
        return null;
    }

}
