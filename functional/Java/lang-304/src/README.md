# Lang 304

Exception:
```java
java.lang.NullPointerException: null
    at org.apache.commons.lang.LocaleUtils.isAvailableLocale(LocaleUtils.java:223)
    at org.apache.commons.lang.LocaleUtilsTest.setUp(LocaleUtilsTest.java:87)
```

Fix Commit: [SVN Revision 489749](http://svn.apache.org/viewvc?view=revision&revision=489749)

Patch: <http://svn.apache.org/viewvc/jakarta/commons/proper/lang/trunk/src/java/org/apache/commons/lang/LocaleUtils.java?r1=489749&r2=489748&pathrev=489749>

```diff
--- jakarta/commons/proper/lang/trunk/src/java/org/apache/commons/lang/LocaleUtils.java	2006/12/22 20:32:43	489748
+++ jakarta/commons/proper/lang/trunk/src/java/org/apache/commons/lang/LocaleUtils.java	2006/12/22 20:34:37	489749
@@ -220,7 +220,7 @@
      * @return true if the locale is a known locale
      */
     public static boolean isAvailableLocale(Locale locale) {
-        return cAvailableLocaleSet.contains(locale);
+        return availableLocaleList().contains(locale);
     }
 
     //-----------------------------------------------------------------------
```