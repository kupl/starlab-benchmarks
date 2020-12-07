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
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

import org.apache.commons.collections.comparators.ComparableComparator;

/**
 * Unit test suite for {@link CollatingIterator}.
 *
 * @version $Revision: 1079587 $ $Date: 2011-03-08 23:31:49 +0100 (mar., 08 mars 2011) $
 * @author Rodney Waldhoff
 */
@SuppressWarnings("boxing")
public class TestCollatingIterator extends AbstractTestIterator<Integer> {

    //------------------------------------------------------------ Conventional

    public TestCollatingIterator(String testName) {
        super(testName);
    }

    //--------------------------------------------------------------- Lifecycle

    private Comparator<Integer> comparator = null;
    private ArrayList<Integer> evens = null;
    private ArrayList<Integer> odds = null;
    private ArrayList<Integer> fib = null;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        comparator = new ComparableComparator<Integer>();
        evens = new ArrayList<Integer>();
        odds = new ArrayList<Integer>();
        for (int i = 0; i < 20; i++) {
            if (0 == i % 2) {
                evens.add(i);
            } else {
                odds.add(i);
            }
        }
        fib = new ArrayList<Integer>();
        fib.add(1);
        fib.add(1);
        fib.add(2);
        fib.add(3);
        fib.add(5);
        fib.add(8);
        fib.add(13);
        fib.add(21);
    }

    //---------------------------------------------------- TestIterator Methods

    @Override
    public org.apache.commons.collections.iterators.CollatingIterator<Integer> makeEmptyIterator() {
        return new org.apache.commons.collections.iterators.CollatingIterator<Integer>(comparator);
    }

    @Override
    public org.apache.commons.collections.iterators.CollatingIterator<Integer> makeObject() {
        org.apache.commons.collections.iterators.CollatingIterator<Integer> iter = new org.apache.commons.collections.iterators.CollatingIterator<Integer>(comparator);
        iter.addIterator(evens.iterator());
        iter.addIterator(odds.iterator());
        iter.addIterator(fib.iterator());
        return iter;
    }

    //------------------------------------------------------------------- Tests

    public void testNullComparator() {
       List<Integer> l1 = Arrays.asList(1, 3, 5);
       List<Integer> l2 = Arrays.asList(2, 4, 6);

       org.apache.commons.collections.iterators.CollatingIterator<Integer> collatingIterator1 = new org.apache.commons.collections.iterators.CollatingIterator<Integer>(null, l1.iterator(), l2.iterator());
       try {
           collatingIterator1.next();
       } catch (NullPointerException e) {
           assertTrue(e.getMessage().startsWith("You must invoke setComparator"));
       }

       int i = 0;
       org.apache.commons.collections.iterators.CollatingIterator<Integer> collatingIterator2 = new org.apache.commons.collections.iterators.CollatingIterator<Integer>(null, l1.iterator(), l2.iterator());
       collatingIterator2.setComparator(new ComparableComparator<Integer>());
       for ( ; collatingIterator2.hasNext(); i++ ) {
          Integer n = (Integer)collatingIterator2.next();
          assertEquals("wrong order", (int)n, i + 1);
       }
       assertEquals("wrong size", i, l1.size() + l2.size());
    }
}

