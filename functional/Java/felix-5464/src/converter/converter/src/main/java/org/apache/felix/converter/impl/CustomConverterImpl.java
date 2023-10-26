/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.felix.converter.impl;

import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import org.osgi.util.converter.ConversionException;
import org.osgi.util.converter.Converter;
import org.osgi.util.converter.ConverterBuilder;
import org.osgi.util.converter.ConverterFunction;
import org.osgi.util.converter.Converting;
import org.osgi.util.converter.Functioning;
import org.osgi.util.converter.TypeReference;

/**
 * A custom converter wraps another converter by adding rules and/or error handlers.
 */
class CustomConverterImpl implements InternalConverter {
    private final InternalConverter delegate;
    private final Map<Type, List<ConverterFunction>> typeRules;
    private final List<ConverterFunction> allRules;
    private final List<ConverterFunction> errorHandlers;

    CustomConverterImpl(InternalConverter converter, Map<Type, List<ConverterFunction>> rules,
            List<ConverterFunction> catchAllRules, List<ConverterFunction> errHandlers) {
        delegate = converter;
        typeRules = rules;
        allRules = catchAllRules;
        errorHandlers = errHandlers;
    }

    @Override
    public InternalConverting convert(Object obj) {
        InternalConverting converting = delegate.convert(obj);
        converting.setConverter(this);
        return new ConvertingWrapper(obj, converting);
    }

    @Override
    public Functioning function() {
        return new FunctioningImpl(this);
    }

    @Override
    public ConverterBuilder newConverterBuilder() {
        return new ConverterBuilderImpl(this);
    }

    private class ConvertingWrapper implements InternalConverting {
        private final InternalConverting del;
        private final Object object;
        private volatile Object defaultValue;
        private volatile boolean hasDefault;

        ConvertingWrapper(Object obj, InternalConverting c) {
            object = obj;
            del = c;
        }

        @Override
        public Converting copy() {
            del.copy();
            return this;
        }

        @Override
        public Converting defaultValue(Object defVal) {
            del.defaultValue(defVal);
            defaultValue = defVal;
            hasDefault = true;
            return this;
        }

        @Override
        public Converting keysIgnoreCase() {
            del.keysIgnoreCase();
            return this;
        }

        @Override
        public void setConverter(Converter c) {
            del.setConverter(c);
        }

        @Override
        public Converting sourceAs(Class<?> type) {
            del.sourceAs(type);
            return this;
        }

        @Override
        public Converting sourceAsBean() {
            del.sourceAsBean();
            return this;
        }

        @Override
        public Converting sourceAsDTO() {
            del.sourceAsDTO();
            return this;
        }

        @Override
        public Converting targetAs(Class<?> cls) {
            del.targetAs(cls);
            return this;
        }

        @Override
        public Converting targetAsBean() {
            del.targetAsBean();
            return this;
        }

        @Override
        public Converting targetAsDTO() {
            del.targetAsDTO();
            return this;
        }

        @SuppressWarnings("unchecked")
        @Override
        public <T> T to(Class<T> cls)  {
            Type type = cls;
            return (T) to(type);
        }

        @SuppressWarnings("unchecked")
        @Override
        public <T> T to(TypeReference<T> ref)  {
            return (T) to(ref.getType());
        }

        @SuppressWarnings("unchecked")
        @Override
        public Object to(Type type) {
            List<ConverterFunction> tr = typeRules.get(Util.baseType(type));
            if (tr == null)
                tr = Collections.emptyList();
            List<ConverterFunction> converters = new ArrayList<>(tr.size() + allRules.size());
            converters.addAll(tr);
            converters.addAll(allRules);

            try {
                if (object != null) {
                    for (ConverterFunction cf : converters) {
                        try {
                            Object res = cf.apply(object, type);
                            if (res != ConverterFunction.CANNOT_HANDLE) {
                                return res;
                            }
                        } catch (Exception ex) {
                            if (hasDefault)
                                return defaultValue;
                            else
                                throw new ConversionException("Cannot convert " + object + " to " + type, ex);
                        }
                    }
                }

                return del.to(type);
            } catch (Exception ex) {
                for (ConverterFunction eh : errorHandlers) {
                    try {
                        Object handled = eh.apply(object, type);
                        if (handled != ConverterFunction.CANNOT_HANDLE)
                            return handled;
                    } catch (RuntimeException re) {
                        throw re;
                    } catch (Exception e) {
                        throw new RuntimeException(e);
                    }
                }

                // No error handler, throw the original exception
                throw ex;
            }
        }

        @Override
        public String toString() {
            return to(String.class);
        }
    }
}
