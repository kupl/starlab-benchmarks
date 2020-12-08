/*
 *  Copyright 2003-2004 The Apache Software Foundation
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
package org.apache.commons.collections.map;

import java.util.HashMap;
import java.util.Map;

import junit.framework.Test;
import junit.framework.TestSuite;

import org.apache.commons.collections.OrderedMap;
import org.apache.commons.collections.Unmodifiable;

/**
 * Extension of {@link AbstractTestOrderedMap} for exercising the 
 * {@link UnmodifiableOrderedMap} implementation.
 *
 * @since Commons Collections 3.0
 * @version $Revision: 1.5 $ $Date: 2004/04/09 10:46:32 $
 * 
 * @author Stephen Colebourne
 */
public class TestUnmodifiableOrderedMap extends AbstractTestOrderedMap {
    
    public TestUnmodifiableOrderedMap(String testName) {
        super(testName);
    }
    
    public static Test suite() {
        return new TestSuite(TestUnmodifiableOrderedMap.class);
    }
    
    public static void main(String args[]) {
        String[] testCaseName = { TestUnmodifiableOrderedMap.class.getName()};
        junit.textui.TestRunner.main(testCaseName);
    }
    
    //-------------------------------------------------------------------
    
    public Map makeEmptyMap() {
        return UnmodifiableOrderedMap.decorate(ListOrderedMap.decorate(new HashMap()));
    }
    
    public boolean isPutChangeSupported() {
        return false;
    }
    
    public boolean isPutAddSupported() {
        return false;
    }
    
    public boolean isRemoveSupported() {
        return false;
    }
    
    public Map makeFullMap() {
        OrderedMap m = ListOrderedMap.decorate(new HashMap());
        addSampleMappings(m);
        return UnmodifiableOrderedMap.decorate(m);
    }
    
    //-----------------------------------------------------------------------
    public void testUnmodifiable() {
        assertTrue(makeEmptyMap() instanceof Unmodifiable);
        assertTrue(makeFullMap() instanceof Unmodifiable);
    }
    
    public void testDecorateFactory() {
        Map map = makeFullMap();
        assertSame(map, UnmodifiableOrderedMap.decorate((OrderedMap) map));
        
        try {
            UnmodifiableOrderedMap.decorate(null);
            fail();
        } catch (IllegalArgumentException ex) {}
    }

    public String getCompatibilityVersion() {
        return "3.1";
    }

//    public void testCreate() throws Exception {
//        resetEmpty();
//        writeExternalFormToDisk(
//            (java.io.Serializable) map,
//            "D:/dev/collections/data/test/UnmodifiableOrderedMap.emptyCollection.version3.1.obj");
//        resetFull();
//        writeExternalFormToDisk(
//            (java.io.Serializable) map,
//            "D:/dev/collections/data/test/UnmodifiableOrderedMap.fullCollection.version3.1.obj");
//    }
}