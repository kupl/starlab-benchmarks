/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. The ASF
 * licenses this file to You under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package org.apache.sling.feature.maven;

import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;
import java.io.StringWriter;
import java.io.Writer;
import java.util.Collections;
import java.util.Map;

import javax.json.Json;
import javax.json.JsonException;
import javax.json.JsonObject;
import javax.json.JsonObjectBuilder;
import javax.json.JsonReader;
import javax.json.JsonValue;
import javax.json.JsonWriter;
import javax.json.stream.JsonGenerator;
import javax.json.stream.JsonGeneratorFactory;

import org.apache.felix.cm.json.Configurations;
import org.apache.sling.feature.ArtifactId;
import org.apache.sling.feature.Feature;
import org.apache.sling.feature.io.json.FeatureJSONWriter;

public class JSONFeatures {

    public static final ArtifactId PLACEHOLDER_ID = new ArtifactId("_", "_", "1.0", null, null);

    /**
     * Read the feature and add the {@code id} attribute if missing
     * @param reader The reader
     * @param optionalId The artifact id to use if the {@code id} attribute is missing
     * @param location The location
     * @return The feature as a string
     * @throws IOException If reading fails
     */
    public static String read(final Reader reader, final ArtifactId optionalId, final String location)
    throws IOException {
        JsonObject featureObj;
        try (JsonReader jsonReader = Json.createReader(Configurations.jsonCommentAwareReader(reader))) {
            featureObj = jsonReader.readObject();
            if ( !featureObj.containsKey("id") ) {
                final JsonObjectBuilder job = Json.createObjectBuilder();
                job.add("id", optionalId.toMvnId());
                for(final Map.Entry<String, JsonValue> prop : featureObj.entrySet() ) {
                    job.add(prop.getKey(), prop.getValue());
                }
                featureObj = job.build();
            }
        } catch ( final JsonException je) {
            throw new IOException(location.concat(" : " ).concat(je.getMessage()), je);
        }

        try ( final StringWriter writer = new StringWriter()) {
            try ( final JsonWriter jsonWriter = Json.createWriter(writer)) {
                jsonWriter.writeObject(featureObj);
            }
            return writer.toString();
        }
    }

    /**
     * Write the feature. If the id is the {@link #PLACEHOLDER_ID} then the id is not written out
     * @param writer The writer
     * @param feature The feature
     * @throws IOException If anything goes wrong
     */
    public static void write(final Writer writer, final Feature feature) throws IOException {
        if ( feature.getId().equals(PLACEHOLDER_ID)) {
            // write feature with placeholder id
            try ( final StringWriter stringWriter = new StringWriter()) {
                FeatureJSONWriter.write(stringWriter, feature);

                // read feature as json object
                try (JsonReader jsonReader = Json.createReader(new StringReader(stringWriter.toString()))) {
                    final JsonObject featureObj = jsonReader.readObject();

                    final JsonGeneratorFactory factory = Json.createGeneratorFactory(Collections.singletonMap(JsonGenerator.PRETTY_PRINTING, true));
                    // write feature object, except id property
                    try (final JsonGenerator generator = factory.createGenerator(writer)) {
                        generator.writeStartObject();

                        for (final Map.Entry<String, JsonValue> entry : featureObj.entrySet()) {
                            if ( !"id".equals(entry.getKey()) ) {
                                generator.write(entry.getKey(), entry.getValue());
                            }
                        }
                        generator.writeEnd();
                    }
                } catch ( final JsonException je) {
                    throw new IOException(je.getMessage(), je);
                }
            }
        } else {
            FeatureJSONWriter.write(writer, feature);
        }
    }
}
