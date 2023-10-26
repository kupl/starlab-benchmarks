/*
 * Copyright (c) OSGi Alliance (2016). All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.felix.schematizer;

import java.util.Collection;
import java.util.Map;

public interface Schema {
    String name();
    Node rootNode();
    boolean hasNodeAtPath(String absolutePath);
    Node nodeAtPath(String absolutePath);
    Node parentOf(Node aNode);
    Map<String, Node.DTO> toMap();

    /**
     * Recursively visits all nodes in the {@code Schema} for processing.
     */
    void visit(NodeVisitor visitor);

    Collection<?> valuesAt(String path, Object object);
}
