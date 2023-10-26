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
package org.apache.commons.collections.iterators;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.collections.list.UnmodifiableList;

/**
 * An IteratorChain is an Iterator that wraps a number of Iterators.
 * <p>
 * This class makes multiple iterators look like one to the caller When any
 * method from the Iterator interface is called, the IteratorChain will delegate
 * to a single underlying Iterator. The IteratorChain will invoke the Iterators
 * in sequence until all Iterators are exhausted.
 * <p>
 * Under many circumstances, linking Iterators together in this manner is more
 * efficient (and convenient) than reading out the contents of each Iterator
 * into a List and creating a new Iterator.
 * <p>
 * Calling a method that adds new Iterator<i>after a method in the Iterator
 * interface has been called</i> will result in an
 * UnsupportedOperationException. Subclasses should <i>take care</i> to not
 * alter the underlying List of Iterators.
 * <p>
 * NOTE: As from version 3.0, the IteratorChain may contain no iterators. In
 * this case the class will function as an empty iterator.
 *
 * @since Commons Collections 2.1
 * @version $Revision: 814997 $ $Date: 2006-10-27 19:52:37 -0500 (Fri, 27 Oct
 * 2006) $
 *
 * @author Morgan Delagrange
 * @author Stephen Colebourne
 */
public class IteratorChain<E> implements Iterator<E> {

    /** The chain of iterators */
    protected final List<Iterator<? extends E>> iteratorChain = new ArrayList<Iterator<? extends E>>();

    /** The index of the current iterator */
    protected int currentIteratorIndex = 0;

    /** The current iterator */
    protected Iterator<? extends E> currentIterator = null;

    /**
     * The "last used" Iterator is the Iterator upon which next() or hasNext()
     * was most recently called used for the remove() operation only
     */
    protected Iterator<? extends E> lastUsedIterator = null;

    /**
     * ComparatorChain is "locked" after the first time compare(Object,Object)
     * is called
     */
    protected boolean isLocked = false;

    //-----------------------------------------------------------------------
    /**
     * Construct an IteratorChain with no Iterators.
     * <p>
     * You will normally use {@link #addIterator(Iterator)} to add some
     * iterators after using this constructor.
     */
    public IteratorChain() {
        super();
    }

    /**
     * Construct an IteratorChain with a single Iterator.
     * <p>
     * This method takes one iterator. The newly constructed iterator will
     * iterate through that iterator. Thus calling this constructor on its own
     * will have no effect other than decorating the input iterator.
     * <p>
     * You will normally use {@link #addIterator(Iterator)} to add some more
     * iterators after using this constructor.
     * 
     * @param iterator the first child iterator in the IteratorChain, not null
     * @throws NullPointerException if the iterator is null
     */
    public IteratorChain(Iterator<? extends E> iterator) {
        super();
        addIterator(iterator);
    }

    /**
     * Constructs a new <code>IteratorChain</code> over the two given iterators.
     * <p>
     * This method takes two iterators. The newly constructed iterator will
     * iterate through each one of the input iterators in turn.
     * 
     * @param first the first child iterator in the IteratorChain, not null
     * @param second the second child iterator in the IteratorChain, not null
     * @throws NullPointerException if either iterator is null
     */
    public IteratorChain(Iterator<? extends E> first, Iterator<? extends E> second) {
        super();
        addIterator(first);
        addIterator(second);
    }

    /**
     * Constructs a new <code>IteratorChain</code> over the array of iterators.
     * <p>
     * This method takes an array of iterators. The newly constructed iterator
     * will iterate through each one of the input iterators in turn.
     * 
     * @param iteratorChain the array of iterators, not null
     * @throws NullPointerException if iterators array is or contains null
     */
    public IteratorChain(Iterator<? extends E>[] iteratorChain) {
        super();
        for (int i = 0; i < iteratorChain.length; i++) {
            addIterator(iteratorChain[i]);
        }
    }

    /**
     * Constructs a new <code>IteratorChain</code> over the collection of
     * iterators.
     * <p>
     * This method takes a collection of iterators. The newly constructed
     * iterator will iterate through each one of the input iterators in turn.
     * 
     * @param iteratorChain the collection of iterators, not null
     * @throws NullPointerException if iterators collection is or contains null
     * @throws ClassCastException if iterators collection doesn't contain an
     * iterator
     */
    public IteratorChain(Collection<Iterator<? extends E>> iteratorChain) {
        super();
        for (Iterator<? extends E> iterator : iteratorChain) {
            addIterator(iterator);
        }
    }

    //-----------------------------------------------------------------------
    /**
     * Add an Iterator to the end of the chain
     * 
     * @param iterator Iterator to add
     * @throws IllegalStateException if I've already started iterating
     * @throws NullPointerException if the iterator is null
     */
    public void addIterator(Iterator<? extends E> iterator) {
        checkLocked();
        if (iterator == null) {
            throw new NullPointerException("Iterator must not be null");
        }
        iteratorChain.add(iterator);
    }

    /**
     * Set the Iterator at the given index
     * 
     * @param index index of the Iterator to replace
     * @param iterator Iterator to place at the given index
     * @throws IndexOutOfBoundsException if index &lt; 0 or index &gt; size()
     * @throws IllegalStateException if I've already started iterating
     * @throws NullPointerException if the iterator is null
     */
    public void setIterator(int index, Iterator<? extends E> iterator)
            throws IndexOutOfBoundsException {
        checkLocked();
        if (iterator == null) {
            throw new NullPointerException("Iterator must not be null");
        }
        iteratorChain.set(index, iterator);
    }

    /**
     * Get the list of Iterators (unmodifiable)
     * 
     * @return the unmodifiable list of iterators added
     */
    public List<Iterator<? extends E>> getIterators() {
        return UnmodifiableList.decorate(iteratorChain);
    }

    /**
     * Number of Iterators in the current IteratorChain.
     * 
     * @return Iterator count
     */
    public int size() {
        return iteratorChain.size();
    }

    /**
     * Determine if modifications can still be made to the IteratorChain.
     * IteratorChains cannot be modified once they have executed a method from
     * the Iterator interface.
     * 
     * @return true if IteratorChain cannot be modified, false if it can
     */
    public boolean isLocked() {
        return isLocked;
    }

    /**
     * Checks whether the iterator chain is now locked and in use.
     */
    private void checkLocked() {
        if (isLocked == true) {
            throw new UnsupportedOperationException(
                    "IteratorChain cannot be changed after the first use of a method from the Iterator interface");
        }
    }

    /**
     * Lock the chain so no more iterators can be added. This must be called
     * from all Iterator interface methods.
     */
    private void lockChain() {
        if (isLocked == false) {
            isLocked = true;
        }
    }

    /**
     * Updates the current iterator field to ensure that the current Iterator is
     * not exhausted
     */
    protected void updateCurrentIterator() {
        if (currentIterator == null) {
            if (iteratorChain.isEmpty()) {
                currentIterator = EmptyIterator.<E> getInstance();
            } else {
                currentIterator = iteratorChain.get(0);
            }
            // set last used iterator here, in case the user calls remove
            // before calling hasNext() or next() (although they shouldn't)
            lastUsedIterator = currentIterator;
        }

        while (currentIterator.hasNext() == false
                && currentIteratorIndex < iteratorChain.size() - 1) {
            currentIteratorIndex++;
            currentIterator = iteratorChain.get(currentIteratorIndex);
        }
    }

    //-----------------------------------------------------------------------
    /**
     * Return true if any Iterator in the IteratorChain has a remaining element.
     * 
     * @return true if elements remain
     */
    public boolean hasNext() {
        lockChain();
        updateCurrentIterator();
        lastUsedIterator = currentIterator;

        return currentIterator.hasNext();
    }

    /**
     * Returns the next Object of the current Iterator
     * 
     * @return Object from the current Iterator
     * @throws java.util.NoSuchElementException if all the Iterators are
     * exhausted
     */
    public E next() {
        lockChain();
        updateCurrentIterator();
        lastUsedIterator = currentIterator;

        return currentIterator.next();
    }

    /**
     * Removes from the underlying collection the last element returned by the
     * Iterator. As with next() and hasNext(), this method calls remove() on the
     * underlying Iterator. Therefore, this method may throw an
     * UnsupportedOperationException if the underlying Iterator does not support
     * this method.
     * 
     * @throws UnsupportedOperationException if the remove operator is not
     * supported by the underlying Iterator
     * @throws IllegalStateException if the next method has not yet been called,
     * or the remove method has already been called after the last call to the
     * next method.
     */
    public void remove() {
        lockChain();
        if (currentIterator == null) {
            updateCurrentIterator();
        }
        lastUsedIterator.remove();
    }

}
