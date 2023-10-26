/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.commons.collections.iterators;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Random;

import junit.framework.TestCase;
import org.apache.commons.collections.Predicate;
import org.apache.commons.collections.PredicateUtils;
import org.apache.commons.collections.list.GrowthList;
import org.junit.Assert;

/**
 * Tests the FilterListIterator class.
 *
 * @version $Revision: 1076034 $ $Date: 2011-03-01 23:10:10 +0100 (mar., 01 mars 2011) $
 *
 * @author Rodney Waldhoff
 */
@SuppressWarnings("boxing")
public class TestFilterListIterator extends TestCase {
    public TestFilterListIterator(String testName) {
        super(testName);
    }

    private ArrayList<Integer> list = null;
    private ArrayList<Integer> odds = null;
    private ArrayList<Integer> evens = null;
    private ArrayList<Integer> threes = null;
    private ArrayList<Integer> fours = null;
    private ArrayList<Integer> sixes = null;
    private Random random = new Random();

    @Override
    public void setUp() {
        list = new ArrayList<Integer>();
        odds = new ArrayList<Integer>();
        evens = new ArrayList<Integer>();
        threes = new ArrayList<Integer>();
        fours = new ArrayList<Integer>();
        sixes = new ArrayList<Integer>();
        for (int i = 0; i < 20; i++) {
            list.add(new Integer(i));
            if (i % 2 == 0) { evens.add(new Integer(i)); }
            if (i % 2 == 1) { odds.add(new Integer(i)); }
            if (i % 3 == 0) { threes.add(new Integer(i)); }
            if (i % 4 == 0) { fours.add(new Integer(i)); }
            if (i % 6 == 0) { sixes.add(new Integer(i)); }
        }

    }

    @Override
    public void tearDown() throws Exception {
        list = null;
        odds = null;
        evens = null;
        threes = null;
        fours = null;
        sixes = null;
    }
    /**
     * Test for {@link https://issues.apache.org/jira/browse/COLLECTIONS-360 COLLECTIONS-360}.
     */
    public void testCollections360() throws Throwable {
        Collection<Predicate<Object>> var7 = new GrowthList<Predicate<Object>>();
        Predicate<Object> var9 = PredicateUtils.anyPredicate(var7);
        FilterListIterator<Object> var13 = new FilterListIterator<Object>(var9);
        Assert.assertFalse(var13.hasNext());
        FilterListIterator<Object> var14 = new FilterListIterator<Object>(var9);
        // NPEX: single error
        // Assert.assertFalse(var14.hasPrevious());
    }

}
