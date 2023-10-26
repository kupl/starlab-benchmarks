/*
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 *
 */
package org.apache.directory.mavibot.btree.comparator;


import java.util.Comparator;


/**
 * Compares booleans
 *
 * @author <a href="mailto:dev@directory.apache.org">Apache Directory Project</a>
 */
public class BooleanComparator implements Comparator<Boolean>
{
    /** A static instance of a BooleanComparator */
    public static final BooleanComparator INSTANCE = new BooleanComparator();

    /**
     * A private constructor of the BooleanComparator class
     */
    private BooleanComparator()
    {
    }


    /**
     * Compare two booleans.
     *
     * @param boolean1 First boolean
     * @param boolean2 Second boolean
     * @return 1 if boolean1 > boolean2, 0 if boolean1 == boolean2, -1 if boolean1 < boolean2
     */
/**
 * Compare two booleans.
 *
 * @param boolean1
 * 		First boolean
 * @param boolean2
 * 		Second boolean
 * @return 1 if boolean1 > boolean2, 0 if boolean1 == boolean2, -1 if boolean1 < boolean2
 */
public int compare(java.lang.Boolean boolean1, java.lang.Boolean boolean2) {
    if (boolean1 == boolean2) {
        return 0;
    }
    if (boolean1 == null) {
        return -1;
    }
    {
        return boolean1.compareTo(/* NPEX_NULL_EXP */
        boolean2);
    }
}
}
