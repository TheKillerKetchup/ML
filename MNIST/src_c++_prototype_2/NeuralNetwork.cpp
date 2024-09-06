#include <iostream>
#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(int number_of_layers, std::vector<int> neuron_count_per_layer){
    this->number_of_layers = number_of_layers;
    this->neuron_count_per_layer = neuron_count_per_layer;

    for(int i = 0;i<number_of_layers;i++){
        std::vector<Neuron> layer;
        std::vector<int> cost_layer;
        std::vector<int> gradient_layer_a;
        this->model.push_back(layer);
        this->cost_array.push_back(cost_layer);
        this->Gradient.push_back(gradient_layer);
        int neuron_count = neuron_count_per_layer[i];
        for(int j = 0;j<neuron_count;j++){
            Neuron n = {std::vector<int>(), 0, 0, 0};
            int connections_count = (i == number_of_layers-1) ? -1:neuron_count_per_layer[i+1];
            std::vector<int> gradient_layer_b;
            //no safeguard needed b/c if connections == -1, then for loop doesnt run
            for(int k = 0;k<connections_count;k++){
                n.connections.push_back(k);
                gradient_layer_b.push_back(0);
            }
            gradient_layer_b.push_back(0);
            this->model[i].push_back(n); 
            this->cost_layer[i].push_back(0);
            gradient_layer_a.push_back(gradient_layer_b);
        }
        this->gradient.push_back(gradient_layer_a);
    }
    //init the cost array

};

void NeuralNetwork::feedForward(std::vector<uint8_t> image, int label){ //returns cost func
    int d1 = sizeof(image);
    for(int i = 0;i<d1;i++){ //init pixels into image 
        Neuron input_neuron = this->model[0][i];
        input_neuron.activation = image[i];
        //std::cout << input_neuron.activation + " ";
    }
    int n = this->number_of_layers;

    for(int i = 1;i<(n-1);i++) //last layer doesnt have connections
    {
        for(int j = 0;j<(this->neuron_count_per_layer[i+1]);j++){
            Neuron neuron_j = this->model[i+1][j];
            for(int k = 0;k<(this->neuron_count_per_layer[i]);k++){
                Neuron neuron_k = this->model[i][k];
                neuron_j.non_linear_activation += (neuron_k.non_linear_activation*neuron_k.connections[j]);
            }
            neuron_j.non_linear_activation += neuron_j.bias;
            //we need to implement the sigmoid squishification function >_< 
            neuron_j.activation = sigmoid(neuron_j.non_linear_activation);
        }
    }
    for(int i = 0;i<(this->neuron_count_per_layer[n-1]);i++){
        int expected = label == i ? 1:0;
        cost_array[this->number_of_layers-1][i] = ((expected - this->model[n-1][i]) ** 2);
    }
    return;
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
void NeuralNetwork::backpropogate(int expected){
    int output_layer_index = this->number_of_layers-1;
    for(int i = output_layer_index;i>=0;i--)
    {
        /*
        if output_layer is current, then we don't go back to check how our activation changes the cost, its linearly computable!
        if it is output_layer, for loop is skipped, and 
        */
        int neurons = this->neuron_count_per_layer[i];
        for(int j = 0;j<neurons;j++){
            Neuron neuron_j = this->model[i][j];
            int dC_dAjL = (i == output_layer_index) ? 2(neuron_j.activation - (expected == j ? 1:0)):0;//ajL-yj, y = expected):0;
            int n = (i == output_layer_index) ? -1:neuron_count_per_layer[i+1]-1;
            for(int k = 0;k<n;k++){
                Neuron neuron_k = this->model[i+1][k];
                dC_dAjL += this->cost_array[i+1][k] * neuron_j.connections[k] * derivative_sigmoid(neuron_k.non_linear_activation);
                int dC_dWjkL = neuron_j.activation * this->cost_array[i+1][k] * derivative_sigmoid(neuron_k.non_linear_activation);
                this->Gradient[i][j][k] = dC_dWjkL;
            }
            int dC_dBjL = derivative_sigmoid(neuron_k.non_linear_activation) * this->cost_array[i+1][k];
            this->Gradient[i][j].push_back(dC_dBjL);
            this->cost_array[i][j] = dC_dAjL;
        }
    }
    //reset all activations to zero 
    for(int i = 0;i<n;i++){
        for(int j = 0;j<this->neuron_count_per_layer[i];i++){
            this->model[i][j].activation = 0;
        }
    }
}
