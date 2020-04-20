/*
	CSC D84 - Unit 4 - Neural Networks
	
	This file contains stubs for implementing your Neural Nets classifier.
	
	You will implement a single layer network, and 2-layer network,
	and you will study what is the effect of having different numbers
	of neurons in the network, as well as what happens when you used
	different sigmoid functions.
	    
	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.
	
	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Feb. 16
*/

#include "NeuralNets.h"
#define EXPLODING_THRESHOLD 100000.0

double generate_reset_value()
{
  /**
   * Generates a psuedo random value between (-1, 1).
   * This attempts to simulate the value of a neuron when it is reset.
   */

  double base = rand();
  double midpoint = RAND_MAX / 2.0;
  return (base / midpoint) - 1.0;
}

int train_1layer_net(double sample[INPUTS], int label, double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS])
{
  /*
  *   This is your main training function for 1-layer networks. Recall from lecture that we have a simple,
  *  direct connection between inputs and output neurons (the only layer present here). What we are doing
  *  in effect is training 10 different classifiers, each of which will learn to distinguish one of our
  *  training digits.
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   weights_io - Array of weights connecting inputs to output neurons, weights[i][j] is the weight from input
  *                i to output neuron j. This array has a size of 785x10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  double activations[OUTPUTS];
  feedforward_1layer(sample, sigmoid, weights_io, activations);

  int classify = 0; // Retrieve argmax from the resulting activations after feedforward.
  for (int i = 1; i < OUTPUTS; i++)
    if (activations[i] > activations[classify])
      classify = i;

  backprop_1layer(sample, activations, sigmoid, label, weights_io);
  return classify;
}

int classify_1layer(double sample[INPUTS], int label, double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS])
{
  /*
  *   This function classifies an input sample given the current network weights. It returns a class in
  *  [0,9] corresponding to the digit the network has decided is present in the input sample
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   weights_io - Array of weights connecting inputs to output neurons, weights[i][j] is the weight from input
  *                i to output neuron j. This array has a size of 785x10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  double activations[OUTPUTS];
  feedforward_1layer(sample, sigmoid, weights_io, activations);

  int classify = 0; // Retrieve argmax from the resulting activations after feedforward.
  for (int i = 1; i < OUTPUTS; i++)
    if (activations[i] > activations[classify])
      classify = i;

  return classify;
}

void feedforward_1layer(double sample[785], double (*sigmoid)(double input), double weights_io[INPUTS][OUTPUTS], double activations[OUTPUTS])
{
  /*
  *  This function performs the feedforward pass of the network's computation - it propagates information
  *  from input to output, determines the input to each neuron, and calls the sigmoid function to
  *  calculate neuron activation.
  * 
  *  Inputs:
  *    sample -      The input sample (see above for a description)
  *    sigmoid -     The sigmoid function being used
  *    weights_op -  Array of current network weights
  *    activations - Array where your function will store the resulting activation for each output neuron
  * 
  *  Return values:
  *    Your function must update the 'activations' array with the output value for each neuron
  * 
  *  NOTE - You must *scale* the input to the sigmoid function using the SIGMOID_SCALE value. Otherwise
  *         the neurons will be totally saturated and learning won't happen.
  */

  // Variables that we need to use.
  int i, j;

  for (i = 0; i < OUTPUTS; i++)
  {
    double weighted_sum = 0.0; // Calculate the activation A(I) of each output neuron.
    for (j = 0; j < INPUTS; j++)
      weighted_sum += weights_io[j][i] * sample[j];
    activations[i] = sigmoid(SIGMOID_SCALE * weighted_sum); // Pass the result through the activation function f(x).
  }
}

void backprop_1layer(double sample[INPUTS], double activations[OUTPUTS], double (*sigmoid)(double input), int label, double weights_io[INPUTS][OUTPUTS])
{
  /*
   *  This function performs the core of the learning process for 1-layer networks. It takes
   *  as input the feed-forward activation for each neuron, the expected label for this training
   *  sample, and the weights array. Then it updates the weights in the array so as to minimize
   *  error across neuron outputs.
   * 
   *  Inputs:
   * 	sample - 	Input sample (see above for details)
   *    activations - 	Neuron outputs as computed above
   *    sigmoid -	Sigmoid function in use
   *    label - 	Correct class for this sample
   *    weights_io -	Network weights
   * 
   *  You have to:
   * 		* Determine the target value for each neuron
   * 			- This depends on the type of sigmoid being used, you should think about
   * 			  this: What should the neuron's output be if the neuron corresponds to
   * 			  the correct label, and what should the output be for every other neuron?
   * 		* Compute an error value given the neuron's target
   * 		* Compute the weight adjustment for each weight (the learning rate is in NeuralNets.h)
   */

  // This integer determines whether or not the activation function is the logistic function or the hyperbolic tangent function.
  int is_logistic = (sigmoid == logistic);

  // Other variables that we need to use.
  int i, j;
  double target, gradient, weighted_sum;

  for (i = 0; i < OUTPUTS; i++)
  {
    // Calculate the gradient from the error.
    target = (label == i) ? 1.0 : 0.0;
    gradient = target - activations[i];

    // Calculate the gradient from the output.
    if (is_logistic)
      gradient *= (activations[i] * (1 - activations[i]));
    else
      gradient *= (1 - pow(activations[i], 2));

    for (j = 0; j < INPUTS; j++) // Calculate the gradient from the activation.
    {
      weights_io[j][i] += (ALPHA * sample[j] * gradient);

      if (fabs(weights_io[j][i]) >= EXPLODING_THRESHOLD) // Reset the weight if we find an "exploding" threshold.
        weights_io[j][i] = generate_reset_value();
    }
  }
}

int train_2layer_net(double sample[INPUTS], int label, double (*sigmoid)(double input), int units, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS])
{
  /*
  *   This is your main training function for 2-layer networks. Now you have to worry about the hidden
  *  layer at this time. *Do not work on this until you have completed the 1-layer network*.
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   units   -  Number of units in the hidden layer
  *   weights_ih - Array of weights connecting inputs to hidden-layer neurons, weights_ih[i][j] is the 
  *                weight from input i to hidden neuron j. This array has a size of units 785 x 10.
  *   weights_ho - Array of weights connecting hidden-layer units to output neurons, weights_ho[i][j] is the 
  *                weight from hidden unit i to output neuron j. This array has a size of units x 10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  double h_activations[MAX_HIDDEN];
  double activations[OUTPUTS];
  feedforward_2layer(sample, sigmoid, weights_ih, weights_ho, h_activations, activations, units);

  int classify = 0; // Retrieve argmax from the resulting activations after feedforward.
  for (int i = 1; i < OUTPUTS; i++)
    if (activations[i] > activations[classify])
      classify = i;

  backprop_2layer(sample, h_activations, activations, sigmoid, label, weights_ih, weights_ho, units);
  return classify;
}

int classify_2layer(double sample[INPUTS], int label, double (*sigmoid)(double input), int units, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS])
{
  /*
  *   This function takes an input sample and classifies it using the current network weights. It returns
  *  an int in [0,9] corresponding to which digit the network thinks is present in the input sample.
  * 
  *  Inputs:
  *   sample  -  Array with the pixel values for the input digit - in this case a 28x28 image (784 pixels)
  *              with values in [0-255], plus one bias term (last entry in the array) which is always 1
  *   label  -   Correct label for this digit (our target class)
  *   sigmoid -  The sigmoid function being used, which will be either the logistic function or the hyperbolic
  *              tangent. You have to implement the logistic function, but math.h provides tanh() already
  *   units   -  Number of units in the hidden layer
  *   weights_ih - Array of weights connecting inputs to hidden-layer neurons, weights_ih[i][j] is the 
  *                weight from input i to hidden neuron j. This array has a size of units 785 x 10.
  *   weights_ho - Array of weights connecting hidden-layer units to output neurons, weights_ho[i][j] is the 
  *                weight from hidden unit i to output neuron j. This array has a size of units x 10.
  *
  *   Return values:
  *     An int in [0,9] corresponding to the class that your current network has chosen for this training
  *   sample.
  * 
  */

  double h_activations[MAX_HIDDEN];
  double activations[OUTPUTS];
  feedforward_2layer(sample, sigmoid, weights_ih, weights_ho, h_activations, activations, units);

  int classify = 0; // Retrieve argmax from the resulting activations after feedforward.
  for (int i = 1; i < OUTPUTS; i++)
    if (activations[i] > activations[classify])
      classify = i;

  return classify;
}

void feedforward_2layer(double sample[INPUTS], double (*sigmoid)(double input), double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS], double h_activations[MAX_HIDDEN], double activations[OUTPUTS], int units)
{
  /*
  *  Here, implement the feedforward part of the two-layer network's computation.
  * 
  *  Inputs:
  *    sample -      The input sample (see above for a description)
  *    sigmoid -     The sigmoid function being used
  *    weights_ih -  Array of current input-to-hidden weights
  *    weights_ho -  Array of current hidden-to-output weights
  *    h_activations - Array of hidden layer unit activations
  *    activations   - Array of activations for output neurons
  *    units -         Number of units in the hidden layer
  * 
  *  Return values:
  *    Your function must update the 'activations' and 'h_activations' arrays with the output values for each neuron
  * 
  *  NOTE - You must *scale* the input to the sigmoid function using the SIGMOID_SCALE value. Otherwise
  *         the neurons will be totally saturated and learning won't happen.
  */

  // These represent the scaling of the A(I) for the hidden layer neurons and the output layer neurons.
  const double scale_hidden = SIGMOID_SCALE;
  const double scale_output = SIGMOID_SCALE * (MAX_HIDDEN / ((double)units));

  // Other variables that we need to use.
  int i, j;
  double weighted_sum;

  for (i = 0; i < MAX_HIDDEN; i++) // Calculate 'h_activations' for the hidden layer.
  {
    weighted_sum = 0.0; // Calculate the activation A(I) of each hidden neuron.
    for (j = 0; j < INPUTS; j++)
      weighted_sum += weights_ih[j][i] * sample[j];
    h_activations[i] = sigmoid(scale_hidden * weighted_sum); // Pass the result through the activation function f(x).
  }

  for (i = 0; i < OUTPUTS; i++) // Calculate 'activations' for the output layer.
  {
    weighted_sum = 0.0; // Calculate the activation A(I) of each output neuron.
    for (j = 0; j < MAX_HIDDEN; j++)
      weighted_sum += weights_ho[j][i] * h_activations[j];
    activations[i] = sigmoid(scale_output * weighted_sum); // Pass the result through the activation function f(x).
  }
}

void backprop_2layer(double sample[INPUTS], double h_activations[MAX_HIDDEN], double activations[OUTPUTS], double (*sigmoid)(double input), int label, double weights_ih[INPUTS][MAX_HIDDEN], double weights_ho[MAX_HIDDEN][OUTPUTS], int units)
{
  /*
   *  This function performs the core of the learning process for 2-layer networks. It performs
   *  the weights update as discussed in lecture. Note that you require the current weights
   *  between the hidden and output layers in order to update the weights from input to hidden,
   *  however the backprop. algorithm would have you update that weight first. So mind the order
   *  of updates and keep track of what you need.
   * 
   *  Inputs:
   * 	sample - 	Input sample (see above for details)
   *    h_activations - Hidden-layer activations
   *    activations -   Output-layer activations
   *    sigmoid -	Sigmoid function in use
   *    label - 	Correct class for this sample
   *    weights_ih -	Network weights from inputs to hidden layer
   *    weights_ho -    Network weights from hidden layer to output layer
   *    units -         Number of units in the hidden layer
   * 
   *  You have to:
   * 		* Determine the target value for each neuron
   * 			- This depends on the type of sigmoid being used, you should think about
   * 			  this: What should the neuron's output be if the neuron corresponds to
   * 			  the correct label, and what should the output be for every other neuron?
   * 		* Compute an error value given the neuron's target
   * 		* Compute the weight adjustment for each weight (the learning rate is in NeuralNets.h)
   */

  // These represent the scaling of the A(I) for the hidden layer neurons and the output layer neurons.
  const double scale_hidden = SIGMOID_SCALE;
  const double scale_output = SIGMOID_SCALE * (MAX_HIDDEN / ((double)units));

  // This integer determines whether or not the activation function is the logistic function or the hyperbolic tangent function.
  int is_logistic = (sigmoid == logistic);

  // We store the gradients of the output layer. We need to use it twice.
  double output_graidents[OUTPUTS];

  // Other variables that we need to use.
  int i, j;
  double target, gradient;

  for (i = 0; i < OUTPUTS; i++) // Backpropagate from the output layer.
  {
    // Calculate the gradient from the error.
    target = (label == i) ? 1.0 : 0.0;
    gradient = target - activations[i];

    // Calculate the gradient from the output.
    if (is_logistic)
      gradient *= (activations[i] * (1 - activations[i]));
    else
      gradient *= (1 - pow(activations[i], 2));

    output_graidents[i] = gradient;

    for (j = 0; j < MAX_HIDDEN; j++) // Calculate the gradient from the activation.
    {
      weights_ho[j][i] += (ALPHA * h_activations[j] * gradient);

      if (fabs(weights_ho[j][i]) >= EXPLODING_THRESHOLD) // Reset the weight if we find an "exploding" threshold.
        weights_ho[j][i] = generate_reset_value();
    }
  }

  for (i = 0; i < MAX_HIDDEN; i++) // Backpropagate from the hidden layer.
  {
    // Calculate the gradient from the error.
    gradient = 0.0;
    for (j = 0; j < OUTPUTS; j++)
      gradient += weights_ho[i][j] * output_graidents[j];

    // Calculate the gradient from the output.
    if (is_logistic)
      gradient *= (h_activations[i] * (1 - h_activations[i]));
    else
      gradient *= (1 - pow(h_activations[i], 2));

    for (j = 0; j < INPUTS; j++) // Calculate the gradient from the activation.
    {
      weights_ih[j][i] += (ALPHA * sample[j] * gradient);

      if (fabs(weights_ih[j][i]) >= EXPLODING_THRESHOLD) // Reset the weight if we find an "exploding" threshold.
        weights_ih[j][i] = generate_reset_value();
    }
  }
}

double logistic(double input)
{
  /**
   * This function returns the value of the logistic function evaluated on input
   */
  return (1 / (1 + exp(-input)));
}

double logistic_inverse(double input)
{
  /**
   * This function returns the value of the logistic inverse function evaluated on input
   */
  return log(input / (1 - input));
}
