/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache license, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the license for the specific language governing permissions and
 * limitations under the license.
 */
package org.apache.logging.log4j.audit.generator;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.audit.util.NamingUtils;
import org.apache.logging.log4j.catalog.api.Attribute;
import org.apache.logging.log4j.catalog.api.CatalogData;
import org.apache.logging.log4j.catalog.api.CatalogReader;
import org.apache.logging.log4j.catalog.api.Constraint;
import org.apache.logging.log4j.catalog.api.ConstraintType;
import org.apache.logging.log4j.catalog.api.Event;
import org.apache.logging.log4j.catalog.api.EventAttribute;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

@Component
public class InterfacesGenerator {

    private static final Logger LOGGER = LogManager.getLogger(InterfacesGenerator.class);

    private static final String CONSTRAINT_IMPORT = "org.apache.logging.log4j.audit.annotation.Constraint";
    private static final String REQUIRED_IMPORT = "org.apache.logging.log4j.audit.annotation.Required";
    private static final String CONSTRAINTS_ATTR = ", constraints={";
    private static final String CONSTRAINT = "@Constraint(constraintType=\"%s\", constraintValue=\"%s\")";
    private static final String KEY = "key=\"";
    private static final String REQUIRED_ATTR = "required=true";
    private static final String REQUIRED = "@Required";

    private static final String REQUEST_CONTEXT_IMPORT = "org.apache.logging.log4j.audit.annotation.RequestContext";
    private static final String PARENT_IMPORT = "org.apache.logging.log4j.audit.AuditEvent";
    private static final String EVENT_NAME_IMPORT = "org.apache.logging.log4j.audit.annotation.EventName";
    private static final String MAX_LENGTH_IMPORT = "org.apache.logging.log4j.audit.annotation.MaxLength";
    private static final String REQCTX_ANN = "@RequestContext(";

    private static final String PARENT_CLASS = "AuditEvent";

    private static final String REQCTX = "ReqCtx_";

    private static final String EVENT_ID = "eventID";

    private static final String EVENT_TYPE = "eventType";

    private static final String TIMESTAMP = "timeStamp";

    private static final String CONTEXT = "context";

    @Autowired
    private CatalogReader catalogReader;

    @Value("${packageName:org.apache.logging.log4j.audit.event}")
    private String packageName;

    @Value("${outputDirectory:target/generated-sources/log4j-audit}")
    private String outputDirectory;

    @Value("${maxKeyLength:32}")
    private int maxKeyLength;

    @Value("${enterpriseId:18060}")
    private int enterpriseId;

    @Value("${verbose:false}")
    private boolean verbose;

    public CatalogReader getCatalogReader() {
        return catalogReader;
    }

    public void setCatalogReader(CatalogReader catalogReader) {
        this.catalogReader = catalogReader;
    }

    public String getPackageName() {
        return packageName;
    }

    public void setPackageName(String packageName) {
        this.packageName = packageName;
    }

    public String getOutputDirectory() {
        return outputDirectory;
    }

    public void setOutputDirectory(String outputDirectory) {
        this.outputDirectory = outputDirectory;
    }

    public void setMaxKeyLength(int maxKeyLength) {
        this.maxKeyLength = maxKeyLength;
    }

    public void setEnterpriseId(int enterpriseId) {
        this.enterpriseId = enterpriseId;
    }

    public void setVerbose(boolean verbose) {
        this.verbose = verbose;
    }

public void generateSource() throws java.lang.Exception {
    boolean errors = false;
    org.apache.logging.log4j.catalog.api.CatalogData catalogData = catalogReader.read();
    {
        java.util.List<org.apache.logging.log4j.catalog.api.Event> events = catalogData.getEvents();
        java.util.Map<java.lang.String, org.apache.logging.log4j.catalog.api.Attribute> requestContextAttrs = new java.util.HashMap<>();
        java.util.Map<java.lang.String, java.lang.Boolean> requestContextIsRequired = new java.util.HashMap<>();
        java.util.Map<java.lang.String, org.apache.logging.log4j.catalog.api.Attribute> attributes = catalogReader.getAttributes();
        java.util.Map<java.lang.String, java.lang.String> importedTypes = new java.util.HashMap<>();
        boolean anyConstraints = false;
        for (org.apache.logging.log4j.catalog.api.Attribute attribute : attributes.values()) {
            if (attribute.isRequestContext()) {
                java.lang.String name = attribute.getName();
                if (name.startsWith(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQCTX)) {
                    name = name.substring(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQCTX.length());
                }
                requestContextAttrs.put(name, attribute);
                requestContextIsRequired.put(name, attribute.isRequired());
            }
        }
        for (org.apache.logging.log4j.catalog.api.Event event : events) {
            java.lang.String maxLen = java.lang.Integer.toString(enterpriseId);
            int maxNameLength = (maxKeyLength - maxLen.length()) - 1;
            if (event.getName().length() > maxNameLength) {
                org.apache.logging.log4j.audit.generator.InterfacesGenerator.LOGGER.error("{} exceeds maximum length of {} for an event name", event.getName(), maxNameLength);
                errors = true;
                continue;
            }
            org.apache.logging.log4j.audit.generator.ClassGenerator classGenerator = new org.apache.logging.log4j.audit.generator.ClassGenerator(org.apache.logging.log4j.audit.util.NamingUtils.getClassName(event.getName()), outputDirectory);
            classGenerator.setClass(false);
            classGenerator.setPackageName(packageName);
            classGenerator.setParentClassName(org.apache.logging.log4j.audit.generator.InterfacesGenerator.PARENT_CLASS);
            classGenerator.setJavadocComment(event.getDescription());
            classGenerator.setVerbose(verbose);
            java.util.Set<java.lang.String> imports = classGenerator.getImports();
            imports.add(org.apache.logging.log4j.audit.generator.InterfacesGenerator.PARENT_IMPORT);
            java.lang.StringBuilder annotations = new java.lang.StringBuilder();
            imports.add(org.apache.logging.log4j.audit.generator.InterfacesGenerator.EVENT_NAME_IMPORT);
            annotations.append("@EventName(\"").append(event.getName()).append("\")\n");
            imports.add(org.apache.logging.log4j.audit.generator.InterfacesGenerator.MAX_LENGTH_IMPORT);
            annotations.append("@MaxLength(").append(maxKeyLength).append(")");
            java.util.List<org.apache.logging.log4j.catalog.api.EventAttribute> eventAttributes = event.getAttributes();
            boolean anyRequired = false;
            {
                for (org.apache.logging.log4j.catalog.api.EventAttribute eventAttribute : eventAttributes) {
                    org.apache.logging.log4j.catalog.api.Attribute attribute = attributes.get(eventAttribute.getName());
                    {
                        if (/* NPEX_NULL_EXP */
                        attribute.isRequestContext() && attribute.isRequired()) {
                            java.lang.String name = eventAttribute.getName();
                            if (name.startsWith(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQCTX)) {
                                name = name.substring(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQCTX.length());
                            }
                            requestContextIsRequired.put(name, java.lang.Boolean.TRUE);
                            continue;
                        }
                        java.lang.String name = attribute.getName();
                        if ((org.apache.logging.log4j.audit.generator.InterfacesGenerator.EVENT_ID.equals(name) || org.apache.logging.log4j.audit.generator.InterfacesGenerator.EVENT_TYPE.equals(name)) || org.apache.logging.log4j.audit.generator.InterfacesGenerator.TIMESTAMP.equals(name)) {
                            continue;
                        }
                        if (name.indexOf('.') != (-1)) {
                            name = name.replaceAll("\\.", "");
                        }
                        if (name.indexOf('/') != (-1)) {
                            name = name.replaceAll("/", "");
                        }
                        if (name.length() > maxKeyLength) {
                            org.apache.logging.log4j.audit.generator.InterfacesGenerator.LOGGER.error("{} exceeds maximum length of {} for an attribute name", name, maxKeyLength);
                            errors = true;
                            continue;
                        }
                        java.lang.String type = attribute.getDataType().getTypeName();
                        org.apache.logging.log4j.audit.generator.MethodDefinition definition = new org.apache.logging.log4j.audit.generator.MethodDefinition("void", org.apache.logging.log4j.audit.util.NamingUtils.getMutatorName(name));
                        if ((!attribute.isRequestContext()) && (attribute.getDataType().getImportClass() != null)) {
                            if (!importedTypes.containsKey(attribute.getDataType().getTypeName())) {
                                importedTypes.put(attribute.getDataType().getTypeName(), attribute.getDataType().getImportClass());
                            }
                        }
                        definition.addParameter(new org.apache.logging.log4j.audit.generator.Parameter(name, type, attribute.getDescription()));
                        definition.setInterface(true);
                        definition.setVisability("public");
                        definition.setJavadocComments((attribute.getDisplayName() + " : ") + attribute.getDescription());
                        java.lang.StringBuilder buffer = new java.lang.StringBuilder();
                        java.util.Set<org.apache.logging.log4j.catalog.api.Constraint> constraints = attribute.getConstraints();
                        boolean first = true;
                        if (attribute.isRequired() || eventAttribute.isRequired()) {
                            anyRequired = true;
                            buffer.append(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQUIRED);
                            first = false;
                        }
                        if ((constraints != null) && (constraints.size() > 0)) {
                            anyConstraints = true;
                            for (org.apache.logging.log4j.catalog.api.Constraint constraint : constraints) {
                                if (!first) {
                                    buffer.append("\n    ");
                                }
                                first = false;
                                appendConstraint(constraint, buffer);
                            }
                        }
                        if (buffer.length() > 0) {
                            definition.setAnnotation(buffer.toString());
                        }
                        classGenerator.addMethod(definition);
                    }
                }
            }
            if (importedTypes.size() > 0) {
                imports.addAll(importedTypes.values());
            }
            if (anyRequired) {
                imports.add(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQUIRED_IMPORT);
            }
            boolean firstReqCtx = true;
            if (requestContextAttrs.size() > 0) {
                imports.add(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQUEST_CONTEXT_IMPORT);
                java.lang.StringBuilder reqCtx = new java.lang.StringBuilder();
                for (java.util.Map.Entry<java.lang.String, org.apache.logging.log4j.catalog.api.Attribute> entry : requestContextAttrs.entrySet()) {
                    if (!firstReqCtx) {
                        reqCtx.append(")\n");
                    }
                    firstReqCtx = false;
                    reqCtx.append(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQCTX_ANN);
                    reqCtx.append(org.apache.logging.log4j.audit.generator.InterfacesGenerator.KEY).append(entry.getKey()).append("\"");
                    org.apache.logging.log4j.catalog.api.Attribute attrib = entry.getValue();
                    java.lang.String name = attrib.getName();
                    if (name.startsWith(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQCTX)) {
                        name = name.substring(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQCTX.length());
                    }
                    java.lang.Boolean isRequired = null;
                    final java.lang.String attrName = name;
                    if (event.getAttributes() != null) {
                        java.util.Optional<org.apache.logging.log4j.catalog.api.EventAttribute> optional = event.getAttributes().stream().filter(( a) -> attrName.equals(a.getName())).findFirst();
                        if (optional.isPresent()) {
                            isRequired = optional.get().isRequired();
                        }
                    }
                    if (((isRequired != null) && isRequired) || ((isRequired == null) && requestContextIsRequired.get(name))) {
                        reqCtx.append(", ").append(org.apache.logging.log4j.audit.generator.InterfacesGenerator.REQUIRED_ATTR);
                    }
                    java.util.Set<org.apache.logging.log4j.catalog.api.Constraint> constraints = entry.getValue().getConstraints();
                    if ((constraints != null) && (constraints.size() > 0)) {
                        anyConstraints = true;
                        reqCtx.append(org.apache.logging.log4j.audit.generator.InterfacesGenerator.CONSTRAINTS_ATTR);
                        boolean first = true;
                        for (org.apache.logging.log4j.catalog.api.Constraint constraint : constraints) {
                            if (!first) {
                                reqCtx.append(", ");
                            }
                            first = false;
                            appendConstraint(constraint, reqCtx);
                        }
                        reqCtx.append("}");
                    }
                }
                reqCtx.append(")");
                if (annotations.length() > 0) {
                    annotations.append("\n");
                }
                annotations.append(reqCtx.toString());
            }
            if (anyConstraints) {
                imports.add(org.apache.logging.log4j.audit.generator.InterfacesGenerator.CONSTRAINT_IMPORT);
            }
            if (annotations.length() > 0) {
                classGenerator.setAnnotations(annotations.toString());
            }
            classGenerator.generate();
        }
    }
    if (errors) {
        throw new java.lang.IllegalStateException("Errors were encountered during code generation");
    }
}

    void appendConstraint(Constraint constraint, StringBuilder buffer) {
        ConstraintType type = constraint.getConstraintType();
        // Add the escapes since they have been removed when converting the original data to a Java Strinng. They need to
        // be added back for use in the Constraint declaration.
        buffer.append(String.format(CONSTRAINT, type.getName(), constraint.getValue().replace("\\", "\\\\")));
    }
}
