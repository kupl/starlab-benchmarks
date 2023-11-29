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
package org.apache.geronimo.jcache.simple;

import static java.util.Arrays.asList;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.ObjectStreamClass;
import java.lang.reflect.Proxy;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.regex.Pattern;

public class Serializations {

    private final Collection<String> acceptedClasses;

    public Serializations(final String acceptedClasses) {
        this.acceptedClasses = acceptedClasses == null ? Collections.<String> emptySet()
                : new HashSet<>(asList(acceptedClasses.split(",")));
    }

    public <K> K copy(final ClassLoader loader, final K key) {
        try {
            return deSerialize(serialize(key), loader);
        } catch (final Exception e) {
            throw new IllegalStateException(e);
        }
    }

    private <T> byte[] serialize(final T obj) throws IOException {
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final ObjectOutputStream oos = new ObjectOutputStream(baos);
        try {
            oos.writeObject(obj);
        } finally {
            oos.close();
        }
        return baos.toByteArray();
    }

    private <T> T deSerialize(final byte[] data, final ClassLoader loader) throws IOException, ClassNotFoundException {
        final ByteArrayInputStream bais = new ByteArrayInputStream(data);
        final BufferedInputStream bis = new BufferedInputStream(bais);
        final ObjectInputStream ois = new ObjectInputStreamClassLoaderAware(bis, loader, acceptedClasses);
        try {
            return (T) ois.readObject();
        } finally {
            ois.close();
        }
    }

    private static class ObjectInputStreamClassLoaderAware extends ObjectInputStream {

        private static final Pattern PRIMITIVE_ARRAY = Pattern.compile("^\\[+[BCDFIJSVZ]$");

        private final ClassLoader classLoader;

        private final Collection<String> accepted;

        public ObjectInputStreamClassLoaderAware(final InputStream in, final ClassLoader classLoader,
                final Collection<String> accepted) throws IOException {
            super(in);
            this.classLoader = classLoader != null ? classLoader : Thread.currentThread().getContextClassLoader();
            this.accepted = accepted;
        }

        @Override
        protected Class<?> resolveClass(final ObjectStreamClass desc) throws ClassNotFoundException {
            if (isAccepted(desc.getName())) {
                return Class.forName(desc.getName(), false, classLoader);
            }
            throw new SecurityException(desc.getName() + " not whitelisted");
        }

        @Override
        protected Class resolveProxyClass(final String[] interfaces) throws ClassNotFoundException {
            final Class[] cinterfaces = new Class[interfaces.length];
            for (int i = 0; i < cinterfaces.length; i++) {
                if (isAccepted(interfaces[i])) {
                    cinterfaces[i] = Class.forName(interfaces[i], false, classLoader);
                } else {
                    throw new SecurityException(interfaces[i] + " not whitelisted");
                }
            }

            try {
                return Proxy.getProxyClass(classLoader, cinterfaces);
            } catch (IllegalArgumentException e) {
                throw new ClassNotFoundException(null, e);
            }
        }

        private boolean isAccepted(final String name) {
            if (PRIMITIVE_ARRAY.matcher(name).matches()) {
                return false;
            }
            if (name.startsWith("[L") && name.endsWith(";")) {
                return isAccepted(name.substring(2, name.length() - 1));
            }
            return !accepted.contains(name);
        }
    }
}
