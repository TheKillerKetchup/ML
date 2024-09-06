#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include <vector>
#include <cstdint>
#include <iostream>

struct Neuron{
    std::vector<int> connections; //these connect to next layer, range: [-1,1]
    int non_linear_activation; //range: [-inf, inf]
    int activation; //range: [-1,1]
    int bias; 
};


struct Derivative{
    int layer_L;
    int neuron_j_index; 
    int neuron_k_index; 
    int bias; //neuron resides on layer L, at index = neuron_a //0 = weight, 1 = bias
    bool operator ==(const Derivative &Derivative2) const {
        return (
            layer_L == Derivative2.layer_L && //layer L corresponds to weight i
            neuron_j_index == Derivative2.neuron_j_index && 
            neuron_k_index == Derivative2.neuron_k_index && 
            bias == Derivative2.bias
        );
    }
};

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

class NeuralNetwork{
    public:
        NeuralNetwork(int number_of_layers, std::vector<int> neuron_count_per_layer);
        std::vector<int> feedForward(std::vector<uint8_t> image, int label); //is the n needed? 
        void backpropogate(std::vector<int> costs); //this is backpropogation!
        void calculateGradient(); //apply feed forward then backprop many times

    private:
        int number_of_layers;
        std::vector<int> neuron_count_per_layer;
        std::vector<std::vector<Neuron> > model; //model[a][b] -> at layer a, neuron b
        std::vector<std::vector<int> > cost_array; //cost_array[a][b] -> at layer a, neuron b has {value} cost //this can be greater than 1
        std::vector<std::vector<std::vector<int> > > Gradient;
        //DEFINITION:Gradient[a][b][c] = gradient of connection at layer a, between neuron b(on layer l), and neuron c(on layer l+1)
        //if c is (b's # of weights + 1), it is a bias not a weight, and does not connect


};

#endif