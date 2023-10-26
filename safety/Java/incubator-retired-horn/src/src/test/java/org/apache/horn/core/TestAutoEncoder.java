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

import static org.junit.Assert.assertEquals;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hama.HamaConfiguration;
import org.apache.hama.commons.io.FloatVectorWritable;
import org.apache.hama.commons.io.VectorWritable;
import org.apache.hama.commons.math.DenseFloatVector;
import org.apache.hama.commons.math.FloatVector;
import org.junit.Test;
import org.mortbay.log.Log;

/**
 * Test the functionality of {@link AutoEncoder}.
 * 
 */
public class TestAutoEncoder extends MLTestBase {
  //TODO need to fix
/*
  @Test
  public void testAutoEncoderSimple() {
    float[][] instances = { { 0, 0, 0, 1 }, { 0, 0, 1, 0 }, { 0, 1, 0, 0 },
        { 0, 0, 0, 0 } };
    AutoEncoder encoder = new AutoEncoder(4, 2);
    // TODO use the configuration

    // encoder.setLearningRate(0.5);
    // encoder.setMomemtumWeight(0.2);

    int maxIteration = 2000;
    Random rnd = new Random();
    for (int iteration = 0; iteration < maxIteration; ++iteration) {
      for (int i = 0; i < instances.length; ++i) {
        encoder.trainOnline(new DenseFloatVector(instances[rnd
            .nextInt(instances.length)]));
      }
    }

    for (int i = 0; i < instances.length; ++i) {
      FloatVector encodeVec = encoder.encode(new DenseFloatVector(
          instances[i]));
      FloatVector decodeVec = encoder.decode(encodeVec);
      for (int d = 0; d < instances[i].length; ++d) {
        assertEquals(instances[i][d], decodeVec.get(d), 0.1);
      }
    }

  }

  @Test
  public void testAutoEncoderSwissRollDataset() {
    List<float[]> instanceList = new ArrayList<float[]>();
    try {
      BufferedReader br = new BufferedReader(new FileReader(
          "src/test/resources/dimensional_reduction.txt"));
      String line = null;
      while ((line = br.readLine()) != null) {
        String[] tokens = line.split("\t");
        float[] instance = new float[tokens.length];
        for (int i = 0; i < instance.length; ++i) {
          instance[i] = Float.parseFloat(tokens[i]);
        }
        instanceList.add(instance);
      }
      br.close();
      // normalize instances
      zeroOneNormalization(instanceList, instanceList.get(0).length);
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    } catch (NumberFormatException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    }

    List<FloatVector> vecInstanceList = new ArrayList<FloatVector>();
    for (float[] instance : instanceList) {
      vecInstanceList.add(new DenseFloatVector(instance));
    }
    AutoEncoder encoder = new AutoEncoder(3, 2);
    // encoder.setLearningRate(0.05);
    // encoder.setMomemtumWeight(0.1);
    int maxIteration = 2000;
    for (int iteration = 0; iteration < maxIteration; ++iteration) {
      for (FloatVector vector : vecInstanceList) {
        encoder.trainOnline(vector);
      }
    }

    double errorInstance = 0;
    for (FloatVector vector : vecInstanceList) {
      FloatVector decoded = encoder.getOutput(vector);
      FloatVector diff = vector.subtract(decoded);
      double error = diff.dot(diff);
      if (error > 0.1) {
        ++errorInstance;
      }
    }
    Log.info(String.format("Autoecoder error rate: %f%%\n", errorInstance * 100
        / vecInstanceList.size()));

  }

  @Test
  public void testAutoEncoderSwissRollDatasetDistributed() {
    HamaConfiguration conf = new HamaConfiguration();
    String strDataPath = "/tmp/dimensional_reduction.txt";
    Path path = new Path(strDataPath);
    List<float[]> instanceList = new ArrayList<float[]>();
    try {
      FileSystem fs = FileSystem.get(new URI(strDataPath), conf);
      if (fs.exists(path)) {
        fs.delete(path, true);
      }

      String line = null;
      BufferedReader br = new BufferedReader(new FileReader(
          "src/test/resources/dimensional_reduction.txt"));
      while ((line = br.readLine()) != null) {
        String[] tokens = line.split("\t");
        float[] instance = new float[tokens.length];
        for (int i = 0; i < instance.length; ++i) {
          instance[i] = Float.parseFloat(tokens[i]);
        }
        instanceList.add(instance);
      }
      br.close();
      // normalize instances
      zeroOneNormalization(instanceList, instanceList.get(0).length);

      SequenceFile.Writer writer = new SequenceFile.Writer(fs, conf, path,
          LongWritable.class, VectorWritable.class);
      for (int i = 0; i < instanceList.size(); ++i) {
        FloatVector vector = new DenseFloatVector(instanceList.get(i));
        writer.append(new LongWritable(i), new FloatVectorWritable(vector));
      }

      writer.close();
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    } catch (URISyntaxException e) {
      e.printStackTrace();
    }

    AutoEncoder encoder = new AutoEncoder(3, 2);
    String modelPath = "/tmp/autoencoder-modelpath";
    encoder.setModelPath(modelPath);
    Map<String, String> trainingParams = new HashMap<String, String>();
    // encoder.setLearningRate(0.5);
    trainingParams.put("tasks", "5");
    trainingParams.put("training.max.iterations", "3000");
    trainingParams.put("training.batch.size", "200");
    // encoder.train(conf, path, trainingParams);

    double errorInstance = 0;
    for (float[] instance : instanceList) {
      FloatVector vector = new DenseFloatVector(instance);
      FloatVector decoded = encoder.getOutput(vector);
      FloatVector diff = vector.subtract(decoded);
      double error = diff.dot(diff);
      if (error > 0.1) {
        ++errorInstance;
      }
    }
    Log.info(String.format("Autoecoder error rate: %f%%\n", errorInstance * 100
        / instanceList.size()));
  }
*/
}
