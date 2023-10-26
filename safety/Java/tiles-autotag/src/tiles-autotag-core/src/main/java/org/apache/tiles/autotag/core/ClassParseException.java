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
package org.apache.tiles.autotag.core;

/**
 * Thrown if there is a problem when parsing a class source.
 *
 * @version $Rev$ $Date$
 */
public class ClassParseException extends AutotagRuntimeException {

	private static final long serialVersionUID = -8579521283073016196L;

	/**
     * Constructor.
     */
    public ClassParseException() {
    }

    /**
     * Constructor.
     *
     * @param message The message of the exception.
     */
    public ClassParseException(String message) {
        super(message);
    }

    /**
     * Constructor.
     *
     * @param cause The cause.
     */
    public ClassParseException(Throwable cause) {
        super(cause);
    }

    /**
     * Constructor.
     *
     * @param message The message of the exception.
     * @param cause The cause.
     */
    public ClassParseException(String message, Throwable cause) {
        super(message, cause);
    }

}
