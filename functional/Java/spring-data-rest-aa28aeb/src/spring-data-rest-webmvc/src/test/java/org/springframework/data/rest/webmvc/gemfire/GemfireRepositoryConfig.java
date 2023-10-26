/*
 * Copyright 2012 the original author or authors.
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
package org.springframework.data.rest.webmvc.gemfire;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.ImportResource;
import org.springframework.data.gemfire.mapping.GemfireMappingContext;
import org.springframework.data.gemfire.mapping.Region;
import org.springframework.data.gemfire.repository.config.EnableGemfireRepositories;
import org.springframework.data.util.AnnotatedTypeScanner;

/**
 * Spring JavaConfig configuration class to setup a Spring container and infrastructure components.
 * 
 * @author Oliver Gierke
 * @author David Turanski
 */
@Configuration
@ImportResource("classpath:META-INF/spring/cache-config.xml")
@EnableGemfireRepositories
public class GemfireRepositoryConfig {

	/**
	 * TODO: Remove, once Spring Data Gemfire exposes a mapping context.
	 */
	@Bean
	@SuppressWarnings("unchecked")
	public GemfireMappingContext gemfireMappingContext() {

		AnnotatedTypeScanner scanner = new AnnotatedTypeScanner(Region.class);

		GemfireMappingContext context = new GemfireMappingContext();
		context.setInitialEntitySet(scanner.findTypes(GemfireRepositoryConfig.class.getPackage().getName()));
		context.initialize();

		return context;
	}
}
