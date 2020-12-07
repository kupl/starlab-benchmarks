# PDFBox 2995

Exception:
```java
testAcroFormProperties(org.apache.pdfbox.pdmodel.interactive.form.PDAcroFormTest)  Time elapsed: 0.008 sec  <<< ERROR!
java.lang.NullPointerException: null
	at org.apache.pdfbox.pdmodel.interactive.form.PDAcroForm.getDefaultAppearance(PDAcroForm.java:356)
	at org.apache.pdfbox.pdmodel.interactive.form.PDAcroFormTest.testAcroFormProperties(PDAcroFormTest.java:48)
```

Fix Commit: [SVN Revision 1705415](http://svn.apache.org/viewvc?view=revision&revision=1705415)

Patch: <http://svn.us.apache.org/viewvc/pdfbox/trunk/pdfbox/src/main/java/org/apache/pdfbox/pdmodel/interactive/form/PDAcroForm.java?r1=1705415&r2=1705414&pathrev=1705415>

```diff
--- pdfbox/trunk/pdfbox/src/main/java/org/apache/pdfbox/pdmodel/interactive/form/PDAcroForm.java	2015/09/26 11:22:37	1705414
+++ pdfbox/trunk/pdfbox/src/main/java/org/apache/pdfbox/pdmodel/interactive/form/PDAcroForm.java	2015/09/26 11:32:45	1705415
@@ -29,7 +29,6 @@
 import org.apache.pdfbox.cos.COSDictionary;
 import org.apache.pdfbox.cos.COSName;
 import org.apache.pdfbox.cos.COSNumber;
-import org.apache.pdfbox.cos.COSString;
 import org.apache.pdfbox.pdmodel.PDDocument;
 import org.apache.pdfbox.pdmodel.PDPage;
 import org.apache.pdfbox.pdmodel.PDPageContentStream;
@@ -352,8 +351,7 @@
      */
     public String getDefaultAppearance()
     {
-        COSString defaultAppearance = (COSString) dictionary.getItem(COSName.DA);
-        return defaultAppearance.getString();
+        return dictionary.getString(COSName.DA,"");
     }
 
     /**
```