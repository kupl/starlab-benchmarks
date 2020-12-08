/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
package org.apache.felix.ipojo.parser;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.jar.Manifest;

import junit.framework.TestCase;

import org.apache.felix.ipojo.ConfigurationException;
import org.apache.felix.ipojo.metadata.Element;

public class PojoMetadataTest extends TestCase {

    PojoMetadata FooProviderType1, FooBarProviderType1, FooProviderTypeDyn, PrimitiveManipulationTester, SimpleMultipleCheckServiceProvider;

    private String header;

	public void setUp() {
		File manFile = new File("src/test/resources/manipulation/MANIFEST.MF");
		Manifest manifest;
		try {
			manifest = new Manifest(new FileInputStream(manFile));
			header = manifest.getMainAttributes().getValue("iPOJO-Components");
		} catch (FileNotFoundException e) {
			fail(e.getMessage());
		} catch (IOException e) {
			fail(e.getMessage());
		}

        String comp_name = "ManipulationMetadata-FooProviderType-1";
        FooProviderType1 = getManipulationMetadataForComponent(comp_name);

        comp_name = "ManipulationMetadata-FooBarProviderType-1";
        FooBarProviderType1 = getManipulationMetadataForComponent(comp_name);

        comp_name = "ManipulationMetadata-FooProviderType-Dyn";
        FooProviderTypeDyn = getManipulationMetadataForComponent(comp_name);

        comp_name = "ManipulationMetadata-PrimitiveManipulationTester";
        PrimitiveManipulationTester = getManipulationMetadataForComponent(comp_name);

        comp_name = "ManipulationMetadata-SimpleMultipleCheckServiceProvider";
        SimpleMultipleCheckServiceProvider = getManipulationMetadataForComponent(comp_name);
	}

	public void testGetMetadata() {
		Element elem = null;
		try {
			elem = ManifestMetadataParser.parseHeaderMetadata(header);
		} catch (ParseException e) {
			fail("Parse Exception when parsing iPOJO-Component");
		}

		assertNotNull("Check elem not null", elem);

		Element manip = getMetadataForComponent(elem, "ManipulationMetadata-FooProviderType-1");
        assertNotNull("Check manipulation metadata not null for " + "Manipulation-FooProviderType-1", manip);
        PojoMetadata mm;
        try {
            mm = new PojoMetadata(manip);
            assertNotNull("Check mm not null", mm);
        } catch (ConfigurationException e) {
            fail("The creation of pojo metadata has failed");
        }
	}

	public void testInterface() {
	    PojoMetadata manip = FooProviderType1;

        String[] itf = manip.getInterfaces();
		assertEquals("Check interfaces number", itf.length, 1);
		assertEquals("Check itf name", itf[0], "org.apache.felix.ipojo.test.scenarios.manipulation.service.FooService");

        assertTrue("Check Foo Service implementation", manip.isInterfaceImplemented("org.apache.felix.ipojo.test.scenarios.manipulation.service.FooService"));
        assertFalse("Check Bar Service implementation", manip.isInterfaceImplemented("org.apache.felix.ipojo.test.scenarios.manipulation.service.BarService"));
	}

	public void testInterfaces() {
	    PojoMetadata manip = FooBarProviderType1;
        String[] itf = manip.getInterfaces();
		assertEquals("Check interfaces number", itf.length, 2);
		assertEquals("Check itf name", itf[0], "org.apache.felix.ipojo.test.scenarios.manipulation.service.FooService");
		assertEquals("Check itf name", itf[1], "org.apache.felix.ipojo.test.scenarios.manipulation.service.BarService");

        assertTrue("Check Foo Service implementation", manip.isInterfaceImplemented("org.apache.felix.ipojo.test.scenarios.manipulation.service.FooService"));
        assertTrue("Check Bar Service implementation", manip.isInterfaceImplemented("org.apache.felix.ipojo.test.scenarios.manipulation.service.BarService"));
	}

	public void testFields() {
	    PojoMetadata manip = FooProviderTypeDyn;

		FieldMetadata[] fields = manip.getFields();
		assertEquals("Check field count + " + fields.length, fields.length, 5);
		/*
		private int intProp;
		private String strProp;
		private String[] strAProp;
		private int[] intAProp;
		private boolean boolProp;
		 */

		FieldMetadata field;

		field = manip.getField("intProp");
		assertEquals("Check field name : " + field.getFieldName(), field.getFieldName(), "intProp");
		assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "int");
        assertEquals("Check field reflective type : " + field.getFieldName(), FieldMetadata.getReflectionType(field.getFieldType()), "int");

        field = manip.getField("intProp", "int");
        assertEquals("Check field name : " + field.getFieldName(), field.getFieldName(), "intProp");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "int");

        field = manip.getField("intProp", "long");
        assertNull("Check bad field", field);

		field = manip.getField("strProp");
		assertEquals("Check field name : " + field.getFieldName(), field.getFieldName(), "strProp");
		assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "java.lang.String");
        assertEquals("Check field reflective type : " + field.getFieldName(), FieldMetadata.getReflectionType(field.getFieldType()), "java.lang.String");

        field = manip.getField("strProp", "String");
        assertNull("Check bad field", field);

        field = manip.getField("strProp", "java.lang.String");
        assertEquals("Check field name : " + field.getFieldName(), field.getFieldName(), "strProp");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "java.lang.String");

		field = manip.getField("strAProp");
		assertEquals("Check field name : " + field.getFieldName(), field.getFieldName(), "strAProp");
		assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "java.lang.String[]");
        assertEquals("Check field reflective type : " + field.getFieldName() + " -> " + FieldMetadata.getReflectionType(field.getFieldType()), FieldMetadata.getReflectionType(field.getFieldType()), "[Ljava.lang.String;");

        field = manip.getField("strAProp", "java.lang.String[]");
        assertEquals("Check field name : " + field.getFieldName(), field.getFieldName(), "strAProp");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "java.lang.String[]");

        field = manip.getField("strAProp", "String[]");
        assertNull("Check bad field", field);

		field = manip.getField("intAProp");
		assertEquals("Check field name : " + field.getFieldName(), field.getFieldName(), "intAProp");
		assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "int[]");
        assertEquals("Check field reflective type : " + field.getFieldName() + " -> " + FieldMetadata.getReflectionType(field.getFieldType()), FieldMetadata.getReflectionType(field.getFieldType()), "[I");

        field = manip.getField("intAProp", "int[]");
        assertEquals("Check field name : " + field.getFieldName(), field.getFieldName(), "intAProp");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "int[]");

        field = manip.getField("intAProp", "String[]");
        assertNull("Check bad field", field);

		field = manip.getField("boolProp");
		assertEquals("Check field name : " + field.getFieldName(), field.getFieldName(), "boolProp");
		assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "boolean");
        assertEquals("Check field reflective type : " + field.getFieldName(), FieldMetadata.getReflectionType(field.getFieldType()), "boolean");

        field = manip.getField("boolProp", "boolean");
        assertEquals("Check field name : " + field.getFieldName(), field.getFieldName(), "boolProp");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "boolean");

        field = manip.getField("boolProp", "bool");
        assertNull("Check bad field", field);
	}

	public void testPrimitivesFields() {
	    PojoMetadata manip = PrimitiveManipulationTester;
		FieldMetadata[] fields = manip.getFields();
		assertEquals("Check field count", fields.length, 16);

		FieldMetadata field;

		field = manip.getField("b");
		assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "byte");
		field = manip.getField("s");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "short");
		field = manip.getField("i");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "int");
		field = manip.getField("l");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "long");
		field = manip.getField("d");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "double");
		field = manip.getField("f");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "float");
		field = manip.getField("c");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "char");
		field = manip.getField("bool");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "boolean");

		field = manip.getField("bs");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "byte[]");
		field = manip.getField("ss");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "short[]");
		field = manip.getField("is");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "int[]");
		field = manip.getField("ls");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "long[]");
		field = manip.getField("ds");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "double[]");
		field = manip.getField("fs");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "float[]");
		field = manip.getField("cs");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "char[]");
		field = manip.getField("bools");
        assertEquals("Check field type : " + field.getFieldName(), field.getFieldType(), "boolean[]");
	}

	public void testNoArgMethod() {
	    PojoMetadata manip = SimpleMultipleCheckServiceProvider;
		MethodMetadata method = manip.getMethod("check");
		assertEquals("Check no args", method.getMethodArguments().length, 0);
		assertEquals("Check return", method.getMethodReturn(), "boolean");

        method = manip.getMethod("check", new String[0]);
        assertEquals("Check no args", method.getMethodArguments().length, 0);
        assertEquals("Check return", method.getMethodReturn(), "boolean");
	}

	public void testOneArgsMethod() {
	    PojoMetadata manip = SimpleMultipleCheckServiceProvider;
        MethodMetadata method = manip.getMethods("refBind")[0];
		assertEquals("Check args count", method.getMethodArguments().length, 1);
        assertEquals("Check args", method.getMethodArguments()[0], "org.osgi.framework.ServiceReference");
		assertEquals("Check return", method.getMethodReturn(), "void");

        method = manip.getMethod("refBind", new String[] {"org.osgi.framework.ServiceReference"});
        assertEquals("Check args count", method.getMethodArguments().length, 1);
        assertEquals("Check args", method.getMethodArguments()[0], "org.osgi.framework.ServiceReference");
        assertEquals("Check return", method.getMethodReturn(), "void");
	}

	public void testTwoArgsMethod() {
	    PojoMetadata manip = SimpleMultipleCheckServiceProvider;
        MethodMetadata method = manip.getMethods("doNothing")[0];
        assertEquals("Check args count", 2, method.getMethodArguments().length);
		assertEquals("Check args - 1", method.getMethodArguments()[0], "java.lang.Object");
        assertEquals("Check args - 2", method.getMethodArguments()[1], "java.lang.String");
		assertEquals("Check return", method.getMethodReturn(), "java.lang.Object");

        method = manip.getMethod("doNothing", new String[] {"java.lang.Object", "java.lang.String"});
        assertEquals("Check args count", 2, method.getMethodArguments().length);
        assertEquals("Check args - 1", method.getMethodArguments()[0], "java.lang.Object");
        assertEquals("Check args - 2", method.getMethodArguments()[1], "java.lang.String");
        assertEquals("Check return", method.getMethodReturn(), "java.lang.Object");
	}

	public void testSuper() {
		String comp_name = "org.apache.felix.ipojo.test.scenarios.component.Child";
		PojoMetadata manip = getManipulationMetadataForComponent(comp_name);
		assertEquals("org.apache.felix.ipojo.test.scenarios.component.Parent", manip.getSuperClass());
		assertEquals(1, manip.getConstructors().length);
	}

	public void testConstructors() {
		String comp_name = "org.apache.felix.ipojo.test.scenarios.component.Multiconstructor";
		PojoMetadata manip = getManipulationMetadataForComponent(comp_name);
		assertEquals(3, manip.getConstructors().length);
		assertNotNull(manip.getConstructor(new String[] {String.class.getName(), String.class.getName()}));
		assertNotNull(manip.getConstructor(new String[] {String.class.getName(), String.class.getName(), Integer.TYPE.getName()}));
		assertNotNull(manip.getConstructor(new String[] {String.class.getName(), Integer.TYPE.getName()}));
		assertNull(manip.getConstructor(new String[] {String.class.getName()}));
	}


	private Element getMetadataForComponent(Element metadata, String comp_name) {
		Element[] comps = metadata.getElements("component");
		for(int i = 0; i < comps.length; i++) {
			if(comps[i].containsAttribute("factory") && comps[i].getAttribute("factory").equals(comp_name)) {
				return comps[i];
			}
            if(comps[i].containsAttribute("name") && comps[i].getAttribute("name").equals(comp_name)) {
                return comps[i];
            }
            if(comps[i].containsAttribute("classname") && comps[i].getAttribute("classname").equals(comp_name)) {
                return comps[i];
            }
		}
		return null;
	}


    private PojoMetadata getManipulationMetadataForComponent(String comp_name) {
        Element elem = null;
        try {
            elem = ManifestMetadataParser.parseHeaderMetadata(header);
        } catch (ParseException e) {
            e.printStackTrace();
            fail("Parse Exception when parsing iPOJO-Component " + e);
        }

        assertNotNull("Check elem not null", elem);

        Element manip = getMetadataForComponent(elem, comp_name);
        assertNotNull("Check manipulation metadata not null for " + comp_name, manip);
        try {
            return new PojoMetadata(manip);
        } catch (ConfigurationException e) {
            fail("The creation of pojo metadata for " + comp_name + " has failed");
            return null;
        }
    }

}
