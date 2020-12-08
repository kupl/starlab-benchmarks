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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import junit.framework.Test;
import junit.framework.TestSuite;

import org.apache.commons.collections.Unmodifiable;

/**
 * Tests the UnmodifiableIterator.
 * 
 * @version $Revision: 1.3 $ $Date: 2004/02/18 01:20:33 $
 * 
 * @author Stephen Colebourne
 */
public class TestUnmodifiableIterator extends AbstractTestIterator {

    protected String[] testArray = { "One", "Two", "Three" };
    protected List testList = new ArrayList(Arrays.asList(testArray));

    public static Test suite() {
        return new TestSuite(TestUnmodifiableIterator.class);
    }

    public TestUnmodifiableIterator(String testName) {
        super(testName);
    }

    public Iterator makeEmptyIterator() {
        return UnmodifiableIterator.decorate(Collections.EMPTY_LIST.iterator());
    }

    public Iterator makeFullIterator() {
        return UnmodifiableIterator.decorate(testList.iterator());
    }

    public boolean supportsRemove() {
        return false;
    }

    //-----------------------------------------------------------------------
    public void testIterator() {
        assertTrue(makeEmptyIterator() instanceof Unmodifiable);
    }
    
    public void testDecorateFactory() {
        Iterator it = makeFullIterator();
        assertSame(it, UnmodifiableIterator.decorate(it));
        
        it = testList.iterator();
        assertTrue(it != UnmodifiableIterator.decorate(it));
        
        try {
            UnmodifiableIterator.decorate(null);
            fail();
        } catch (IllegalArgumentException ex) {}
    }

}
