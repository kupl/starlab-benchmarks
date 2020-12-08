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
package org.apache.commons.collections.bag;

import junit.framework.Test;
import junit.framework.TestSuite;

import org.apache.commons.collections.Bag;

/**
 * Extension of {@link TestBag} for exercising the {@link HashBag}
 * implementation.
 * 
 * @version $Revision: 1.4 $ $Date: 2004/02/18 01:20:39 $
 *
 * @author Chuck Burdick
 */
public class TestHashBag extends AbstractTestBag {
    
    public TestHashBag(String testName) {
        super(testName);
    }

    public static Test suite() {
        return new TestSuite(TestHashBag.class);
    }

    public static void main(String args[]) {
        String[] testCaseName = { TestHashBag.class.getName()};
        junit.textui.TestRunner.main(testCaseName);
    }

    public Bag makeBag() {
        return new HashBag();
    }
    
    public String getCompatibilityVersion() {
        return "3";
    }
    
//    public void testCreate() throws Exception {
//        Bag bag = makeBag();
//        writeExternalFormToDisk((Serializable) bag, "D:/dev/collections/data/test/HashBag.emptyCollection.version3.obj");
//        bag = makeBag();
//        bag.add("A");
//        bag.add("A");
//        bag.add("B");
//        bag.add("B");
//        bag.add("C");
//        writeExternalFormToDisk((Serializable) bag, "D:/dev/collections/data/test/HashBag.fullCollection.version3.obj");
//    }
}
