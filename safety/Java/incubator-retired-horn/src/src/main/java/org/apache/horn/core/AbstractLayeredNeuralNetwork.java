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
package org.apache.horn.core;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.List;

import org.apache.hadoop.io.WritableUtils;
import org.apache.hama.HamaConfiguration;
import org.apache.hama.commons.math.FloatFloatFunction;
import org.apache.hama.commons.math.FloatFunction;
import org.apache.hama.commons.math.FloatMatrix;
import org.apache.hama.commons.math.FloatVector;
import org.apache.horn.core.Constants.LearningStyle;
import org.apache.horn.core.Constants.TrainingMethod;
import org.apache.horn.funcs.CategoricalCrossEntropy;
import org.apache.horn.funcs.CrossEntropy;
import org.apache.horn.funcs.FunctionFactory;

import com.google.common.base.Preconditions;
import com.google.common.collect.Lists;

/**
 * AbstractLayeredNeuralNetwork defines the general operations for derivative
 * layered models, include Linear Regression, Logistic Regression, Multilayer
 * Perceptron, Autoencoder, and Restricted Boltzmann Machine, etc.
 * 
 * In general, these models consist of neurons which are aligned in layers.
 * Between layers, for any two adjacent layers, the neurons are connected to
 * form a bipartite weighted graph.
 * 
 */
abstract class AbstractLayeredNeuralNetwork extends AbstractNeuralNetwork {

  private static final float DEFAULT_REGULARIZATION_WEIGHT = 0;
  private static final float DEFAULT_MOMENTUM_WEIGHT = 0.1f;

  float trainingError;

  /* The weight of regularization */
  protected float regularizationWeight;

  /* The momentumWeight */
  protected float momentumWeight;

  /* The cost function of the model */
  protected FloatFloatFunction costFunction;

  /* Record the size of each layer */
  protected List<Integer> layerSizeList;

  protected TrainingMethod trainingMethod;

  protected LearningStyle learningStyle;

  protected float dropRate;

  public AbstractLayeredNeuralNetwork() {
    this.regularizationWeight = DEFAULT_REGULARIZATION_WEIGHT;
    this.momentumWeight = DEFAULT_MOMENTUM_WEIGHT;
    this.trainingMethod = TrainingMethod.GRADIENT_DESCENT;
    this.learningStyle = LearningStyle.SUPERVISED;
  }

  public AbstractLayeredNeuralNetwork(HamaConfiguration conf, String modelPath) {
    super(conf, modelPath);
    if (this.layerSizeList.get(this.layerSizeList.size() - 1) > 1
        && this.costFunction.getFunctionName().equalsIgnoreCase(
            CrossEntropy.class.getSimpleName())) {
      this.setCostFunction(new CategoricalCrossEntropy());
    }
  }

  /**
   * Set the regularization weight. Recommend in the range [0, 0.1), More
   * complex the model is, less weight the regularization is.
   * 
   * @param regularizationWeight
   */
  public void setRegularizationWeight(float regularizationWeight) {
    Preconditions.checkArgument(regularizationWeight >= 0
        && regularizationWeight < 1.0,
        "Regularization weight must be in range [0, 1.0)");
    this.regularizationWeight = regularizationWeight;
  }

  public float getRegularizationWeight() {
    return this.regularizationWeight;
  }

  /**
   * Set the momemtum weight for the model. Recommend in range [0, 0.5].
   * 
   * @param momentumWeight
   */
  public void setMomemtumWeight(float momentumWeight) {
    Preconditions.checkArgument(momentumWeight >= 0 && momentumWeight <= 1.0,
        "Momentum weight must be in range [0, 1.0]");
    this.momentumWeight = momentumWeight;
  }

  public float getMomemtumWeight() {
    return this.momentumWeight;
  }

  public void setTrainingMethod(TrainingMethod method) {
    this.trainingMethod = method;
  }

  public TrainingMethod getTrainingMethod() {
    return this.trainingMethod;
  }

  public void setLearningStyle(LearningStyle style) {
    this.learningStyle = style;
  }

  public LearningStyle getLearningStyle() {
    return this.learningStyle;
  }

  /**
   * Set the cost function for the model.
   * 
   * @param costFunction
   */
  public void setCostFunction(FloatFloatFunction costFunction) {
    this.costFunction = costFunction;
  }

  /**
   * Add a layer of neurons with specified size. If the added layer is not the
   * first layer, it will automatically connects the neurons between with the
   * previous layer.
   * 
   * @param size
   * @param isFinalLayer If false, add a bias neuron.
   * @param squashingFunction The squashing function for this layer, input layer
   *          is f(x) = x by default.
   * @return The layer index, starts with 0.
   */
  public abstract int addLayer(int size, boolean isFinalLayer,
      FloatFunction squashingFunction, Class<? extends Neuron<?>> neuronClass);

  /**
   * Get the size of a particular layer.
   * 
   * @param layer
   * @return The layer size.
   */
  public int getLayerSize(int layer) {
    Preconditions.checkArgument(
        layer >= 0 && layer < this.layerSizeList.size(),
        String.format("Input must be in range [0, %d]\n",
            this.layerSizeList.size() - 1));
    return this.layerSizeList.get(layer);
  }

  /**
   * Get the layer size list.
   * 
   * @return The layer size list.
   */
  protected List<Integer> getLayerSizeList() {
    return this.layerSizeList;
  }

  /**
   * Get the weights between layer layerIdx and layerIdx + 1
   * 
   * @param layerIdx The index of the layer
   * @return The weights in form of {@link floatMatrix}
   */
  public abstract FloatMatrix getWeightsByLayer(int layerIdx);

  /**
   * Get the updated weights using one training instance.
   * 
   * @param trainingInstance The trainingInstance is the concatenation of
   *          feature vector and class label vector.
   * @return The update of each weight, in form of matrix list.
   * @throws Exception
   */
  public abstract FloatMatrix[] trainByInstance(FloatVector trainingInstance);

  /**
   * Get the output calculated by the model.
   * 
   * @param instance The feature instance.
   * @return a new vector with the result of the operation.
   */
  public abstract FloatVector getOutput(FloatVector instance);

  /**
   * Calculate the training error based on the labels and outputs.
   * 
   * @param labels
   * @param output
   */
  protected abstract void calculateTrainingError(FloatVector labels,
      FloatVector output);

  @Override
  public void readFields(DataInput input) throws IOException {
    super.readFields(input);
    // read regularization weight
    this.regularizationWeight = input.readFloat();
    // read momentum weight
    this.momentumWeight = input.readFloat();

    // read cost function
    this.costFunction = FunctionFactory
        .createFloatFloatFunction(WritableUtils.readString(input));

    // read layer size list
    int numLayers = input.readInt();
    this.layerSizeList = Lists.newArrayList();
    for (int i = 0; i < numLayers; ++i) {
      this.layerSizeList.add(input.readInt());
    }

    this.trainingMethod = WritableUtils.readEnum(input, TrainingMethod.class);
    this.learningStyle = WritableUtils.readEnum(input, LearningStyle.class);
  }

  @Override
  public void write(DataOutput output) throws IOException {
    super.write(output);
    // write regularization weight
    output.writeFloat(this.regularizationWeight);
    // write momentum weight
    output.writeFloat(this.momentumWeight);

    // write cost function
    WritableUtils.writeString(output, costFunction.getFunctionName());

    // write layer size list
    output.writeInt(this.layerSizeList.size());
    for (Integer aLayerSizeList : this.layerSizeList) {
      output.writeInt(aLayerSizeList);
    }

    WritableUtils.writeEnum(output, this.trainingMethod);
    WritableUtils.writeEnum(output, this.learningStyle);
  }

  public void setDropRateOfInputLayer(float dropRate) {
    this.dropRate = dropRate;
  }

}
