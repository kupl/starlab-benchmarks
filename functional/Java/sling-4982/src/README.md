# Sling 4982

Exception:
```java
test_isJcrData6(org.apache.sling.security.impl.ContentDispositionFilterTest)  Time elapsed: 0.101 sec  <<< ERROR!
java.lang.NullPointerException: null
	at org.apache.sling.security.impl.ContentDispositionFilter$RewriterResponse.isJcrData(ContentDispositionFilter.java:288)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at junitx.util.PrivateAccessor.invoke(PrivateAccessor.java:253)
	at org.apache.sling.security.impl.ContentDispositionFilterTest.test_isJcrData6(ContentDispositionFilterTest.java:60)

test_isJcrData7(org.apache.sling.security.impl.ContentDispositionFilterTest)  Time elapsed: 0.005 sec  <<< ERROR!
java.lang.NullPointerException: null
	at org.apache.sling.security.impl.ContentDispositionFilter$RewriterResponse.isJcrData(ContentDispositionFilter.java:294)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at junitx.util.PrivateAccessor.invoke(PrivateAccessor.java:253)
	at org.apache.sling.security.impl.ContentDispositionFilterTest.test_isJcrData7(ContentDispositionFilterTest.java:92)
```

Fix Commit: [SVN Revision 1700424](http://svn.apache.org/viewvc?view=revision&revision=1700424)

Patch: <http://svn.us.apache.org/viewvc/sling/trunk/contrib/extensions/security/src/main/java/org/apache/sling/security/impl/ContentDispositionFilter.java?r1=1700424&r2=1700423&pathrev=1700424>

```diff
--- sling/trunk/contrib/extensions/security/src/main/java/org/apache/sling/security/impl/ContentDispositionFilter.java	2015/09/01 08:54:32	1700423
+++ sling/trunk/contrib/extensions/security/src/main/java/org/apache/sling/security/impl/ContentDispositionFilter.java	2015/09/01 08:55:03	1700424
@@ -285,13 +285,13 @@
             boolean jcrData = false;
             if (resource!= null) {
                 ValueMap props = resource.adaptTo(ValueMap.class);
-                if (props.containsKey(PROP_JCR_DATA) ) {
+                if (props != null && props.containsKey(PROP_JCR_DATA) ) {
                     jcrData = true;
                 } else {
                     Resource jcrContent = resource.getChild(JCR_CONTENT_LEAF);
                     if (jcrContent!= null) {
                         props = jcrContent.adaptTo(ValueMap.class);
-                        if (props.containsKey(PROP_JCR_DATA) ) {
+                        if (props != null && props.containsKey(PROP_JCR_DATA) ) {
                             jcrData = true;
                         }
                     }
```