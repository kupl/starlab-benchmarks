/*
 * Copyright (c) OSGi Alliance (2016). All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.felix.serializer;

import java.io.InputStream;
import java.nio.charset.Charset;

import org.osgi.annotation.versioning.ProviderType;
import org.osgi.util.converter.Converter;

/**
 * Interface to specify the source of the decoding operation
 *
 * @param <T> The target type for the decoding operation.
 * @author $Id$
 * @ThreadSafe
 */
@ProviderType
public interface Deserializing<T> {
	/**
	 * Use an input stream as the source of the decoding operation. As encoding
	 * UTF-8 is used.
	 *
	 * @param in The stream to use.
	 * @return the decoded object.
	 */
	T from(InputStream in);

	/**
	 * Use an input stream as the source of the decoding operation.
	 *
	 * @param in The stream to use.
	 * @param charset The character set to use.
	 * @return the decoded object.
	 */
	T from(InputStream in, Charset charset);

	/**
	 * Use a Readable as the source of the decoding operation.
	 *
	 * @param in The readable to use.
	 * @return the decoded object.
	 */
	T from(Readable in);

	/**
	 * Use a Char Sequence as the source of the decoding operation.
	 *
	 * @param in The char sequence to use.
	 * @return the decoded object.
	 */
	T from(CharSequence in);

	/**
	 * Specify the converter to be used by the code, if an alternative, adapted,
	 * converter is to be used.
	 *
	 * @param converter The converter to use.
	 * @return This Deserializing object to allow further invocations on it.
	 */
	Deserializing<T> convertWith(Converter converter);

    /**
     * Specify the parser to be used, if an alternative to the default internal
     * parser is required.
     * 
     * @param parser the parser to use.
     * @return This Deserializing object to allow further invocations on it.
     */
	Deserializing<T> parseWith(Parser parser);
}
