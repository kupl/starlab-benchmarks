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
package org.apache.commons.collections.comparators;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;

import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * Tests for ReverseComparator.
 * 
 * @version $Revision: 1.9 $ $Date: 2004/02/18 01:20:34 $
 * 
 * @author Unknown
 */
public class TestReverseComparator extends AbstractTestComparator {

    public TestReverseComparator(String testName) {
        super(testName);
    }

    public static Test suite() {
        return new TestSuite(TestReverseComparator.class);
    }

    /**
     * For the purposes of this test, return a 
     * ReverseComparator that wraps the java.util.Collections.reverseOrder()
     * Comparator.  The resulting comparator should
     * sort according to natural Order.  (Note: we wrap
     * a Comparator taken from the JDK so that we can
     * save a "canonical" form in CVS.
     * 
     * @return Comparator that returns "natural" order
     */
    public Comparator makeComparator() {
        return new ReverseComparator(Collections.reverseOrder());
    }

    public List getComparableObjectsOrdered() {
        List list = new LinkedList();
        list.add(new Integer(1));
        list.add(new Integer(2));
        list.add(new Integer(3));
        list.add(new Integer(4));
        list.add(new Integer(5));
        return list;
    }

    /** 
     * Override this inherited test since Collections.reverseOrder
     * doesn't adhere to the "soft" Comparator contract, and we've
     * already "cannonized" the comparator returned by makeComparator.
     */
    public void testSerializeDeserializeThenCompare() throws Exception {
        Comparator comp = new ReverseComparator(new ComparableComparator());

        ByteArrayOutputStream buffer = new ByteArrayOutputStream();
        ObjectOutputStream out = new ObjectOutputStream(buffer);
        out.writeObject(comp);
        out.close();
            
        ObjectInputStream in = new ObjectInputStream(new ByteArrayInputStream(buffer.toByteArray()));
        Object dest = in.readObject();
        in.close();
        assertEquals("obj != deserialize(serialize(obj))",comp,dest);
    }

}
