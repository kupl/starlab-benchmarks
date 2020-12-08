/*
 *  Copyright 2001-2004 The Apache Software Foundation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package org.apache.commons.collections.iterators;

import java.util.Iterator;
import java.util.NoSuchElementException;

import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * Tests the ArrayIterator to ensure that the next() method will actually
 * perform the iteration rather than the hasNext() method.
 * The code of this test was supplied by Mauricio S. Moura.
 * 
 * @version $Revision: 1.7 $ $Date: 2004/02/18 01:20:33 $
 * 
 * @author James Strachan
 * @author Mauricio S. Moura
 * @author Morgan Delagrange
 * @author Stephen Colebourne
 */
public class TestArrayIterator extends AbstractTestIterator {

    protected String[] testArray = { "One", "Two", "Three" };

    public static Test suite() {
        return new TestSuite(TestArrayIterator.class);
    }

    public TestArrayIterator(String testName) {
        super(testName);
    }

    public Iterator makeEmptyIterator() {
        return new ArrayIterator(new Object[0]);
    }

    public Iterator makeFullIterator() {
        return new ArrayIterator(testArray);
    }

    public boolean supportsRemove() {
        return false;
    }


    public void testIterator() {
        Iterator iter = (Iterator) makeFullIterator();
        for (int i = 0; i < testArray.length; i++) {
            Object testValue = testArray[i];
            Object iterValue = iter.next();

            assertEquals("Iteration value is correct", testValue, iterValue);
        }

        assertTrue("Iterator should now be empty", !iter.hasNext());

        try {
            Object testValue = iter.next();
        } catch (Exception e) {
            assertTrue(
                "NoSuchElementException must be thrown",
                e.getClass().equals((new NoSuchElementException()).getClass()));
        }
    }

    public void testNullArray() {
        try {
            Iterator iter = new ArrayIterator(null);

            fail("Constructor should throw a NullPointerException when constructed with a null array");
        } catch (NullPointerException e) {
            // expected
        }

        ArrayIterator iter = new ArrayIterator();
        try {
            iter.setArray(null);

            fail("setArray(null) should throw a NullPointerException");
        } catch (NullPointerException e) {
            // expected
        }
    }
    
    public void testReset() {
        ArrayIterator it = (ArrayIterator) makeFullIterator();
        it.next();
        it.reset();
        assertEquals("One", it.next());
    }

}
