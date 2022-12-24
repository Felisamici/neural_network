#include <stdio.h>
#include <stdlib.h>

#include "neural_network.h"
#include "utils.h"
#include "data_utils.h"

// Example function to approximate
double f(double *x)
{
    return (0.23 * *x * *x) + 3.2 * *x;
}

double f2(double *x)
{
    return (-123.78 * x[0]) + (648.25 * x[1]);
}

int main(void)
{
    srand(time(NULL));

    // define all parameters
    // for data
    const size_t DATA_SIZE = 20000;
    const size_t INPUT_SIZE = 2;
    const size_t OUTPUT_SIZE = 1;
    const size_t TEST_SIZE = 20;

    // for model
    const double learning_rate = 1e-8;
    const double momentum_constant = 0.2;
    const double initializer_mean = 0;
    const double initializer_stddev = 1;
    const int use_bias = 1;
    const size_t training_epochs = 25;
    const size_t batch_size = 1;

    // define model shape
    size_t layers_size[] = {16, 8, 1};
    double (*activations[])(double, int) = {&relu, &relu, &linear};

    struct neural_network *nn = create_model(3, layers_size, INPUT_SIZE, activations);
    randomize_weights(nn, initializer_mean, initializer_stddev, use_bias);

    // generate data
    double *inputs[DATA_SIZE];
    double *outputs[DATA_SIZE];

    generate_data_inputs(DATA_SIZE, INPUT_SIZE, inputs, -10, 10);
    generate_data_outputs(DATA_SIZE, OUTPUT_SIZE, inputs, outputs, &f2);

    // Start training !
    fit(nn, DATA_SIZE, inputs, outputs, training_epochs, batch_size, learning_rate, momentum_constant);

    for (size_t i = 0; i < DATA_SIZE; i++)
    {
        free(inputs[i]);
        free(outputs[i]);
    }

    double *test_inputs[TEST_SIZE];
    double *test_outputs[TEST_SIZE];

    generate_data_inputs(TEST_SIZE, INPUT_SIZE, test_inputs, -10, 10);
    generate_data_outputs(TEST_SIZE, OUTPUT_SIZE, test_inputs, test_outputs, &f2);

    evaluate(nn, TEST_SIZE, test_inputs, test_outputs, &mean_squared_error, 0);

    for (size_t i = 0; i < TEST_SIZE; i++)
    {
        free(test_inputs[i]);
        free(test_outputs[i]);
    }

    free_neural_network(nn);

    return EXIT_SUCCESS;
}