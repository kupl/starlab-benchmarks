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
package org.apache.sling.caconfig.management.impl.console;

import java.io.PrintWriter;
import java.lang.reflect.Array;

import org.apache.sling.caconfig.spi.ConfigurationMetadataProvider;
import org.apache.sling.caconfig.spi.metadata.ConfigurationMetadata;
import org.apache.sling.caconfig.spi.metadata.PropertyMetadata;
import org.osgi.framework.BundleContext;
import org.osgi.framework.ServiceReference;

/**
 * Print configuration metadata provided by a {@link ConfigurationMetadata}.
 */
class ConfigurationMetadataPrinter implements ServiceConfigurationPrinter<ConfigurationMetadataProvider> {

    @Override
public void printConfiguration(java.io.PrintWriter pw, org.osgi.framework.ServiceReference<org.apache.sling.caconfig.spi.ConfigurationMetadataProvider> serviceReference, org.osgi.framework.BundleContext bundleContext) {
    org.apache.sling.caconfig.spi.ConfigurationMetadataProvider service = bundleContext.getService(serviceReference);
    for (java.lang.String configName : service.getConfigurationNames()) {
        org.apache.sling.caconfig.spi.metadata.ConfigurationMetadata metadata = service.getConfigurationMetadata(configName);
        if (metadata == null) {
            continue;
        }
        pw.print(org.apache.sling.caconfig.management.impl.console.ServiceConfigurationPrinter.INDENT);
        pw.print(org.apache.sling.caconfig.management.impl.console.ServiceConfigurationPrinter.BULLET);
        pw.println(metadata.getName());
        for (org.apache.sling.caconfig.spi.metadata.PropertyMetadata<?> property : metadata.getPropertyMetadata().values()) {
            pw.print(org.apache.sling.caconfig.management.impl.console.ServiceConfigurationPrinter.INDENT_2);
            pw.print(org.apache.sling.caconfig.management.impl.console.ServiceConfigurationPrinter.BULLET);
            pw.print(property.getName());
            pw.print("(");
            pw.print(property.getType().getSimpleName());
            pw.print(")");
            {
                pw.print(" = ");
                printValue(pw, /* NPEX_NULL_EXP */
                property.getDefaultValue());
            }
            pw.println();
        }
    }
    bundleContext.ungetService(serviceReference);
}
    
    private void printValue(PrintWriter pw, Object value) {
        if (value.getClass().isArray()) {
            for (int i=0; i<Array.getLength(value); i++) {
                if (i > 0) {
                    pw.print(", ");
                }
                printValue(pw, Array.get(value, i));
            }
        }
        else {
            pw.print(value);
        }
    }

}
