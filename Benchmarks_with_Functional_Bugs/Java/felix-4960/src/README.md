# Felix 4960

Exception:
```java
testGetResourcesLocalNullContentPath(org.apache.felix.framework.BundleRevisionImplTest)  Time elapsed: 0.013 sec  <<< ERROR!
java.lang.NullPointerException
	at org.apache.felix.framework.BundleRevisionImpl.getResourcesLocal(BundleRevisionImpl.java:537)
	at org.apache.felix.framework.BundleRevisionImplTest.testGetResourcesLocalNullContentPath(BundleRevisionImplTest.java:39)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at junit.framework.TestCase.runTest(TestCase.java:164)
	at junit.framework.TestCase.runBare(TestCase.java:130)
	at junit.framework.TestResult$1.protect(TestResult.java:110)
	at junit.framework.TestResult.runProtected(TestResult.java:128)
	at junit.framework.TestResult.run(TestResult.java:113)
	at junit.framework.TestCase.run(TestCase.java:120)
	at junit.framework.TestSuite.runTest(TestSuite.java:228)
	at junit.framework.TestSuite.run(TestSuite.java:223)
	at org.junit.internal.runners.OldTestClassRunner.run(OldTestClassRunner.java:35)
	at org.apache.maven.surefire.junit4.JUnit4TestSet.execute(JUnit4TestSet.java:35)
	at org.apache.maven.surefire.junit4.JUnit4Provider.executeTestSet(JUnit4Provider.java:115)
	at org.apache.maven.surefire.junit4.JUnit4Provider.invoke(JUnit4Provider.java:97)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at org.apache.maven.surefire.booter.ProviderFactory$ClassLoaderProxy.invoke(ProviderFactory.java:103)
	at com.sun.proxy.$Proxy0.invoke(Unknown Source)
	at org.apache.maven.surefire.booter.SurefireStarter.invokeProvider(SurefireStarter.java:150)
	at org.apache.maven.surefire.booter.SurefireStarter.runSuitesInProcess(SurefireStarter.java:91)
	at org.apache.maven.surefire.booter.ForkedBooter.main(ForkedBooter.java:69)
```

Fix Commit: [SVN Revision 1691137](http://svn.apache.org/viewvc?view=revision&revision=1691137)

Patch: <http://svn.us.apache.org/viewvc/felix/trunk/framework/src/main/java/org/apache/felix/framework/BundleRevisionImpl.java?r1=1691137&r2=1691136&pathrev=1691137>

```diff
--- felix/trunk/framework/src/main/java/org/apache/felix/framework/BundleRevisionImpl.java	2015/07/15 07:32:19	1691136
+++ felix/trunk/framework/src/main/java/org/apache/felix/framework/BundleRevisionImpl.java	2015/07/15 07:44:28	1691137
@@ -202,7 +202,7 @@
 
     static List<Capability> asCapabilityList(List reqs)
     {
-        return (List<Capability>) reqs;
+        return reqs;
     }
 
     public List<BundleCapability> getDeclaredCapabilities(String namespace)
@@ -229,7 +229,7 @@
 
     static List<Requirement> asRequirementList(List reqs)
     {
-        return (List<Requirement>) reqs;
+        return reqs;
     }
 
     public List<BundleRequirement> getDeclaredRequirements(String namespace)
@@ -517,6 +517,9 @@
         // each bundle class path entry...this isn't very
         // clean or meaningful, but the Spring guys want it.
         final List<Content> contentPath = getContentPath();
+        if (contentPath == null)
+            return Collections.emptyEnumeration();
+
         if (name.equals("/"))
         {
             for (int i = 0; i < contentPath.size(); i++)
```