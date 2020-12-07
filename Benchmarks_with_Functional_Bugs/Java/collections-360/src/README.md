# Collections 360

Exception:
```java
testCollections360(org.apache.commons.collections.iterators.TestFilterListIterator)  Time elapsed: 0.012 sec  <<< ERROR!
java.lang.NullPointerException
	at org.apache.commons.collections.iterators.FilterListIterator.setNextObject(FilterListIterator.java:230)
	at org.apache.commons.collections.iterators.FilterListIterator.hasNext(FilterListIterator.java:123)
	at org.apache.commons.collections.iterators.TestFilterListIterator.testCollections360(TestFilterListIterator.java:85)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at junit.framework.TestCase.runTest(TestCase.java:168)
	at junit.framework.TestCase.runBare(TestCase.java:134)
	at junit.framework.TestResult$1.protect(TestResult.java:110)
	at junit.framework.TestResult.runProtected(TestResult.java:128)
	at junit.framework.TestResult.run(TestResult.java:113)
	at junit.framework.TestCase.run(TestCase.java:124)
	at junit.framework.TestSuite.runTest(TestSuite.java:243)
	at junit.framework.TestSuite.run(TestSuite.java:238)
	at org.junit.internal.runners.JUnit38ClassRunner.run(JUnit38ClassRunner.java:83)
	at org.apache.maven.surefire.junit4.JUnit4TestSet.execute(JUnit4TestSet.java:59)
	at org.apache.maven.surefire.suite.AbstractDirectoryTestSuite.executeTestSet(AbstractDirectoryTestSuite.java:115)
	at org.apache.maven.surefire.suite.AbstractDirectoryTestSuite.execute(AbstractDirectoryTestSuite.java:102)
	at org.apache.maven.surefire.Surefire.run(Surefire.java:180)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at org.apache.maven.surefire.booter.SurefireBooter.runSuitesInProcess(SurefireBooter.java:350)
	at org.apache.maven.surefire.booter.SurefireBooter.main(SurefireBooter.java:1021)
```

Fix Commit: [SVN Revision 1076034](http://svn.apache.org/viewvc?view=revision&revision=1076034)

Patch: <http://svn.us.apache.org/viewvc/commons/proper/collections/trunk/src/java/org/apache/commons/collections/iterators/FilterListIterator.java?r1=1076034&r2=1076033&pathrev=1076034>

```diff
--- commons/proper/collections/trunk/src/java/org/apache/commons/collections/iterators/FilterListIterator.java	2011/03/01 22:07:40	1076033
+++ commons/proper/collections/trunk/src/java/org/apache/commons/collections/iterators/FilterListIterator.java	2011/03/01 22:10:10	1076034
@@ -227,6 +227,9 @@
             clearNextObject();
         }
 
+        if (iterator == null) {
+            return false;
+        }
         while (iterator.hasNext()) {
             E object = iterator.next();
             if (predicate.evaluate(object)) {
@@ -256,6 +259,9 @@
             clearPreviousObject();
         }
 
+        if (iterator == null) {
+            return false;
+        }
         while (iterator.hasPrevious()) {
             E object = iterator.previous();
             if (predicate.evaluate(object)) {
```