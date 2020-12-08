/*
 * Copyright 2012-2013 the original author or authors.
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
package org.springframework.data.rest.webmvc;

import static org.hamcrest.Matchers.*;
import static org.junit.Assert.*;
import static org.mockito.Matchers.*;
import static org.mockito.Mockito.*;
import static org.springframework.data.rest.webmvc.HttpEntityMatcher.*;
import static org.springframework.util.ReflectionUtils.*;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.hamcrest.Matcher;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.runners.MockitoJUnitRunner;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.core.MethodParameter;
import org.springframework.data.rest.core.projection.ProxyProjectionFactory;
import org.springframework.data.rest.webmvc.ResourceProcessorHandlerMethodReturnValueHandler.ResourcesProcessorWrapper;
import org.springframework.data.util.ClassTypeInformation;
import org.springframework.data.util.TypeInformation;
import org.springframework.hateoas.Link;
import org.springframework.hateoas.PagedResources;
import org.springframework.hateoas.PagedResources.PageMetadata;
import org.springframework.hateoas.Resource;
import org.springframework.hateoas.ResourceProcessor;
import org.springframework.hateoas.Resources;
import org.springframework.hateoas.mvc.HeaderLinksResponseEntity;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.util.ReflectionUtils.MethodCallback;
import org.springframework.web.context.request.NativeWebRequest;
import org.springframework.web.method.support.HandlerMethodReturnValueHandler;
import org.springframework.web.method.support.ModelAndViewContainer;

/**
 * Unit tests for {@link org.springframework.data.rest.webmvc.ResourceProcessorHandlerMethodReturnValueHandler}.
 * 
 * @author Oliver Gierke
 * @author Jon Brisbin
 */
@RunWith(MockitoJUnitRunner.class)
public class ResourceProcessorHandlerMethodReturnValueHandlerUnitTests {

	static final Resource<String> FOO = new Resource<String>("foo");
	static final Resources<Resource<String>> FOOS = new Resources<Resource<String>>(Collections.singletonList(FOO));
	static final PagedResources<Resource<String>> FOO_PAGE = new PagedResources<Resource<String>>(
			Collections.singleton(FOO), new PageMetadata(1, 0, 10));
	static final StringResource FOO_RES = new StringResource("foo");
	static final HttpEntity<Resource<String>> FOO_ENTITY = new HttpEntity<Resource<String>>(FOO);
	static final ResponseEntity<Resource<String>> FOO_RESP_ENTITY = new ResponseEntity<Resource<String>>(FOO,
			HttpStatus.OK);
	static final HttpEntity<StringResource> FOO_RES_ENTITY = new HttpEntity<StringResource>(FOO_RES);
	static final Resource<String> BAR = new Resource<String>("bar");
	static final Resources<Resource<String>> BARS = new Resources<Resource<String>>(Collections.singletonList(BAR));
	static final StringResource BAR_RES = new StringResource("bar");
	static final HttpEntity<Resource<String>> BAR_ENTITY = new HttpEntity<Resource<String>>(BAR);
	static final ResponseEntity<Resource<String>> BAR_RESP_ENTITY = new ResponseEntity<Resource<String>>(BAR,
			HttpStatus.OK);
	static final HttpEntity<StringResource> BAR_RES_ENTITY = new HttpEntity<StringResource>(BAR_RES);
	static final Resource<Long> LONG_10 = new Resource<Long>(10L);
	static final Resource<Long> LONG_20 = new Resource<Long>(20L);
	static final LongResource LONG_10_RES = new LongResource(10L);
	static final LongResource LONG_20_RES = new LongResource(20L);
	static final HttpEntity<Resource<Long>> LONG_10_ENTITY = new HttpEntity<Resource<Long>>(LONG_10);
	static final HttpEntity<LongResource> LONG_10_RES_ENTITY = new HttpEntity<LongResource>(LONG_10_RES);
	static final HttpEntity<Resource<Long>> LONG_20_ENTITY = new HttpEntity<Resource<Long>>(LONG_20);
	static final HttpEntity<LongResource> LONG_20_RES_ENTITY = new HttpEntity<LongResource>(LONG_20_RES);
	static final Map<String, MethodParameter> METHOD_PARAMS = new HashMap<String, MethodParameter>();

	static {
		doWithMethods(Controller.class, new MethodCallback() {
			@Override
			public void doWith(Method method) throws IllegalArgumentException, IllegalAccessException {
				METHOD_PARAMS.put(method.getName(), new MethodParameter(method, -1));
			}
		});
	}

	@Mock HandlerMethodReturnValueHandler delegate;
	List<ResourceProcessor<?>> resourceProcessors;

	@Before
	public void setUp() {
		resourceProcessors = new ArrayList<ResourceProcessor<?>>();
	}

	@Test
	public void supportsIfDelegateSupports() {
		assertSupport(true);
	}

	@Test
	public void doesNotSupportIfDelegateDoesNot() {
		assertSupport(false);
	}

	@Test
	public void postProcessesStringResource() throws Exception {
		resourceProcessors.add(StringResourceProcessor.INSTANCE);
		resourceProcessors.add(LongResourceProcessor.INSTANCE);

		invokeReturnValueHandler("stringResourceEntity", is(BAR), FOO);
	}

	@Test
	public void postProcessesStringResourceInResponseEntity() throws Exception {
		resourceProcessors.add(StringResourceProcessor.INSTANCE);
		resourceProcessors.add(LongResourceProcessor.INSTANCE);

		invokeReturnValueHandler("stringResourceEntity", httpEntity(BAR_RESP_ENTITY), FOO_RESP_ENTITY);
	}

	@Test
	public void postProcessesStringResourceInWildcardResponseEntity() throws Exception {
		resourceProcessors.add(StringResourceProcessor.INSTANCE);
		resourceProcessors.add(LongResourceProcessor.INSTANCE);

		invokeReturnValueHandler("resourceEntity", httpEntity(BAR_RESP_ENTITY), FOO_RESP_ENTITY);
	}

	@Test
	public void postProcessesStringResources() throws Exception {
		resourceProcessors.add(StringResourcesProcessor.INSTANCE);
		resourceProcessors.add(LongResourceProcessor.INSTANCE);

		invokeReturnValueHandler("resources", is(BARS), FOOS);
	}

	@Test
	public void postProcessesSpecializedStringResource() throws Exception {
		resourceProcessors.add(SpecializedStringResourceProcessor.INSTANCE);
		resourceProcessors.add(LongResourceProcessor.INSTANCE);

		invokeReturnValueHandler("stringResourceEntity", httpEntity(BAR_RES_ENTITY), FOO_RES_ENTITY);
	}

	@Test
	public void postProcessesSpecializedStringUsingStringResourceProcessor() throws Exception {
		resourceProcessors.add(StringResourceProcessor.INSTANCE);
		resourceProcessors.add(LongResourceProcessor.INSTANCE);

		invokeReturnValueHandler("specializedStringResourceEntity", httpEntity(BAR_ENTITY), FOO_RES_ENTITY);
	}

	@Test
	public void postProcessesLongResource() throws Exception {
		resourceProcessors.add(StringResourceProcessor.INSTANCE);
		resourceProcessors.add(LongResourceProcessor.INSTANCE);

		invokeReturnValueHandler("longResource", is(LONG_20), LONG_10);
	}

	@Test
	public void postProcessesSpecializedLongResource() throws Exception {
		resourceProcessors.add(StringResourceProcessor.INSTANCE);
		resourceProcessors.add(SpecializedLongResourceProcessor.INSTANCE);

		invokeReturnValueHandler("specializedLongResourceEntity", httpEntity(LONG_20_RES_ENTITY), LONG_10_RES_ENTITY);
	}

	@Test
	public void doesNotPostProcesseLongResourceWithSpecializedLongResourceProcessor() throws Exception {
		resourceProcessors.add(StringResourceProcessor.INSTANCE);
		resourceProcessors.add(SpecializedLongResourceProcessor.INSTANCE);

		invokeReturnValueHandler("numberResourceEntity", httpEntity(LONG_10_ENTITY), LONG_10_ENTITY);
	}

	@Test
	public void postProcessesSpecializedLongResourceUsingLongResourceProcessor() throws Exception {
		resourceProcessors.add(StringResourceProcessor.INSTANCE);
		resourceProcessors.add(LongResourceProcessor.INSTANCE);

		invokeReturnValueHandler("resourceEntity", is(LONG_20), LONG_10_RES);
	}

	@Test
	public void usesHeaderLinksResponseEntityIfConfigured() throws Exception {

		Resource<String> resource = new Resource<String>("foo", new Link("href", "rel"));
		MethodParameter parameter = METHOD_PARAMS.get("resource");

		ResourceProcessorHandlerMethodReturnValueHandler handler = new ResourceProcessorHandlerMethodReturnValueHandler(
				delegate, resourceProcessors);
		handler.setRootLinksAsHeaders(true);
		handler.handleReturnValue(resource, parameter, null, null);

		verify(delegate, times(1)).handleReturnValue(Mockito.any(HeaderLinksResponseEntity.class), eq(parameter),
				Mockito.any(ModelAndViewContainer.class), Mockito.any(NativeWebRequest.class));
	}

	/**
	 * @see DATAREST-331
	 */
	@Test
	public void resourcesProcessorMatchesValueSubTypes() {

		TypeInformation<?> type = ClassTypeInformation.from(PagedStringResources.class);
		assertThat(ResourcesProcessorWrapper.isValueTypeMatch(FOO_PAGE, type), is(true));
	}

	/**
	 * @see DATAREST-331
	 */
	@Test
	public void invokesProcessorsForProjection() throws Exception {

		ProjectionProcessor projectionProcessor = new ProjectionProcessor();
		resourceProcessors.add(projectionProcessor);

		ProxyProjectionFactory factory = new ProxyProjectionFactory(new DefaultListableBeanFactory());
		SampleProjection projection = factory.createProjection(new Sample(), SampleProjection.class);
		Resource<SampleProjection> resource = new Resource<SampleProjection>(projection);

		invokeReturnValueHandler("object", is(resource), resource);
		assertThat(projectionProcessor.invoked, is(true));
	}

	/**
	 * @see DATAREST-331
	 */
	@Test
	public void doesNotMatchOnNonMatchingResourcesTypes() throws Exception {

		Resource<Object> resource = new Resource<Object>(new Object());
		PagedResources<Resource<Object>> pagedResources = new PagedResources<Resource<Object>>(
				Collections.singleton(resource), new PageMetadata(1, 0, 10));

		TypeInformation<?> type = ClassTypeInformation.from(RepositoryLinksResource.class);
		assertThat(ResourcesProcessorWrapper.isValueTypeMatch(pagedResources, type), is(false));
	}

	// Helpers ---------------------------------------------------------//
	private void invokeReturnValueHandler(String method, final Matcher<?> matcher, Object returnValue) throws Exception {
		final MethodParameter methodParam = METHOD_PARAMS.get(method);

		if (methodParam == null) {
			throw new IllegalArgumentException("Invalid method!");
		}

		HandlerMethodReturnValueHandler handler = new ResourceProcessorHandlerMethodReturnValueHandler(delegate,
				resourceProcessors);
		handler.handleReturnValue(returnValue, methodParam, null, null);
	}

	private void assertSupport(boolean value) {

		final MethodParameter parameter = Mockito.mock(MethodParameter.class);
		when(delegate.supportsReturnType(Mockito.any(MethodParameter.class))).thenReturn(value);

		HandlerMethodReturnValueHandler handler = new ResourceProcessorHandlerMethodReturnValueHandler(delegate,
				resourceProcessors);

		assertThat(handler.supportsReturnType(parameter), is(value));
	}

	enum StringResourceProcessor implements ResourceProcessor<Resource<String>> {
		INSTANCE;

		@Override
		public Resource<String> process(Resource<String> resource) {
			return BAR;
		}
	}

	enum LongResourceProcessor implements ResourceProcessor<Resource<Long>> {
		INSTANCE;

		@Override
		public Resource<Long> process(Resource<Long> resource) {
			return LONG_20;
		}
	}

	enum StringResourcesProcessor implements ResourceProcessor<Resources<Resource<String>>> {
		INSTANCE;

		@Override
		public Resources<Resource<String>> process(Resources<Resource<String>> resource) {
			return BARS;
		}
	}

	enum SpecializedStringResourceProcessor implements ResourceProcessor<StringResource> {
		INSTANCE;

		@Override
		public StringResource process(StringResource resource) {
			return BAR_RES;
		}
	}

	enum SpecializedLongResourceProcessor implements ResourceProcessor<LongResource> {
		INSTANCE;

		@Override
		public LongResource process(LongResource resource) {
			return LONG_20_RES;
		}
	}

	static interface Controller {

		Resources<Resource<String>> resources();

		Resource<String> resource();

		Resource<Long> longResource();

		StringResource specializedResource();

		Object object();

		HttpEntity<Resource<?>> resourceEntity();

		HttpEntity<Resources<?>> resourcesEntity();

		HttpEntity<Object> objectEntity();

		HttpEntity<Resource<String>> stringResourceEntity();

		HttpEntity<Resource<? extends Number>> numberResourceEntity();

		HttpEntity<StringResource> specializedStringResourceEntity();

		HttpEntity<LongResource> specializedLongResourceEntity();

		ResponseEntity<Resource<?>> resourceResponseEntity();

		ResponseEntity<Resources<?>> resourcesResponseEntity();
	}

	static class StringResource extends Resource<String> {
		public StringResource(String value) {
			super(value);
		}
	}

	static class LongResource extends Resource<Long> {
		public LongResource(Long value) {
			super(value);
		}
	}

	static class PagedStringResources extends PagedResources<Resource<String>> {};

	static class Sample {

	}

	static interface SampleProjection {

	}

	static class ProjectionProcessor implements ResourceProcessor<Resource<SampleProjection>> {

		boolean invoked = false;

		@Override
		public Resource<SampleProjection> process(Resource<SampleProjection> resource) {

			this.invoked = true;
			return resource;
		}
	}
}
