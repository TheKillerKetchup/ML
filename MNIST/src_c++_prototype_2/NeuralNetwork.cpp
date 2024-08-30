#include <iostream>
#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(int number_of_layers, std::vector<int> neuron_count_per_layer){
    this->number_of_layers = number_of_layers;
    this->neuron_count_per_layer = neuron_count_per_layer;

    for(int i = 0;i<number_of_layers;i++){
        int neuron_count = neuron_count_per_layer[i];
        for(int j = 0;j<neuron_count;j++){
            Neuron n = {i, j, std::vector<int>()};
            int connections_count = (i+1) == number_of_layers ? -1:neuron_count_per_layer[i+1];
            //no safeguard needed b/c if connections == -1, then for loop doesnt run
            for(int k = 0;k<connections_count;k++){
                n.connections.push_back(k);
            }
            //this->model[i].push_back(n); -> this causes segmentation fault 
        }
    }
};

std::vector<int> NeuralNetwork::feedForward(int n, std::vector<uint8_t> image, int label){ //returns cost func
    int layer = 0;
    for(int i = 0;i<n;i++)
    {

    }
    return std::vector<int>();
}