#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include <vector>
#include <cstdint>
#include <iostream>

struct Neuron{
    std::vector<int> connections; //these connect to next layer, range: [-1,1]
    float non_linear_activation; //range: [-inf, inf]
    float activation; //range: [-1,1]
    float bias; 
};
/*
struct GradientHasher{
    std::size_t operator()(const Derivative& g) const {
        using std::size_t;
        using std::hash;

        return (((std::hash<int>()(g.layer_L)) ^ 
            (std::hash<int>()(g.neuron_j_index) << 1)) >> 1) ^ 
            (std::hash<int>()(g.neuron_k_index) << 1) ^ 
            (std::hash<int>()(g.bias) << 1); 
    }
};
*/

class NeuralNetwork{
    public:
        NeuralNetwork(int number_of_layers, std::vector<int> neuron_count_per_layer);
        void feedForward(std::vector<uint8_t> image, int label); //is the n needed? 
        void backpropogate(int expected); //this is backpropogation!
        void train(std::vector<std::vector<uint8_t> > images, std::vector<uint8_t> labels, int stepSize); //if there's less images than the step size available, simply take that many? 
        float test(std::vector<std::vector<uint8_t> > images, std::vector<uint8_t> labels); //returns accuracy?
        void applyGradient(int divisor);
        float sigmoid(float input); //weak sigmoid!!
        float derivative_sigmoid(float input);

    private:
        int sigmoid_power;
        int number_of_layers;
        std::vector<int> neuron_count_per_layer;
        std::vector<std::vector<Neuron> > model; //model[a][b] -> at layer a, neuron b
        std::vector<std::vector<float> > cost_array; //cost_array[a][b] -> at layer a, neuron b has {value} cost //this can be greater than 1
        std::vector<std::vector<std::vector<float> > > Gradient;
        //DEFINITION:Gradient[a][b][c] = gradient of connection at layer a, between neuron b(on layer l), and neuron c(on layer l+1)
        //if c is (b's # of weights + 1), it is a bias not a weight, and does not connect


};

#endif