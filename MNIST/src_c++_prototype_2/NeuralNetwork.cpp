#include <iostream>
#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(int number_of_layers, std::vector<int> neuron_count_per_layer){
    this->number_of_layers = number_of_layers;
    this->neuron_count_per_layer = neuron_count_per_layer;

    for(int i = 0;i<number_of_layers;i++){
        std::vector<Neuron> layer;
        this->model.push_back(layer);
        int neuron_count = neuron_count_per_layer[i];
        for(int j = 0;j<neuron_count;j++){
            Neuron n = {i, j, std::vector<int>()};
            int connections_count = (i == 0) ? -1:neuron_count_per_layer[i-1];
            //no safeguard needed b/c if connections == -1, then for loop doesnt run
            for(int k = 0;k<connections_count;k++){
                n.connections.push_back(k);
            }
            this->model[i].push_back(n); 
        }
    }
};

std::vector<int> NeuralNetwork::feedForward(std::vector<uint8_t> image, int label){ //returns cost func
    int d1 = sizeof(image);
    for(int i = 0;i<d1;i++){ //init pixels into image 
        Neuron input_neuron = this->model[0][i];
        input_neuron.activation = image[i];
        //std::cout << input_neuron.activation + " ";
    }
    int n = this->number_of_layers;

    for(int i = 1;i<(n);i++) //last layer doesnt have connections
    {
        for(int j = 0;j<(this->neuron_count_per_layer[i+1]);j++){
            Neuron neuron_j = this->model[i][j];
            for(int k = 0;k<(this->neuron_count_per_layer[i-1]);k++){
                Neuron neuron_k = this->model[i-1][k];
                neuron_j.activation += (neuron_j.activation*neuron_j.connections[k]);
            }
            neuron_j.activation += neuron_j.bias;
            //we need to implement the sigmoid squishification function >_< 
        }
    }
    std::vector<uint8_t> cost;
    for(int i = 0;i<(this->neuron_count_per_layer[n-1]);i++){
        int expected = label == i ? 1:0;
        cost.push_back((expected - this->model[n-1][i]) ** 2);
    }

    return cost;
}
/*
C = cost
Z = pre-sum
A = activation
dC / dW(j,k,L) = A(k,L-1) * (derivative of sigmoid at z(j,L)) * (dC/dA(j,L))
dC/dA(j,L) = for(j in range (1:))

available funcs
   int calculateCostWeight(int layer, int neuron_i_index, int neuron_j_index);
   int calculateCostBias(int layer, int neuron_i_index);
   int calculateCostActivation(int layer, int neuron_i_index);
*/
void NeuralNetwork::backPropogate(int expected){
    int output_layer_index = this->number_of_layers-1;
    for(int i = output_layer_index;i>=0;i--)
    {
        /*
        if output_layer is current, then we don't go back to check how our activation changes the cost, its linearly computable!
        if it is output_layer, for loop is skipped, and 
        */
        int neurons = this->neuron_count_per_layer[i];
        for(int j = 0;j<neurons;j++){
            int dC_dAjL = (i == output_layer_index) ? 2(this->model[i][j]- (expected == j ? 1:0));//ajL-yj, y = expected):0;
            int n = (i == output_layer_index) ? -1:neuron_count_per_layer[i+1]-1;

            for(int k = 0;k<n;k++)
                int dC_dWjkL = 0;
        }
    }
    //reset all activations to zero 
    for(int i = 0;i<n;i++){
        for(int j = 0;j<this->neuron_count_per_layer[i];i++){
            this->model[i][j].activation = 0;
        }
    }
}