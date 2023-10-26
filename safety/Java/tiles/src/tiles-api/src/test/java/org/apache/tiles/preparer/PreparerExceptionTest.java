/*
 * $Id$
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.apache.tiles.preparer;

import static org.junit.Assert.*;

import org.junit.Test;

/**
 * Tests {@link PreparerException}.
 *
 * @version $Rev$ $Date$
 */
public class PreparerExceptionTest {

    /**
     * Test method for {@link PreparerException#PreparerException()}.
     */
    @Test
    public void testPreparerException() {
        PreparerException exception = new PreparerException();
        assertNull(exception.getMessage());
        assertNull(exception.getCause());
    }

    /**
     * Test method for {@link PreparerException#PreparerException(java.lang.String)}.
     */
    @Test
    public void testPreparerExceptionString() {
        PreparerException exception = new PreparerException("my message");
        assertEquals("my message", exception.getMessage());
        assertNull(exception.getCause());
    }

    /**
     * Test method for {@link PreparerException#PreparerException(java.lang.Throwable)}.
     */
    @Test
    public void testPreparerExceptionThrowable() {
        Throwable cause = new Throwable();
        PreparerException exception = new PreparerException(cause);
        assertEquals(cause.toString(), exception.getMessage());
        assertEquals(cause, exception.getCause());
    }

    /**
     * Test method for {@link PreparerException#PreparerException(java.lang.String, java.lang.Throwable)}.
     */
    @Test
    public void testPreparerExceptionStringThrowable() {
        Throwable cause = new Throwable();
        PreparerException exception = new PreparerException("my message", cause);
        assertEquals("my message", exception.getMessage());
        assertEquals(cause, exception.getCause());
    }

}
