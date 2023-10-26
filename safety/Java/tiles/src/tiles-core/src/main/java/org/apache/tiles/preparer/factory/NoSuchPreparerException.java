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
package org.apache.tiles.preparer.factory;

import org.apache.tiles.preparer.PreparerException;

/**
 * Thrown when the named preparerInstance can not be found.
 *
 * @since 2.0
 * @version $Rev$ $Date$
 */
public class NoSuchPreparerException extends PreparerException {

    /**
     * Constructor.
     */
    public NoSuchPreparerException() {
        super();
    }

    /**
     * Constructor.
     *
     * @param message The message to include.
     */
    public NoSuchPreparerException(String message) {
        super(message);
    }

    /**
     * Constructor.
     *
     * @param e The cause exception.
     */
    public NoSuchPreparerException(Throwable e) {
        super(e);
    }

    /**
     * Constructor.
     *
     * @param message The message to include.
     * @param e The cause exception.
     */
    public NoSuchPreparerException(String message, Throwable e) {
        super(message, e);
    }
}
