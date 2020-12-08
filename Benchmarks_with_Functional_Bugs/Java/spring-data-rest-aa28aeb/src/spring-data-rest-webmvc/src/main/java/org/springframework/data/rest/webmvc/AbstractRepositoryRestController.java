/*
 * Copyright 2012-2014 the original author or authors.
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

import static org.springframework.data.rest.webmvc.ControllerUtils.*;

import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Locale;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.MessageSource;
import org.springframework.context.MessageSourceAware;
import org.springframework.context.support.MessageSourceAccessor;
import org.springframework.core.convert.ConversionFailedException;
import org.springframework.dao.DataIntegrityViolationException;
import org.springframework.dao.OptimisticLockingFailureException;
import org.springframework.data.domain.Page;
import org.springframework.data.rest.core.RepositoryConstraintViolationException;
import org.springframework.data.rest.core.mapping.ResourceMetadata;
import org.springframework.data.rest.webmvc.support.ExceptionMessage;
import org.springframework.data.rest.webmvc.support.RepositoryConstraintViolationExceptionMessage;
import org.springframework.data.web.PagedResourcesAssembler;
import org.springframework.hateoas.Link;
import org.springframework.hateoas.Resource;
import org.springframework.hateoas.Resources;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.http.converter.HttpMessageNotReadableException;
import org.springframework.util.Assert;
import org.springframework.web.HttpRequestMethodNotSupportedException;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseBody;

/**
 * @author Jon Brisbin
 * @author Oliver Gierke
 */
@SuppressWarnings({ "rawtypes" })
class AbstractRepositoryRestController implements MessageSourceAware {

	private static final Logger LOG = LoggerFactory.getLogger(AbstractRepositoryRestController.class);

	private final PagedResourcesAssembler<Object> pagedResourcesAssembler;
	private MessageSourceAccessor messageSourceAccessor;

	/**
	 * Creates a new {@link AbstractRepositoryRestController} for the given {@link PagedResourcesAssembler}.
	 * 
	 * @param pagedResourcesAssembler must not be {@literal null}.
	 */
	public AbstractRepositoryRestController(PagedResourcesAssembler<Object> pagedResourcesAssembler) {

		Assert.notNull(pagedResourcesAssembler, "PagedResourcesAssembler must not be null!");
		this.pagedResourcesAssembler = pagedResourcesAssembler;
	}

	/* 
	 * (non-Javadoc)
	 * @see org.springframework.context.MessageSourceAware#setMessageSource(org.springframework.context.MessageSource)
	 */
	@Override
	public void setMessageSource(MessageSource messageSource) {
		this.messageSourceAccessor = new MessageSourceAccessor(messageSource);
	}

	@ExceptionHandler({ NullPointerException.class })
	@ResponseBody
	public ResponseEntity<?> handleNPE(NullPointerException npe) {
		return errorResponse(npe, HttpStatus.INTERNAL_SERVER_ERROR);
	}

	@ExceptionHandler({ ResourceNotFoundException.class })
	@ResponseBody
	public ResponseEntity<?> handleNotFound() {
		return notFound();
	}

	@ExceptionHandler({ HttpMessageNotReadableException.class })
	@ResponseBody
	public ResponseEntity<ExceptionMessage> handleNotReadable(HttpMessageNotReadableException e) {
		return badRequest(e);
	}

	/**
	 * Handle failures commonly thrown from code tries to read incoming data and convert or cast it to the right type.
	 * 
	 * @param t
	 * @return
	 */
	@ExceptionHandler({ InvocationTargetException.class, IllegalArgumentException.class, ClassCastException.class,
			ConversionFailedException.class })
	@ResponseBody
	public ResponseEntity handleMiscFailures(Throwable t) {
		if (null != t.getCause() && t.getCause() instanceof ResourceNotFoundException) {
			return notFound();
		}
		return badRequest(t);
	}

	@ExceptionHandler({ RepositoryConstraintViolationException.class })
	@ResponseBody
	public ResponseEntity handleRepositoryConstraintViolationException(Locale locale,
			RepositoryConstraintViolationException rcve) {

		return response(null, new RepositoryConstraintViolationExceptionMessage(rcve, messageSourceAccessor),
				HttpStatus.BAD_REQUEST);
	}

	/**
	 * Send a 409 Conflict in case of concurrent modification.
	 * 
	 * @param ex
	 * @return
	 */
	@ExceptionHandler({ OptimisticLockingFailureException.class, DataIntegrityViolationException.class })
	@ResponseBody
	public ResponseEntity handleConflict(Exception ex) {
		return errorResponse(null, ex, HttpStatus.CONFLICT);
	}

	/**
	 * Send {@code 405 Method Not Allowed} and include the supported {@link HttpMethod}s in the {@code Allow} header.
	 * 
	 * @param o_O
	 * @return
	 */
	@ExceptionHandler
	@ResponseBody
	public ResponseEntity<Void> handle(HttpRequestMethodNotSupportedException o_O) {

		HttpHeaders headers = new HttpHeaders();
		headers.setAllow(o_O.getSupportedHttpMethods());

		return new ResponseEntity<Void>(headers, HttpStatus.METHOD_NOT_ALLOWED);
	}

	protected <T> ResponseEntity<T> notFound() {
		return notFound(null, null);
	}

	protected <T> ResponseEntity<T> notFound(HttpHeaders headers, T body) {
		return response(headers, body, HttpStatus.NOT_FOUND);
	}

	protected <T extends Throwable> ResponseEntity<ExceptionMessage> badRequest(T throwable) {
		return badRequest(null, throwable);
	}

	protected <T extends Throwable> ResponseEntity<ExceptionMessage> badRequest(HttpHeaders headers, T throwable) {
		return errorResponse(headers, throwable, HttpStatus.BAD_REQUEST);
	}

	public <T extends Throwable> ResponseEntity<ExceptionMessage> errorResponse(T throwable, HttpStatus status) {
		return errorResponse(null, throwable, status);
	}

	public <T extends Throwable> ResponseEntity<ExceptionMessage> errorResponse(HttpHeaders headers, T throwable,
			HttpStatus status) {
		if (null != throwable && null != throwable.getMessage()) {
			LOG.error(throwable.getMessage(), throwable);
			return response(headers, new ExceptionMessage(throwable), status);
		} else {
			return response(headers, null, status);
		}
	}

	public <T> ResponseEntity<T> response(HttpHeaders headers, T body, HttpStatus status) {
		HttpHeaders hdrs = new HttpHeaders();
		if (null != headers) {
			hdrs.putAll(headers);
		}
		return new ResponseEntity<T>(body, hdrs, status);
	}

	protected Link resourceLink(RootResourceInformation resourceLink, Resource resource) {

		ResourceMetadata repoMapping = resourceLink.getResourceMetadata();

		Link selfLink = resource.getLink("self");
		String rel = repoMapping.getItemResourceRel();

		return new Link(selfLink.getHref(), rel);
	}

	@SuppressWarnings({ "unchecked" })
	protected Resources resultToResources(Object result, PersistentEntityResourceAssembler assembler, Link baseLink) {

		if (result instanceof Page) {
			Page<Object> page = (Page<Object>) result;
			return entitiesToResources(page, assembler, baseLink);
		} else if (result instanceof Iterable) {
			return entitiesToResources((Iterable<Object>) result, assembler);
		} else if (null == result) {
			return new Resources(EMPTY_RESOURCE_LIST);
		} else {
			Resource<Object> resource = assembler.toFullResource(result);
			return new Resources(Collections.singletonList(resource));
		}
	}

	protected Resources<? extends Resource<Object>> entitiesToResources(Page<Object> page,
			PersistentEntityResourceAssembler assembler, Link baseLink) {
		return baseLink == null ? pagedResourcesAssembler.toResource(page, assembler) : pagedResourcesAssembler.toResource(
				page, assembler, baseLink);
	}

	protected Resources<Resource<Object>> entitiesToResources(Iterable<Object> entities,
			PersistentEntityResourceAssembler assembler) {

		List<Resource<Object>> resources = new ArrayList<Resource<Object>>();

		for (Object obj : entities) {
			resources.add(obj == null ? null : assembler.toResource(obj));
		}

		return new Resources<Resource<Object>>(resources);
	}
}
