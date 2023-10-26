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
import junit.textui.TestRunner;

import org.apache.commons.collections.BidiMap;
import org.apache.commons.collections.BulkTest;
import org.apache.commons.collections.SortedBidiMap;

/**
 * JUnit tests.
 * 
 * @version $Revision: 1.4 $ $Date: 2004/04/09 15:15:18 $
 * 
 * @author Stephen Colebourne
 */
public class TestUnmodifiableSortedBidiMap extends AbstractTestSortedBidiMap {

    public static void main(String[] args) {
        TestRunner.run(suite());
    }
    
    public static Test suite() {
        return BulkTest.makeSuite(TestUnmodifiableSortedBidiMap.class);
    }

    public TestUnmodifiableSortedBidiMap(String testName) {
        super(testName);
    }

    //-----------------------------------------------------------------------
    public BidiMap makeEmptyBidiMap() {
        return UnmodifiableSortedBidiMap.decorate(new DualTreeBidiMap());
    }
    public BidiMap makeFullBidiMap() {
        SortedBidiMap bidi = new DualTreeBidiMap();
        for (int i = 0; i < entries.length; i++) {
            bidi.put(entries[i][0], entries[i][1]);
        }
        return UnmodifiableSortedBidiMap.decorate(bidi);
    }
    public Map makeFullMap() {
        SortedBidiMap bidi = new DualTreeBidiMap();
        addSampleMappings(bidi);
        return UnmodifiableSortedBidiMap.decorate(bidi);
    }
    
    public Map makeConfirmedMap() {
        return new TreeMap();
    }

    public boolean isSubMapViewsSerializable() {
        // TreeMap sub map views have a bug in deserialization.
        return false;
    }
    public String[] ignoredTests() {
        // Override to prevent infinite recursion of tests.
        return new String[] {"TestUnmodifiableSortedBidiMap.bulkTestInverseMap.bulkTestInverseMap"};
    }

    //-----------------------------------------------------------------------
    public boolean isAllowNullKey() {
        return false;
    }
    public boolean isAllowNullValue() {
        return false;
    }
    public boolean isPutAddSupported() {
        return false;
    }
    public boolean isPutChangeSupported() {
        return false;
    }
    public boolean isRemoveSupported() {
        return false;
    }
    
}
