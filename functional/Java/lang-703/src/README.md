# Lang 703

Exception:
```java
testJoin_Objectarray(org.apache.commons.lang3.StringUtilsTest)  Time elapsed: 0.015 sec  <<< ERROR!
java.lang.NullPointerException: null
	at org.apache.commons.lang3.StringUtils.join(StringUtils.java:3385)
	at org.apache.commons.lang3.StringUtils.join(StringUtils.java:3340)
	at org.apache.commons.lang3.StringUtils.join(StringUtils.java:3230)
	at org.apache.commons.lang3.StringUtilsTest.testJoin_Objectarray(StringUtilsTest.java:90)

testJoin_ArrayChar(org.apache.commons.lang3.StringUtilsTest)  Time elapsed: 0.001 sec  <<< ERROR!
java.lang.NullPointerException: null
	at org.apache.commons.lang3.StringUtils.join(StringUtils.java:3298)
	at org.apache.commons.lang3.StringUtilsTest.testJoin_ArrayChar(StringUtilsTest.java:106)
```

Fix Commit: [SVN Revision 1142381](http://svn.apache.org/viewvc?view=revision&revision=1142381)

Patch: <http://svn.apache.org/viewvc/commons/proper/lang/trunk/src/main/java/org/apache/commons/lang3/StringUtils.java?r1=1142381&r2=1142380&pathrev=1142381>

```diff
--- commons/proper/lang/trunk/src/main/java/org/apache/commons/lang3/StringUtils.java	2011/07/03 06:59:59	1142380
+++ commons/proper/lang/trunk/src/main/java/org/apache/commons/lang3/StringUtils.java	2011/07/03 07:04:38	1142381
@@ -3290,13 +3290,12 @@
         if (array == null) {
             return null;
         }
-        int bufSize = (endIndex - startIndex);
-        if (bufSize <= 0) {
+        int noOfItems = (endIndex - startIndex);
+        if (noOfItems <= 0) {
             return EMPTY;
         }
-
-        bufSize *= ((array[startIndex] == null ? 16 : array[startIndex].toString().length()) + 1);
-        StringBuilder buf = new StringBuilder(bufSize);
+        
+        StringBuilder buf = new StringBuilder(noOfItems * 16);
 
         for (int i = startIndex; i < endIndex; i++) {
             if (i > startIndex) {
@@ -3309,7 +3308,6 @@
         return buf.toString();
     }
 
-
     /**
      * <p>Joins the elements of the provided array into a single String
      * containing the provided list of elements.</p>
@@ -3377,15 +3375,12 @@
 
         // endIndex - startIndex > 0:   Len = NofStrings *(len(firstString) + len(separator))
         //           (Assuming that all Strings are roughly equally long)
-        int bufSize = (endIndex - startIndex);
-        if (bufSize <= 0) {
+        int noOfItems = (endIndex - startIndex);
+        if (noOfItems <= 0) {
             return EMPTY;
         }
 
-        bufSize *= ((array[startIndex] == null ? 16 : array[startIndex].toString().length())
-                        + separator.length());
-
-        StringBuilder buf = new StringBuilder(bufSize);
+        StringBuilder buf = new StringBuilder(noOfItems * 16);
 
         for (int i = startIndex; i < endIndex; i++) {
             if (i > startIndex) {
```