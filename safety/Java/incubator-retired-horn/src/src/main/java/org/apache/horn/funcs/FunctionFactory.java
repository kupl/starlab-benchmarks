/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.horn.funcs;

import org.apache.hama.commons.math.FloatFloatFunction;
import org.apache.hama.commons.math.FloatFunction;

/**
 * Factory to create the functions.
 * 
 */
public class FunctionFactory {

  /**
   * Create a double function with specified name.
   * 
   * @param functionName
   * @return an appropriate double function.
   */
  public static FloatFunction createFloatFunction(String functionName) {
    if (functionName.equalsIgnoreCase(Sigmoid.class.getSimpleName())) {
      return new Sigmoid();
    } else if (functionName.equalsIgnoreCase(Tanh.class.getSimpleName())) {
      return new Tanh();
    } else if (functionName.equalsIgnoreCase(ReLU.class.getSimpleName())) {
      return new ReLU();
    } else if (functionName.equalsIgnoreCase(SoftMax.class.getSimpleName())) {
      return new SoftMax();
    } else if (functionName.equalsIgnoreCase(IdentityFunction.class
        .getSimpleName())) {
      return new IdentityFunction();
    }

    throw new IllegalArgumentException(String.format(
        "No double function with name '%s' exists.", functionName));
  }

  /**
   * Create a double double function with specified name.
   * 
   * @param functionName
   * @return an appropriate double double function.
   */
  public static FloatFloatFunction createFloatFloatFunction(
      String functionName) {
    if (functionName.equalsIgnoreCase(SquaredError.class.getSimpleName())) {
      return new SquaredError();
    } else if (functionName
        .equalsIgnoreCase(CrossEntropy.class.getSimpleName())) {
      return new CrossEntropy();
    } else if (functionName
        .equalsIgnoreCase(CategoricalCrossEntropy.class.getSimpleName())) {
      return new CategoricalCrossEntropy();
    } 

    throw new IllegalArgumentException(String.format(
        "No double double function with name '%s' exists.", functionName));
  }

}