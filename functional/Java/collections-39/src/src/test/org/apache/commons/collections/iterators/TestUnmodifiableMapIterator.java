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

import java.util.HashMap;
import java.util.Map;

import junit.framework.Test;
import junit.framework.TestSuite;

import org.apache.commons.collections.BidiMap;
import org.apache.commons.collections.MapIterator;
import org.apache.commons.collections.Unmodifiable;
import org.apache.commons.collections.bidimap.DualHashBidiMap;

/**
 * Tests the UnmodifiableMapIterator.
 * 
 * @version $Revision: 1.8 $ $Date: 2004/02/18 01:20:33 $
 * 
 * @author Stephen Colebourne
 */
public class TestUnmodifiableMapIterator extends AbstractTestMapIterator {

    public static Test suite() {
        return new TestSuite(TestUnmodifiableMapIterator.class);
    }

    public TestUnmodifiableMapIterator(String testName) {
        super(testName);
    }

    public MapIterator makeEmptyMapIterator() {
        return UnmodifiableMapIterator.decorate(new DualHashBidiMap().mapIterator());
    }

    public MapIterator makeFullMapIterator() {
        return UnmodifiableMapIterator.decorate(((BidiMap) getMap()).mapIterator());
    }
    
    public Map getMap() {
        Map testMap = new DualHashBidiMap();
        testMap.put("A", "a");
        testMap.put("B", "b");
        testMap.put("C", "c");
        return testMap;
    }

    public Map getConfirmedMap() {
        Map testMap = new HashMap();
        testMap.put("A", "a");
        testMap.put("B", "b");
        testMap.put("C", "c");
        return testMap;
    }

    public boolean supportsRemove() {
        return false;
    }

    public boolean supportsSetValue() {
        return false;
    }
    
    //-----------------------------------------------------------------------
    public void testMapIterator() {
        assertTrue(makeEmptyMapIterator() instanceof Unmodifiable);
    }
    
    public void testDecorateFactory() {
        MapIterator it = makeFullMapIterator();
        assertSame(it, UnmodifiableMapIterator.decorate(it));
        
        it = ((BidiMap) getMap()).mapIterator() ;
        assertTrue(it != UnmodifiableMapIterator.decorate(it));
        
        try {
            UnmodifiableMapIterator.decorate(null);
            fail();
        } catch (IllegalArgumentException ex) {}
    }

}
