#include <stdio.h>
#include <stdlib.h>

#include "neural_network.h"
#include "utils.h"

// Example function to approximate
float f(float *x)
{
    return (0.23 * *x * *x) + 3.2 * *x;
}

float f2(float *x)
{
    return (0.78 * x[0]) + (1.25 * x[1]);
}

int main(void)
{
    srand(time(NULL));

    // define all parameters
    // for data
    const size_t DATA_SIZE = 1000;
    const size_t INPUT_SIZE = 1;
    const size_t OUTPUT_SIZE = 1;
    const size_t TEST_SIZE = 20;

    // for model
    const float learning_rate = 1e-5;
    const float momentum_constant = 0.3;
    const float initializer_mean = 0;
    const float initializer_stddev = 0.5;
    const int use_bias = 1;
    const size_t training_epochs = 15;

    // define model shape
    size_t layers_size[] = {4, 3, 1};
    float (*activations[])(float, int) = {&relu, &relu, &linear};

    struct neural_network *nn = create_model(3, layers_size, INPUT_SIZE, activations);
    randomize_weights(nn, initializer_mean, initializer_stddev, use_bias);

    // generate data
    float *inputs[DATA_SIZE];
    float *outputs[DATA_SIZE];

    generate_data_inputs(DATA_SIZE, INPUT_SIZE, inputs, -25, 25);
    generate_data_outputs(DATA_SIZE, OUTPUT_SIZE, inputs, outputs, &f2);

    // Start training !
    fit(nn, DATA_SIZE, inputs, outputs, training_epochs, learning_rate, momentum_constant);

    for (size_t i = 0; i < DATA_SIZE; i++)
    {
        free(inputs[i]);
        free(outputs[i]);
    }

    float *test_inputs[TEST_SIZE];
    float *test_outputs[TEST_SIZE];

    generate_data_inputs(TEST_SIZE, INPUT_SIZE, test_inputs, -25, 25);
    generate_data_outputs(TEST_SIZE, OUTPUT_SIZE, test_inputs, test_outputs, &f2);

    evaluate(nn, TEST_SIZE, test_inputs, test_outputs, &mean_squared_error, 1);

    for (size_t i = 0; i < TEST_SIZE; i++)
    {
        free(test_inputs[i]);
        free(test_outputs[i]);
    }

    free_neural_network(nn);

    return EXIT_SUCCESS;
}