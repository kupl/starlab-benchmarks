# PDFBox 2965

Exception:
```java
testFieldsEntry(org.apache.pdfbox.pdmodel.interactive.form.PDAcroFormTest)  Time elapsed: 0.048 sec  <<< ERROR!
java.lang.NullPointerException: null
	at org.apache.pdfbox.pdmodel.interactive.form.PDAcroForm.getField(PDAcroForm.java:251)
	at org.apache.pdfbox.pdmodel.interactive.form.PDAcroFormTest.testFieldsEntry(PDAcroFormTest.java:68)
```

Fix Commit: [SVN Revision 1701905](http://svn.apache.org/viewvc?view=revision&revision=1701905)

Patch: <http://svn.apache.org/viewvc/pdfbox/trunk/pdfbox/src/main/java/org/apache/pdfbox/pdmodel/interactive/form/PDAcroForm.java?r1=1701905&r2=1701904&pathrev=1701905>

```diff
--- pdfbox/trunk/pdfbox/src/main/java/org/apache/pdfbox/pdmodel/interactive/form/PDAcroForm.java	2015/09/09 06:11:13	1701904
+++ pdfbox/trunk/pdfbox/src/main/java/org/apache/pdfbox/pdmodel/interactive/form/PDAcroForm.java	2015/09/09 06:19:19	1701905
@@ -234,9 +234,8 @@
      *
      * @param fullyQualifiedName The name of the field to get.
      * @return The field with that name of null if one was not found.
-     * @throws IOException If there is an error getting the field type.
      */
-    public PDField getField(String fullyQualifiedName) throws IOException
+    public PDField getField(String fullyQualifiedName)
     {
         PDField retval = null;
         if (fieldCache != null)
@@ -248,35 +247,38 @@
             String[] nameSubSection = fullyQualifiedName.split("\\.");
             COSArray fields = (COSArray) dictionary.getDictionaryObject(COSName.FIELDS);
 
-            for (int i = 0; i < fields.size() && retval == null; i++)
+            if (fields != null)
             {
-                COSDictionary element = (COSDictionary) fields.getObject(i);
-                if (element != null)
+                for (int i = 0; i < fields.size() && retval == null; i++)
                 {
-                    COSString fieldName =
-                        (COSString)element.getDictionaryObject(COSName.T);
-                    if (fieldName.getString().equals(fullyQualifiedName) ||
-                        fieldName.getString().equals(nameSubSection[0]))
+                    COSDictionary element = (COSDictionary) fields.getObject(i);
+                    if (element != null)
                     {
-                        PDField root = PDField.fromDictionary(this, element, null);
-                        if (root != null)
+                        COSString fieldName =
+                            (COSString)element.getDictionaryObject(COSName.T);
+                        if (fieldName.getString().equals(fullyQualifiedName) ||
+                            fieldName.getString().equals(nameSubSection[0]))
                         {
-                            if (nameSubSection.length > 1)
+                            PDField root = PDField.fromDictionary(this, element, null);
+                            if (root != null)
                             {
-                                PDField kid = root.findKid(nameSubSection, 1);
-                                if (kid != null)
+                                if (nameSubSection.length > 1)
                                 {
-                                    retval = kid;
+                                    PDField kid = root.findKid(nameSubSection, 1);
+                                    if (kid != null)
+                                    {
+                                        retval = kid;
+                                    }
+                                    else
+                                    {
+                                        retval = root;
+                                    }
                                 }
                                 else
                                 {
                                     retval = root;
                                 }
                             }
-                            else
-                            {
-                                retval = root;
-                            }
                         }
                     }
                 }
```