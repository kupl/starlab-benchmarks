/*
 *  Copyright 2004 The Apache Software Foundation
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
package org.apache.commons.collections.collection;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;

import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * Extension of {@link AbstractTestCollection} for exercising the 
 * {@link SynchronizedCollection} implementation.
 *
 * @since Commons Collections 3.1
 * @version $Revision: 1.1 $ $Date: 2004/06/01 23:07:12 $
 * 
 * @author Phil Steitz
 * @author Stephen Colebourne
 */
public class TestSynchronizedCollection extends AbstractTestCollection {
    
    public TestSynchronizedCollection(String testName) {
        super(testName);
    }
    
    public static Test suite() {
        return new TestSuite(TestSynchronizedCollection.class);
    }
    
    public static void main(String args[]) {
        String[] testCaseName = { TestSynchronizedCollection.class.getName()};
        junit.textui.TestRunner.main(testCaseName);
    }

    //-----------------------------------------------------------------------    
    public Collection makeCollection() {
        return SynchronizedCollection.decorate(new ArrayList());
    }
    
    public Collection makeConfirmedCollection() {
        ArrayList list = new ArrayList();
        return list;
    }

    public Collection makeConfirmedFullCollection() {
        ArrayList list = new ArrayList();
        list.addAll(Arrays.asList(getFullElements()));
        return list;
    }

    public String getCompatibilityVersion() {
        return "3.1";
    }

//    public void testCreate() throws Exception {
//        resetEmpty();
//        writeExternalFormToDisk((java.io.Serializable) collection, "D:/dev/collections/data/test/SynchronizedCollection.emptyCollection.version3.1.obj");
//        resetFull();
//        writeExternalFormToDisk((java.io.Serializable) collection, "D:/dev/collections/data/test/SynchronizedCollection.fullCollection.version3.1.obj");
//    }

}
