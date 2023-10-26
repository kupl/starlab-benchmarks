/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.commons.collections.functors;

import java.io.Serializable;
import java.util.Comparator;

import org.apache.commons.collections.Predicate;

/**
 * Predicate that compares the input object with the one stored in the predicate using a comparator.
 * In addition, the comparator result can be evaluated in accordance to a supplied criterion value.
 *
 * In order to demonstrate the use of the predicate, the following variables are declared:
 *
 * <pre>
 * Integer ONE = new Integer(1);
 * Integer TWO = new Integer(2);
 *
 * Comparator comparator = new Comparator() {
 *
 *     public int compare(Object first, Object second) {
 *         return ((Integer) second) - ((Integer) first);
 *     }
 *
 * };
 * </pre>
 *
 * Using the declared variables, the <code>ComparatorPredicate</code> can be used used in the
 * following way:
 *
 * <pre>
 * ComparatorPredicate.getInstance(ONE, comparator).evaluate(TWO);
 * </pre>
 *
 * The input variable <code>TWO</code> in compared to the stored variable <code>ONE</code> using
 * the supplied <code>comparator</code>. This is the default usage of the predicate and will return
 * <code>true</code> if the underlying comparator returns <code>0</code>. In addition to the default
 * usage of the predicate, it is possible to evaluate the comparator's result in several ways. The
 * following {@link Criterion} enumeration values are provided by the predicate:
 * </p>
 *
 * <ul>
 *     <li>EQUAL</li>
 *     <li>GREATER</li>
 *     <li>GREATER_OR_EQUAL</li>
 *     <li>LESS</li>
 *     <li>LESS_OR_EQUAL</li>
 * </ul>
 *
 * The following examples demonstrates how these constants can be used in order to manipulate the
 * evaluation of a comparator result.
 *
 * <pre>
 * ComparatorPredicate.getInstance(ONE, comparator, <b>ComparatorPredicate.Criterion.GREATER</b>).evaluate(TWO);
 * </pre>
 *
 * The input variable TWO is compared to the stored variable ONE using the supplied <code>comparator</code>
 * using the <code>GREATER</code> evaluation criterion constant. This instructs the predicate to
 * return <code>true</code> if the comparator returns a value greater than <code>0</code>.
 *
 * @since Commons Collections 4.0
 * @version $Revision: $ $Date: $
 *
 * @author Rune Peter Bj�rnstad.
 */
public class ComparatorPredicate<T> implements Predicate<T>, Serializable {

    private static final long serialVersionUID = -1863209236504077399L;

    public enum Criterion {
        EQUAL, GREATER, LESS, GREATER_OR_EQUAL, LESS_OR_EQUAL,
    }

    // Instance variables:

    /** The internal object to compare with */
    private final T object;

    /** The comparator to use for comparison */
    private final Comparator<T> comparator;

    /** The comparison evaluation criterion to use */
    private final Criterion criterion;

    /**
     * Factory to create the comparator predicate
     *
     * @param object  the object to compare to
     * @param comparator  the comparator to use for comparison
     * @return the predicate
     * @throws IllegalArgumentException if comparator is null
     */
    public static <T> Predicate<T> getInstance(T object, Comparator<T> comparator) {
        return getInstance(object, comparator, Criterion.EQUAL);
    }

    /**
     * Factory to create the comparator predicate
     *
     * @param object  the object to compare to
     * @param comparator  the comparator to use for comparison
     * @param criterion  the criterion to use to evaluate comparison
     * @return the predicate
     * @throws IllegalArgumentException if comparator is null of criterion is invalid
     */
    public static <T> Predicate<T> getInstance(T object, Comparator<T> comparator, Criterion criterion) {
        if (comparator == null) {
            throw new IllegalArgumentException("Comparator must not be null.");
        }
        if (criterion == null) {
            throw new IllegalArgumentException("Criterion must not be null.");
        }
        return new ComparatorPredicate<T>(object, comparator, criterion);
    }

    /**
     * Constructor that performs no validation.
     * Use <code>getInstance</code> if you want.
     *
     * @param object  the object to compare to
     * @param comparator  the comparator to use for comparison
     * @param criterion  the criterion to use to evaluate comparison
     */
    public ComparatorPredicate(T object, Comparator<T> comparator, Criterion criterion) {
        super();
        this.object = object;
        this.comparator = comparator;
        this.criterion = criterion;
    }

    /**
     * Evaluates the predicate. The predicate evaluates to <code>true</code> in the following cases:
     *
     * <ul>
     *     <li><code>comparator.compare(object, input) == 0 && criterion == EQUAL</code></li>
     *     <li><code>comparator.compare(object, input) < 0 && criterion == LESS</code></li>
     *     <li><code>comparator.compare(object, input) > 0 && criterion == GREATER</code></li>
     *     <li><code>comparator.compare(object, input) >= 0 && criterion == GREATER_OR_EQUAL</code></li>
     *     <li><code>comparator.compare(object, input) <= 0 && criterion == LESS_OR_EQUAL</code></li>
     * </ul>
     *
     * @see org.apache.commons.collections.Predicate#evaluate(java.lang.Object)
     * @see java.util.Comparator#compare(java.lang.Object first, java.lang.Object second)
     *
     * @throws IllegalStateException if the criterion is invalid (really not possible)
     */
    public boolean evaluate(T target) {

        boolean result = false;
        int comparison = comparator.compare(object, target);
        switch (criterion) {
        case EQUAL:
            result = (comparison == 0);
            break;
        case GREATER:
            result = (comparison > 0);
            break;
        case LESS:
            result = (comparison < 0);
            break;
        case GREATER_OR_EQUAL:
            result = (comparison >= 0);
            break;
        case LESS_OR_EQUAL:
            result = (comparison <= 0);
            break;
        default:
            throw new IllegalStateException("The current criterion '" + criterion + "' is invalid.");
        }

        return result;
    }
}
