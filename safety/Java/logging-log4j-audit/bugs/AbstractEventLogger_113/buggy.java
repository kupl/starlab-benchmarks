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
package org.apache.logging.log4j.audit;

import org.apache.logging.log4j.ThreadContext;
import org.apache.logging.log4j.audit.catalog.CatalogManager;
import org.apache.logging.log4j.audit.exception.AuditException;
import org.apache.logging.log4j.audit.exception.ConstraintValidationException;
import org.apache.logging.log4j.catalog.api.Attribute;
import org.apache.logging.log4j.catalog.api.Constraint;
import org.apache.logging.log4j.catalog.api.Event;
import org.apache.logging.log4j.catalog.api.EventAttribute;
import org.apache.logging.log4j.catalog.api.plugins.ConstraintPlugins;
import org.apache.logging.log4j.message.StructuredDataMessage;

import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.Set;

import static java.util.Collections.emptyList;
import static java.util.Collections.emptyMap;

/**
 * This class is used to log events generated remotely.
 */
public abstract class AbstractEventLogger {

    private static final int DEFAULT_MAX_LENGTH = 32;

    private static final ConstraintPlugins constraintPlugins = ConstraintPlugins.getInstance();

    public CatalogManager catalogManager;

    private static final AuditExceptionHandler DEFAULT_EXCEPTION_HANDLER = (message, ex) -> {
        throw new AuditException("Error logging event " + message.getId().getName(), ex);
    };

    private static final AuditExceptionHandler NOOP_EXCEPTION_HANDLER = (message, ex) -> {
    };

    private AuditExceptionHandler defaultAuditExceptionHandler = DEFAULT_EXCEPTION_HANDLER;

    private final int maxLength;

    protected AbstractEventLogger() {
        maxLength = DEFAULT_MAX_LENGTH;
    }

    protected AbstractEventLogger(int maxLength) {
        this.maxLength = maxLength;
    }

    public void setCatalogManager(CatalogManager catalogManager) {
        this.catalogManager = catalogManager;
    }

    public List<String> getAttributeNames(String eventId) {
        return catalogManager.getAttributeNames(eventId);
    }

    public void setDefaultAuditExceptionHandler(AuditExceptionHandler auditExceptionHandler) {
        defaultAuditExceptionHandler = auditExceptionHandler == null ? NOOP_EXCEPTION_HANDLER : auditExceptionHandler;
    }

    public void logEvent(String eventName, Map<String, String> attributes) {
        logEvent(eventName, null, attributes, defaultAuditExceptionHandler);
    }

    public void logEvent(String eventName, String catalogId, Map<String, String> attributes) {
        logEvent(eventName, catalogId, attributes, defaultAuditExceptionHandler);
    }

    public void logEvent(String eventName, Map<String, String> attributes, AuditExceptionHandler exceptionHandler) {
        logEvent(eventName, null, attributes, exceptionHandler);
    }

    private void logEvent(String eventName, String catalogId, Map<String, String> attributes, AuditExceptionHandler exceptionHandler) {
        Event event = catalogId == null ? catalogManager.getEvent(eventName) : catalogManager.getEvent(eventName, catalogId);
        if (event == null) {
            throw new AuditException("Unable to locate definition of audit event " + eventName);
        }
        logEvent(eventName, attributes, event, exceptionHandler);
    }

    protected abstract void logEvent(StructuredDataMessage message);

private void logEvent(java.lang.String eventName, java.util.Map<java.lang.String, java.lang.String> attributes, org.apache.logging.log4j.catalog.api.Event event, org.apache.logging.log4j.audit.AuditExceptionHandler exceptionHandler) {
    org.apache.logging.log4j.audit.AuditMessage msg = new org.apache.logging.log4j.audit.AuditMessage(eventName, maxLength);
    if (attributes == null) {
        attributes = java.util.Collections.emptyMap();
    }
    java.lang.StringBuilder missingAttributes = new java.lang.StringBuilder();
    java.lang.StringBuilder errors = new java.lang.StringBuilder();
    java.util.List<org.apache.logging.log4j.catalog.api.EventAttribute> eventAttributes = /* NPEX_NULL_EXP */
    event.getAttributes();
    for (org.apache.logging.log4j.catalog.api.EventAttribute eventAttribute : eventAttributes) {
        org.apache.logging.log4j.catalog.api.Attribute attr = catalogManager.getAttribute(eventAttribute.getName(), event.getCatalogId());
        if (((!attr.isRequestContext()) && attr.isRequired()) || ((eventAttribute.isRequired() != null) && eventAttribute.isRequired())) {
            java.lang.String name = attr.getName();
            if (!attributes.containsKey(name)) {
                if (missingAttributes.length() > 0) {
                    missingAttributes.append(", ");
                }
                missingAttributes.append(name);
            } else if ((attr.getConstraints() != null) && (attr.getConstraints().size() > 0)) {
                org.apache.logging.log4j.audit.AbstractEventLogger.validateConstraints(false, attr.getConstraints(), name, attributes.get(name), errors);
            }
        }
    }
    java.util.Map<java.lang.String, org.apache.logging.log4j.catalog.api.Attribute> attributeMap = catalogManager.getAttributes(eventName, event.getCatalogId());
    for (java.lang.String name : attributes.keySet()) {
        if ((!attributeMap.containsKey(name)) && (!name.equals("completionStatus"))) {
            if (errors.length() > 0) {
                errors.append("\n");
            }
            errors.append("Attribute ").append(name).append(" is not defined for ").append(eventName);
        }
    }
    if (missingAttributes.length() > 0) {
        if (errors.length() > 0) {
            errors.append("\n");
        }
        errors.append("Event ").append(eventName).append(" is missing required attribute(s) ").append(missingAttributes.toString());
    }
    if (errors.length() > 0) {
        throw new org.apache.logging.log4j.audit.exception.ConstraintValidationException(errors.toString());
    }
    java.util.List<java.lang.String> attributeNames = catalogManager.getAttributeNames(eventName, event.getCatalogId());
    java.lang.StringBuilder buf = new java.lang.StringBuilder();
    for (java.lang.String attribute : attributes.keySet()) {
        if (!attributeNames.contains(attribute)) {
            if (buf.length() > 0) {
                buf.append(", ");
            }
            buf.append(attribute);
        }
    }
    if (buf.length() > 0) {
        throw new org.apache.logging.log4j.audit.exception.ConstraintValidationException((("Event " + eventName) + " contains invalid attribute(s) ") + buf.toString());
    }
    java.util.List<java.lang.String> reqCtxAttrs = catalogManager.getRequiredContextAttributes(eventName, event.getCatalogId());
    if ((reqCtxAttrs != null) && (!reqCtxAttrs.isEmpty())) {
        java.lang.StringBuilder sb = new java.lang.StringBuilder();
        for (java.lang.String attr : reqCtxAttrs) {
            if (!org.apache.logging.log4j.ThreadContext.containsKey(attr)) {
                if (sb.length() > 0) {
                    sb.append(", ");
                }
                sb.append(attr);
            }
        }
        if (sb.length() > 0) {
            throw new org.apache.logging.log4j.audit.exception.ConstraintValidationException((("Event " + msg.getId().getName()) + " is missing required RequestContextMapping values for ") + sb.toString());
        }
    }
    java.util.Map<java.lang.String, org.apache.logging.log4j.catalog.api.Attribute> reqCtxAttributes = catalogManager.getRequestContextAttributes();
    for (java.util.Map.Entry<java.lang.String, org.apache.logging.log4j.catalog.api.Attribute> entry : reqCtxAttributes.entrySet()) {
        org.apache.logging.log4j.catalog.api.Attribute attribute = entry.getValue();
        java.lang.String attr = entry.getKey();
        if (attribute.isRequired() && (!org.apache.logging.log4j.ThreadContext.containsKey(attr))) {
            if (errors.length() > 0) {
                errors.append(", ");
            }
            errors.append(attr);
        }
    }
    if (errors.length() > 0) {
        throw new org.apache.logging.log4j.audit.exception.ConstraintValidationException((("Event " + eventName) + " is missing required Thread Context values for ") + errors.toString());
    }
    for (java.util.Map.Entry<java.lang.String, org.apache.logging.log4j.catalog.api.Attribute> entry : reqCtxAttributes.entrySet()) {
        org.apache.logging.log4j.catalog.api.Attribute attribute = reqCtxAttributes.get(entry.getKey());
        if (!org.apache.logging.log4j.ThreadContext.containsKey(entry.getKey())) {
            continue;
        }
        java.util.Set<org.apache.logging.log4j.catalog.api.Constraint> constraintList = attribute.getConstraints();
        if ((constraintList != null) && (constraintList.size() > 0)) {
            org.apache.logging.log4j.audit.AbstractEventLogger.validateConstraints(true, constraintList, entry.getKey(), org.apache.logging.log4j.ThreadContext.get(entry.getKey()), errors);
        }
    }
    if (errors.length() > 0) {
        throw new org.apache.logging.log4j.audit.exception.ConstraintValidationException((("Event " + eventName) + " has incorrect data in the Thread Context: ") + errors.toString());
    }
    msg.putAll(attributes);
    try {
        logEvent(msg);
    } catch (java.lang.Throwable ex) {
        if (exceptionHandler == null) {
            defaultAuditExceptionHandler.handleException(msg, ex);
        } else {
            exceptionHandler.handleException(msg, ex);
        }
    }
}

    private static void validateConstraints(boolean isRequestContext, Collection<Constraint> constraints, String name,
                                            String value, StringBuilder errors) {
        for (Constraint constraint : constraints) {
            constraintPlugins.validateConstraint(isRequestContext, constraint.getConstraintType().getName(), name, value,
                    constraint.getValue(), errors);
        }
    }
}
