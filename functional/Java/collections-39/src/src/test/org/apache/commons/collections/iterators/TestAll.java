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

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Entry point for all iterator tests.
 * 
 * @version $Revision: 1.16 $ $Date: 2004/07/17 21:02:47 $
 * 
 * @author Rodney Waldhoff
 */
public class TestAll extends TestCase {
    
    public TestAll(String testName) {
        super(testName);
    }

    public static Test suite() {
        TestSuite suite = new TestSuite();
        suite.addTest(TestArrayIterator.suite());
        suite.addTest(TestArrayIterator2.suite());
        suite.addTest(TestArrayListIterator.suite());
        suite.addTest(TestArrayListIterator2.suite());
        suite.addTest(TestObjectArrayIterator.suite());
        suite.addTest(TestObjectArrayListIterator.suite());
        suite.addTest(TestObjectArrayListIterator2.suite());
        suite.addTest(TestCollatingIterator.suite());
        suite.addTest(TestFilterIterator.suite());
        suite.addTest(TestFilterListIterator.suite());
        suite.addTest(TestIteratorChain.suite());
        suite.addTest(TestListIteratorWrapper.suite());
        suite.addTest(TestLoopingIterator.suite());
        suite.addTest(TestLoopingListIterator.suite());
        suite.addTest(TestSingletonIterator.suite());
        suite.addTest(TestSingletonIterator2.suite());
        suite.addTest(TestSingletonListIterator.suite());
        suite.addTest(TestObjectGraphIterator.suite());
        suite.addTest(TestUniqueFilterIterator.suite());
        suite.addTest(TestUnmodifiableIterator.suite());
        suite.addTest(TestUnmodifiableListIterator.suite());
        suite.addTest(TestUnmodifiableMapIterator.suite());
        suite.addTest(TestUnmodifiableOrderedMapIterator.suite());
        return suite;
    }
        
    public static void main(String args[]) {
        String[] testCaseName = { TestAll.class.getName() };
        junit.textui.TestRunner.main(testCaseName);
    }
}
