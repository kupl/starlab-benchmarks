# Apache Horn

The Apache Horn is an Apache Incubating project, a neuron-centric programming model and Sync and Async hybrid distributed training framework, supports both data and model parallelism for training large models with massive datasets on top of Apache Hadoop and Hama.

## Programming Model

Apache Horn provides a neuron-centric programming model for implementing the neural network based algorithms. The user defines the computation that takes place at each neuron in each layer of the model, and the messages that should be passed during the forward and backward phases of computation. For example, we apply a set of weights to the input data and calculate an output in forward() method like below:
```Java
    @Override
    public void forward(
        Iterable<Synapse<FloatWritable, FloatWritable>> messages)
        throws IOException {
      float sum = 0;
      for (Synapse<FloatWritable, FloatWritable> m : messages) {
        sum += m.getInput() * m.getWeight();
      }
      this.feedforward(this.squashingFunction.apply(sum));
    }
```
Then, we measure the margin of error of the output and adjust the weights accordingly to decrease the error in backward() method:
```Java
    @Override
    public void backward(
        Iterable<Synapse<FloatWritable, FloatWritable>> messages)
        throws IOException {
      float delta = 0;
      for (Synapse<FloatWritable, FloatWritable> m : messages) {
        // Calculates error gradient for each neuron
        delta += (m.getDelta() * m.getWeight());

        // Weight corrections
        float weight = -this.getLearningRate() * this.getOutput()
            * m.getDelta() + this.getMomentumWeight() * m.getPrevWeight();
        this.push(weight);
      }

      this.backpropagate(delta
          * this.squashingFunction.applyDerivative(this.getOutput()));
    }
  }
```
The advantages of this programming model is easy and intuitive to use.

Also, Apache Horn provides a simplified and intuitive configuration interface. To create neural network job and submit it to existing Hadoop or Hama cluster, we just add the layer with its properties such as squashing function and neuron class. The below example configures the create 2-layer neural network for train MNIST dataset:
```Java
  HornJob job = new HornJob(conf, MultiLayerPerceptron.class);
  job.setLearningRate(learningRate);
  ..

  job.inputLayer(features, 0.8f); // droprate
  job.addLayer(hu, ReLU.class, DropoutNeuron.class);
  job.outputLayer(labels, SoftMax.class, StandardNeuron.class);
  job.setCostFunction(CrossEntropy.class);
```

## Quick Run Example

Download a MNIST training and label datasets, and convert into a HDFS sequence file with following command:
```
 % bin/horn jar horn-0.x.0.jar MNISTConverter \
   train-images.idx3-ubyte train-labels.idx1-ubyte /tmp/mnist.seq 
```

Then, train it with following command (in this example, we used η 0.01, α 0.9, λ 0.0005, 100 hidden units, and minibatch 10):
```
 % bin/horn jar horn-0.x.0.jar MultiLayerPerceptron /tmp/model /tmp/mnist.seq \
   0.01 0.9 0.0005 784 100 10 10 12000
```

With this default example, you'll reach over the 95% accuracy. In local mode, 20 tasks will train the model in synchronous parallel fashion and will took around 10 mins. 

## High Scalability

The Apache Horn is an Sync and Async hybrid distributed training framework. Within single BSP job, each task group works asynchronously using region barrier synchronization instead of global barrier synchronization, and trains large-scale neural network model using assigned data sets in synchronous way.

## Getting Involved

Horn is an open source volunteer project under the Apache Software Foundation. We encourage you to learn about the project and contribute your expertise.

