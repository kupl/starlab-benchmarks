CollatingIterator
-----------------

This project requires Java 7

    export JAVA_HOME=/home/martin/martin-no-backup/opt/jdk1.7.0_10/jre
    
    $ mvn --version
    Apache Maven 3.3.9
    Maven home: /usr/share/maven
    Java version: 1.8.0_40, vendor: Oracle Corporation
    Java home: /home/martin/martin-no-backup/jdk1.8.0_40/jre
    Default locale: en_US, platform encoding: UTF-8
    OS name: "linux", version: "4.3.0-1-amd64", arch: "amd64", family: "unix"

Exception:

    testNullComparator(org.apache.commons.collections.iterators.TestCollatingIterator)  Time elapsed: 0.004 sec  <<< ERROR!
    java.lang.NullPointerException: null
            at org.apache.commons.collections.iterators.TestCollatingIterator.testNullComparator(TestCollatingIterator.java:250)

The NPE happens in test code, because `NPE.getMessage()` returns null. The fix is to throw an carefully crafted NPE with a message (instead of the original vanilla NPE).
            
Fix Commit: [SVN Revision 1079587](http://svn.apache.org/viewvc?view=revision&revision=1079587)

Patch: <http://svn.apache.org/viewvc/commons/proper/collections/trunk/src/java/org/apache/commons/collections/iterators/CollatingIterator.java?r1=1079587&r2=1079586&pathrev=1079587&diff_format=l>

```diff
--- commons/proper/collections/trunk/src/java/org/apache/commons/collections/iterators/CollatingIterator.java   2011/03/08 22:31:37 1079586
+++ commons/proper/collections/trunk/src/java/org/apache/commons/collections/iterators/CollatingIterator.java   2011/03/08 22:31:49 1079587
@@ -26,6 +26,7 @@
 
 import org.apache.commons.collections.list.UnmodifiableList;
 
+
 /**
  * Provides an ordered iteration over the elements contained in a collection of
  * ordered Iterators.
@@ -63,8 +64,10 @@
     // Constructors
     // ----------------------------------------------------------------------
     /**
-     * Constructs a new <code>CollatingIterator</code>. Natural sort order will
-     * be used, and child iterators will have to be manually added using the
+     * Constructs a new <code>CollatingIterator</code>. A comparator must be
+     * set by calling {@link #setComparator(Comparator)} before invoking
+     * {@link #hasNext()}, or {@link #next()} for the first time. Child
+     * iterators will have to be manually added using the
      * {@link #addIterator(Iterator)} method.
      */
     public CollatingIterator() {
@@ -76,8 +79,9 @@
      * specified comparator for ordering. Child iterators will have to be
      * manually added using the {@link #addIterator(Iterator)} method.
      * 
-     * @param comp the comparator to use to sort, or null to use natural sort
-     * order
+     * @param comp the comparator to use to sort; must not be null,
+     *   unless you'll be invoking {@link #setComparator(Comparator)}
+     *   later on.
      */
     public CollatingIterator(final Comparator<? super E> comp) {
         this(comp, 2);
@@ -89,8 +93,9 @@
      * capacity. Child iterators will have to be manually added using the
      * {@link #addIterator(Iterator)} method.
      * 
-     * @param comp the comparator to use to sort, or null to use natural sort
-     * order
+     * @param comp the comparator to use to sort; must not be null,
+     *   unless you'll be invoking {@link #setComparator(Comparator)}
+     *   later on.
      * @param initIterCapacity the initial capacity for the internal list of
      * child iterators
      */
@@ -104,8 +109,9 @@
      * specified comparator to provide ordered iteration over the two given
      * iterators.
      * 
-     * @param comp the comparator to use to sort, or null to use natural sort
-     * order
+     * @param comp the comparator to use to sort; must not be null,
+     *   unless you'll be invoking {@link #setComparator(Comparator)}
+     *   later on.
      * @param a the first child ordered iterator
      * @param b the second child ordered iterator
      * @throws NullPointerException if either iterator is null
@@ -121,8 +127,9 @@
      * specified comparator to provide ordered iteration over the array of
      * iterators.
      * 
-     * @param comp the comparator to use to sort, or null to use natural sort
-     * order
+     * @param comp the comparator to use to sort; must not be null,
+     *   unless you'll be invoking {@link #setComparator(Comparator)}
+     *   later on.
      * @param iterators the array of iterators
      * @throws NullPointerException if iterators array is or contains null
      */
@@ -138,8 +145,9 @@
      * specified comparator to provide ordered iteration over the collection of
      * iterators.
      * 
-     * @param comp the comparator to use to sort, or null to use natural sort
-     * order
+     * @param comp the comparator to use to sort; must not be null,
+     *   unless you'll be invoking {@link #setComparator(Comparator)}
+     *   later on.
      * @param iterators the collection of iterators
      * @throws NullPointerException if the iterators collection is or contains
      * null
@@ -204,7 +212,11 @@
     }
 
     /**
-     * Sets the {@link Comparator} by which collation occurs.
+     * Sets the {@link Comparator} by which collation occurs. If you
+     * would like to use the natural sort order (or, in other words,
+     * if the elements in the iterators are implementing the
+     * {@link java.lang.Comparable} interface), then use the
+     * {@link org.apache.commons.collections.comparators.ComparableComparator}.
      * 
      * @throws IllegalStateException if iteration has started
      */
@@ -349,6 +361,9 @@
                     leastObject = values.get(i);
                 } else {
                     E curObject = values.get(i);
+                    if (comparator == null) {
+                        throw new NullPointerException("You must invoke setComparator() to set a compator first.");
+                    }
                     if (comparator.compare(curObject, leastObject) < 0) {
                         leastObject = curObject;
                         leastIndex = i;
```