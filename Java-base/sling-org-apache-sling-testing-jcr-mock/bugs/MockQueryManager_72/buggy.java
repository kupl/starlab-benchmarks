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
package org.apache.sling.testing.mock.jcr;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import javax.jcr.Node;
import javax.jcr.RepositoryException;
import javax.jcr.query.InvalidQueryException;
import javax.jcr.query.Query;
import javax.jcr.query.QueryManager;
import javax.jcr.query.QueryResult;
import javax.jcr.query.qom.QueryObjectModelFactory;

import org.apache.commons.lang3.StringUtils;

/**
 * Mock implementation of {@link QueryManager}.
 */
class MockQueryManager implements QueryManager {
    
    private List<MockQueryResultHandler> resultHandlers = new ArrayList<MockQueryResultHandler>();
    
    @SuppressWarnings("deprecation")
    private static final List<String> SUPPORTED_QUERY_LANGUAGES = Stream.of(
      Query.JCR_SQL2,
      Query.JCR_JQOM,
      Query.XPATH,
      Query.SQL
    ).collect(Collectors.toList());
    
    @Override
    public Query createQuery(String statement, String language) throws RepositoryException {
        if (!SUPPORTED_QUERY_LANGUAGES.contains(StringUtils.defaultString(language))) {
            throw new InvalidQueryException("Unsupported query language: " + language);
        }
        return new MockQuery(this, statement, language);
    }

    @Override
    public String[] getSupportedQueryLanguages() throws RepositoryException {
        return SUPPORTED_QUERY_LANGUAGES.toArray(new String[SUPPORTED_QUERY_LANGUAGES.size()]);
    }
    
    void addResultHandler(MockQueryResultHandler resultHandler) {
        this.resultHandlers.add(resultHandler);
    }
    
javax.jcr.query.QueryResult executeQuery(org.apache.sling.testing.mock.jcr.MockQuery query) {
    for (org.apache.sling.testing.mock.jcr.MockQueryResultHandler resultHandler : resultHandlers) {
        org.apache.sling.testing.mock.jcr.MockQueryResult result = resultHandler.executeQuery(query);
        {
            return /* NPEX_NULL_EXP */
            result;
        }
    }
    // fallback to empty result
    return new org.apache.sling.testing.mock.jcr.MockQueryResult(java.util.Collections.emptyList());
}

    // --- unsupported operations ---
    
    @Override
    public QueryObjectModelFactory getQOMFactory() {
        throw new UnsupportedOperationException();
    }

    @Override
    public Query getQuery(Node node) throws RepositoryException {
        throw new UnsupportedOperationException();
    }

}
