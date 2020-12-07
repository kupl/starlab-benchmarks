# Math 988a

Exception:
```java
testIntersectionParallel(org.apache.commons.math3.geometry.euclidean.twod.SubLineTest)  Time elapsed: 0.025 sec  <<< ERROR!
java.lang.NullPointerException: null
	at org.apache.commons.math3.geometry.euclidean.twod.Line.toSubSpace(Line.java:182)
	at org.apache.commons.math3.geometry.euclidean.twod.SubLine.intersection(SubLine.java:124)
	at org.apache.commons.math3.geometry.euclidean.twod.SubLineTest.testIntersectionParallel(SubLineTest.java:151)
```

Fix Commit: [SVN Revision 1488866](http://svn.apache.org/viewvc?view=revision&revision=1488866)

Patch: <http://svn.apache.org/viewvc/commons/proper/math/trunk/src/main/java/org/apache/commons/math3/geometry/euclidean/twod/SubLine.java?r1=1488866&r2=1488865&pathrev=1488866>

```diff
--- commons/proper/math/trunk/src/main/java/org/apache/commons/math3/geometry/euclidean/twod/SubLine.java	2013/06/03 07:12:06	1488865
+++ commons/proper/math/trunk/src/main/java/org/apache/commons/math3/geometry/euclidean/twod/SubLine.java	2013/06/03 07:15:00	1488866
@@ -115,6 +115,9 @@
 
         // compute the intersection on infinite line
         Vector2D v2D = line1.intersection(line2);
+        if (v2D == null) {
+            return null;
+        }
 
         // check location of point with respect to first sub-line
         Location loc1 = getRemainingRegion().checkPoint(line1.toSubSpace(v2D));
```