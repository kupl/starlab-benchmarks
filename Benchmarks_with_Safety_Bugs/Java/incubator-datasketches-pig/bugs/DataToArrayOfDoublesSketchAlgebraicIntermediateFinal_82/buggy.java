/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.apache.datasketches.pig.tuple;

import static org.apache.datasketches.Util.DEFAULT_NOMINAL_ENTRIES;

import java.io.IOException;

import org.apache.datasketches.memory.Memory;
import org.apache.datasketches.tuple.ArrayOfDoublesSetOperationBuilder;
import org.apache.datasketches.tuple.ArrayOfDoublesSketches;
import org.apache.datasketches.tuple.ArrayOfDoublesUnion;
import org.apache.datasketches.tuple.ArrayOfDoublesUpdatableSketch;
import org.apache.datasketches.tuple.ArrayOfDoublesUpdatableSketchBuilder;
import org.apache.log4j.Logger;
import org.apache.pig.EvalFunc;
import org.apache.pig.data.DataBag;
import org.apache.pig.data.DataByteArray;
import org.apache.pig.data.Tuple;

/**
 * Class used to calculate the intermediate pass (combiner) or the final pass
 * (reducer) of an Algebraic sketch operation. This may be called multiple times
 * (from the mapper and from the reducer). It will receive a bag of values
 * returned by either the Intermediate stage or the Initial stages, so
 * it needs to be able to differentiate between and interpret both types.
 */
abstract class DataToArrayOfDoublesSketchAlgebraicIntermediateFinal extends EvalFunc<Tuple> {
  private final int sketchSize_;
  private final float samplingProbability_;
  private final int numValues_;
  private boolean isFirstCall_ = true;

  DataToArrayOfDoublesSketchAlgebraicIntermediateFinal() {
    this(DEFAULT_NOMINAL_ENTRIES, 1);
  }

  DataToArrayOfDoublesSketchAlgebraicIntermediateFinal(final int numValues) {
    this(DEFAULT_NOMINAL_ENTRIES, numValues);
  }

  DataToArrayOfDoublesSketchAlgebraicIntermediateFinal(final int sketchSize, final int numValues) {
    this(sketchSize, 1f, numValues);
  }

  DataToArrayOfDoublesSketchAlgebraicIntermediateFinal(
      final int sketchSize, final float samplingProbability, final int numValues) {
    sketchSize_ = sketchSize;
    samplingProbability_ = samplingProbability;
    numValues_ = numValues;
  }

  @Override
public org.apache.pig.data.Tuple exec(final org.apache.pig.data.Tuple inputTuple) throws java.io.IOException {
    if (isFirstCall_) {
        // this is to see in the log which way was used by Pig
        org.apache.log4j.Logger.getLogger(getClass()).info("algebraic is used");
        isFirstCall_ = false;
    }
    final org.apache.datasketches.tuple.ArrayOfDoublesUnion union = new org.apache.datasketches.tuple.ArrayOfDoublesSetOperationBuilder().setNominalEntries(sketchSize_).setNumberOfValues(numValues_).buildUnion();
    final org.apache.pig.data.DataBag bag = ((org.apache.pig.data.DataBag) (inputTuple.get(0)));
    {
        for (final org.apache.pig.data.Tuple dataTuple : /* NPEX_NULL_EXP */
        bag) {
            final java.lang.Object item = dataTuple.get(0);
            if (item instanceof org.apache.pig.data.DataBag) {
                // this is a bag from the Initial function.
                // just insert each item of the tuple into the sketch
                final org.apache.datasketches.tuple.ArrayOfDoublesUpdatableSketch sketch = new org.apache.datasketches.tuple.ArrayOfDoublesUpdatableSketchBuilder().setNominalEntries(sketchSize_).setSamplingProbability(samplingProbability_).setNumberOfValues(numValues_).build();
                org.apache.datasketches.pig.tuple.DataToArrayOfDoublesSketchBase.updateSketch(((org.apache.pig.data.DataBag) (item)), sketch, numValues_);
                union.update(sketch);
            } else if (item instanceof org.apache.pig.data.DataByteArray) {
                // This is a sketch from a prior call to the
                // Intermediate function. merge it with the
                // current sketch.
                final org.apache.pig.data.DataByteArray dba = ((org.apache.pig.data.DataByteArray) (item));
                union.update(org.apache.datasketches.tuple.ArrayOfDoublesSketches.wrapSketch(org.apache.datasketches.memory.Memory.wrap(dba.get())));
            } else {
                // we should never get here.
                throw new java.lang.IllegalArgumentException("InputTuple.Field0: Bag contains unrecognized types: " + item.getClass().getName());
            }
        }
        return org.apache.datasketches.pig.tuple.Util.tupleFactory.newTuple(new org.apache.pig.data.DataByteArray(union.getResult().toByteArray()));
    }
}
}
