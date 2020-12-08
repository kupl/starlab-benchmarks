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

import java.util.ListIterator;

import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * Tests the ObjectArrayListIterator class.
 * 
 * @version $Revision: 1.3 $ $Date: 2004/02/18 01:20:33 $
 * 
 * @author Stephen Colebourne
 */
public class TestObjectArrayListIterator2 extends AbstractTestListIterator {

    protected String[] testArray = { "One", "Two", "Three" };
    
    public TestObjectArrayListIterator2(String testName) {
        super(testName);
    }

    public static Test suite() {
        return new TestSuite(TestObjectArrayListIterator2.class);
    }

    public ListIterator makeEmptyListIterator() {
        return new ObjectArrayListIterator(new Object[0]);
    }

    public ListIterator makeFullListIterator() {
        return new ObjectArrayListIterator(testArray);
    }

    public ListIterator makeArrayListIterator(Object[] array) {
        return new ObjectArrayListIterator(array);
    }

    public boolean supportsAdd() {
        return false;
    }
    
    public boolean supportsRemove() {
        return false;
    }

}
