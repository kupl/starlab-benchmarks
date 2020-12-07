# Lang 587

Exception:
```java
testToClass_object(org.apache.commons.lang3.ClassUtilsTest)  Time elapsed: 0.016 sec  <<< ERROR!
java.lang.NullPointerException: null
        at org.apache.commons.lang3.ClassUtils.toClass(ClassUtils.java:910)
        at org.apache.commons.lang3.ClassUtilsTest.testToClass_object(ClassUtilsTest.java:901)
```

Fix Commit: [SVN Revision 907102](http://svn.apache.org/viewvc?view=revision&revision=907102)

The test has been modified: the `assertEquals(Arrays.equals(...))` (equivalent to `assertArrayEquals`) has been removed.

Patch: <http://svn.apache.org/viewvc/commons/proper/lang/trunk/src/main/java/org/apache/commons/lang3/ClassUtils.java?r1=907102&r2=907101&pathrev=907102>

```diff
Class<?>[] classes = new Class[array.length];
for (int i = 0; i < array.length; i++) {
-            classes[i] = array[i].getClass();
+            classes[i] = array[i] == null ? null : array[i].getClass();
}
return classes;
```
     