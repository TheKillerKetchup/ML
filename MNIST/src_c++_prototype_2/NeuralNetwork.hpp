#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include <vector>
#include <cstdint>
#include <iostream>

struct Neuron{
    int layer;
    int index;
    std::vector<int> connections; //these connect to prev layer
    int activation;
};

struct Gradient{
    int layer_L;
    int neuron_i_index; 
    int neuron_j_index; 
    int bias; //neuron resides on layer a, at index = neuron_a //0 = weight, 1 = bias
    bool operator ==(const Gradient &Gradient2) const {
        return (
            layer_L == Gradient2.layer_L && //layer L corresponds to weight i
            neuron_i_index == Gradient2.neuron_i_index && 
            neuron_j_index == Gradient2.neuron_j_index && 
            bias == Gradient2.bias
        );
    }
};

struct GradientHasher{
    std::size_t operator()(const Gradient& g) const {
        using std::size_t;
        using std::hash;

        return (((std::hash<int>()(g.layer_L)) ^ 
            (std::hash<int>()(g.neuron_i_index) << 1)) >> 1) ^ 
            (std::hash<int>()(g.neuron_j_index) << 1) ^ 
            (std::hash<int>()(g.bias) << 1); 
    }
};

class NeuralNetwork{
    public:
        NeuralNetwork(int number_of_layers, std::vector<int> neuron_count_per_layer);
        std::vector<int> feedForward(std::vector<uint8_t> image, int label); //is the n needed? 
        void backPropogate(std::vector<int> costs);
        std::vector<int> calculateCost(std::vector<std::vector<uint8_t> > images, std::vector<int> labels);
        Gradient calculateGradient(std::vector<int> averageCost);
            //notes on gradient
            //gradient[a][b][c][d][e] -> the connection from layer a to b, of neuron c to d, being a (e=1) -> weight, 
                //(e=0) -> bias, should change by that amount,
                
            //only needs average cost b/c it has direct access to neurons
        /*
        calculate gradient and then 
        some math functions, not sure? 
        */
        int calculateCostWeight(int layer, int neuron_i_index, int neuron_j_index);
        int calculateCostBias(int layer, int neuron_i_index);
        int calculateCostActivation(int layer, int neuron_i_index);

    private:
        int number_of_layers;
        std::vector<int> neuron_count_per_layer;
        std::vector<std::vector<Neuron> > model;
            //model[a][b] -> at layer a, neuron b
};

#endif