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
package org.apache.logging.log4j.audit.catalog;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.ObjectMapper;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.audit.exception.AuditException;
import org.apache.logging.log4j.audit.util.NamingUtils;
import org.apache.logging.log4j.catalog.api.Attribute;
import org.apache.logging.log4j.catalog.api.CatalogData;
import org.apache.logging.log4j.catalog.api.CatalogReader;
import org.apache.logging.log4j.catalog.api.Event;
import org.apache.logging.log4j.catalog.api.EventAttribute;

import static java.util.Collections.emptyList;
import static org.apache.logging.log4j.catalog.api.constant.Constants.DEFAULT_CATALOG;

/**
 *
 */
public class CatalogManagerImpl implements CatalogManager {

    private static final Logger logger = LogManager.getLogger(CatalogManagerImpl.class);

    private volatile Map<String, Map<String, CatalogInfo>> infoMap;

    private final Map<String, Attribute> requestContextAttributes = new HashMap<>();

    protected final Map<String, Map<String, Attribute>> attributeMap = new ConcurrentHashMap<>();

    private static final String REQCTX = "ReqCtx_";

    protected CatalogData catalogData;

    public CatalogManagerImpl(CatalogReader catalogReader) {
        try {
            infoMap = initializeData(catalogReader);
        } catch (Exception ex) {
            throw new AuditException("Unable to initialize catalog data", ex);
        }
    }

    protected Map<String, Map<String, CatalogInfo>> getInfoMap() {
        return infoMap;
    }

    @Override
    public Event getEvent(String eventName, String catalogId) {
        CatalogInfo info = getCatalogInfo(eventName, catalogId);
        return info != null ? info.event : null;
    }

    @Override
    public List<String> getRequiredContextAttributes(String eventName, String catalogId) {
        Map<String, CatalogInfo> catalogMap = infoMap.get(catalogId == null ? DEFAULT_CATALOG : catalogId);
        return catalogMap != null ? catalogMap.get(eventName).requiredContextAttributes : null;
    }

    @Override
    public Map<String, Attribute> getAttributes(String eventName, String catalogId) {
        Event event = getEvent(eventName, catalogId);
        if (event == null) {
            logger.warn("The event named {} could not be found in catalog {}", eventName, catalogId);
            return null;
        }
        List<EventAttribute> eventAttributes = event.getAttributes() == null ? emptyList() : event.getAttributes();
        Map<String, Attribute> attributes = new HashMap<>(eventAttributes.size());
        for (EventAttribute eventAttribute : eventAttributes) {
            Attribute attr = getAttribute(eventAttribute.getName(), event.getCatalogId());
            if (attr != null) {
                attributes.put(attr.getName(), attr);
            }
        }
        return attributes;
    }

    @Override
    public List<String> getAttributeNames(String eventName, String catalogId) {
        return infoMap.get(catalogId == null ? DEFAULT_CATALOG : catalogId).get(eventName).attributeNames;
    }

    @Override
    public Attribute getAttribute(String name) {
        Map<String, Attribute> attrMap = attributeMap.get(DEFAULT_CATALOG);
        return attrMap != null ? attrMap.get(name) : null;
    }

    public Attribute getAttribute(String name, String catalogId) {
        Map<String, Attribute> attrMap = attributeMap.get(catalogId);
        if (attrMap == null || !attrMap.containsKey(name)) {
            attrMap = attributeMap.get(DEFAULT_CATALOG);
        }
        return attrMap != null ? attrMap.get(name) : null;
    }

    @Override
    public Map<String, Attribute> getRequestContextAttributes() {
        return requestContextAttributes;
    }

    private CatalogInfo getCatalogInfo(String eventName, String catalogId) {
        Map<String, CatalogInfo> defaultCatalog = infoMap.get(DEFAULT_CATALOG);
        Map<String, CatalogInfo> catalog = catalogId != null ? infoMap.get(catalogId) : null;
        return catalog != null && catalog.containsKey(eventName) ? catalog.get(eventName) :
                defaultCatalog.get(eventName);
    }

    private Map<String, Map<String, CatalogInfo>> initializeData(CatalogReader catalogReader) throws Exception {
        JsonFactory factory = new JsonFactory();
        factory.enable(JsonParser.Feature.ALLOW_COMMENTS);
        ObjectMapper mapper = new ObjectMapper(factory);

        String catalog = catalogReader.readCatalog();
        catalogData = mapper.readValue(catalog, CatalogData.class);

        if (catalogData.getAttributes() != null) {
            for (Attribute attr : catalogData.getAttributes()) {
                if (attr.isRequestContext()) {
                    requestContextAttributes.put(attr.getName(), attr);
                }
                Map<String, Attribute> attrMap = attributeMap.computeIfAbsent(attr.getCatalogId(), k -> new HashMap<>());
                attrMap.put(attr.getName(), attr);
            }
        }

        Map<String, Map<String, CatalogInfo>> map = new HashMap<>();
        map.put(DEFAULT_CATALOG, new HashMap<>());
        for (Event event : catalogData.getEvents()) {
            addEntry(map, event);
        }
        return map;
    }

protected void addEntry(java.util.Map<java.lang.String, java.util.Map<java.lang.String, org.apache.logging.log4j.audit.catalog.CatalogManagerImpl.CatalogInfo>> map, org.apache.logging.log4j.catalog.api.Event event) {
    org.apache.logging.log4j.audit.catalog.CatalogManagerImpl.CatalogInfo info = new org.apache.logging.log4j.audit.catalog.CatalogManagerImpl.CatalogInfo();
    info.event = event;
    java.lang.String catalogId = event.getCatalogId();
    if (((catalogId != null) && (catalogId.length() > 0)) && (!map.containsKey(catalogId))) {
        map.put(catalogId, new java.util.HashMap<>());
    }
    java.util.List<java.lang.String> required = new java.util.ArrayList<>();
    java.util.List<java.lang.String> names = new java.util.ArrayList<>();
    info.attributes = new java.util.HashMap<>(names.size());
    {
        for (org.apache.logging.log4j.catalog.api.EventAttribute eventAttribute : event.getAttributes()) {
            java.lang.String name = eventAttribute.getName();
            org.apache.logging.log4j.catalog.api.Attribute attribute = getAttribute(name, event.getCatalogId());
            {
                info.attributes.put(name, /* NPEX_NULL_EXP */
                attribute);
                if (name.indexOf('.') != (-1)) {
                    name = name.replaceAll("\\.", "");
                }
                if (name.indexOf('/') != (-1)) {
                    name = name.replaceAll("/", "");
                }
                if (attribute.isRequestContext()) {
                    if (attribute.isRequired()) {
                        if (name.startsWith(org.apache.logging.log4j.audit.catalog.CatalogManagerImpl.REQCTX)) {
                            name = name.substring(org.apache.logging.log4j.audit.catalog.CatalogManagerImpl.REQCTX.length());
                        }
                        required.add(name);
                    }
                } else {
                    names.add(name);
                }
            }
        }
    }
    info.requiredContextAttributes = required;
    info.attributeNames = names;
    java.util.Map<java.lang.String, org.apache.logging.log4j.audit.catalog.CatalogManagerImpl.CatalogInfo> catalogMap = (catalogId == null) ? map.get(org.apache.logging.log4j.catalog.api.constant.Constants.DEFAULT_CATALOG) : map.get(catalogId);
    catalogMap.put(org.apache.logging.log4j.audit.util.NamingUtils.getFieldName(event.getName()), info);
}

    protected class CatalogInfo {
        private Event event;

        private List<String> requiredContextAttributes;

        private List<String> attributeNames;

        private Map<String, Attribute> attributes;
    }
}
