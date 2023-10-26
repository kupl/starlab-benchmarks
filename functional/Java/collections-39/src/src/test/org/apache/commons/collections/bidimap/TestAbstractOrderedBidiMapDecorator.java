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
package org.apache.commons.collections.bidimap;

import java.util.Map;
import java.util.TreeMap;

import junit.framework.Test;
import junit.framework.TestSuite;

import org.apache.commons.collections.BidiMap;
import org.apache.commons.collections.OrderedBidiMap;

/**
 * Test class for AbstractOrderedBidiMapDecorator.
 * 
 * @version $Revision: 1.3 $ $Date: 2004/02/18 01:20:39 $
 */
public class TestAbstractOrderedBidiMapDecorator
        extends AbstractTestOrderedBidiMap {

    public TestAbstractOrderedBidiMapDecorator(String testName) {
        super(testName);
    }

    public static Test suite() {
        return new TestSuite(TestAbstractOrderedBidiMapDecorator.class);
    }

    public BidiMap makeEmptyBidiMap() {
        return new TestOrderedBidiMap();
    }

    public Map makeConfirmedMap() {
        return new TreeMap();
    }

    public boolean isAllowNullKey() {
        return false;
    }

    public boolean isAllowNullValue() {
        return false;
    }

    public boolean isSetValueSupported() {
        return true;
    }

    /**
     * Simple class to actually test.
     */
    private static final class TestOrderedBidiMap extends AbstractOrderedBidiMapDecorator {
            
        private TestOrderedBidiMap inverse = null;

        public TestOrderedBidiMap() {
            super(new DualTreeBidiMap());
        }
        
        public TestOrderedBidiMap(OrderedBidiMap map) {
            super(map);
        }
        
        public BidiMap inverseBidiMap() {
            if (inverse == null) {
                inverse = new TestOrderedBidiMap((OrderedBidiMap) getBidiMap().inverseBidiMap());
                inverse.inverse = this;
            }
            return inverse;
        }
    }
}
