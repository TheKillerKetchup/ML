#include <iostream>
#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(int number_of_layers, std::vector<int> neuron_count_per_layer){
    this->number_of_layers = number_of_layers;
    this->neuron_count_per_layer = neuron_count_per_layer;
    this->sigmoid_power = 2;

    for(int i = 0;i<number_of_layers;i++){
        std::vector<Neuron> layer;
        std::vector<int> cost_layer;
        std::vector<float> gradient_layer_a;
        this->model.push_back(layer);
        this->cost_array.push_back(cost_layer);
        this->Gradient.push_back(gradient_layer_a);
        int neuron_count = neuron_count_per_layer[i];
        for(int j = 0;j<neuron_count;j++){
            Neuron n = {std::vector<int>(), 0, 0, 0};
            int connections_count = (i == number_of_layers-1) ? -1:neuron_count_per_layer[i+1];
            std::vector<float> gradient_layer_b;
            //no safeguard needed b/c if connections == -1, then for loop doesnt run
            for(int k = 0;k<connections_count;k++){
                n.connections.push_back(k);
                gradient_layer_b.push_back(0);
            }
            gradient_layer_b.push_back(0);
            this->model[i].push_back(n); 
            cost_layer[i].push_back(0);
            gradient_layer_a.push_back(gradient_layer_b);
        }
        this->Gradient.push_back(gradient_layer_a);
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
        cost_array[this->number_of_layers-1][i] = (expected - this->model[n-1][i].activation) ** 2;
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
            float dC_dAjL = (i == output_layer_index) ? 2(neuron_j.activation - (expected == j ? 1:0)):0;//ajL-yj, y = expected):0;
            int n = (i == output_layer_index) ? -1:neuron_count_per_layer[i+1]-1;
            for(int k = 0;k<n;k++){
                Neuron neuron_k = this->model[i+1][k];
                dC_dAjL += this->cost_array[i+1][k] * neuron_j.connections[k] * derivative_sigmoid(neuron_k.non_linear_activation);
                float dC_dWjkL = neuron_j.activation * this->cost_array[i+1][k] * derivative_sigmoid(neuron_k.non_linear_activation);
                this->Gradient[i][j][k] = dC_dWjkL;
            }
            float dC_dBjL = derivative_sigmoid(neuron_j.non_linear_activation) * this->cost_array[i+1][j];
            this->Gradient[i][j].push_back(dC_dBjL);
            this->cost_array[i][j] = dC_dAjL;
        }
    }
    //reset all activations to zero 
    for(int i = 0;i<this->number_of_layers;i++){
        for(int j = 0;j<this->neuron_count_per_layer[i];i++){
            this->model[i][j].activation = 0;
        }
    }
}
void NeuralNetwork::train(std::vector<std::vector<uint8_t> > images, std::vector<uint8_t> labels, int stepSize){
    int n = images.size();
    for(int i = 0;i<n;i++){
        if(i % stepSize == 0)
        {
            applyGradient(stepSize); //divisor 
        }
        feedForward(images[i], labels[i]);
        backpropogate(labels[i]);
    }
}
void NeuralNetwork::applyGradient(int divisor){
    //gradient has lengths of dimensions -> number_of_layers, neuron_count_per_layer[d1], neuron_count_per_layer[d1+1]
    int n = this->number_of_layers;
    for(int i = 0;i<n-1;i++){ //this
        for(int j = 0;j<this->neuron_count_per_layer[i];j++){
            Neuron victim = this->model[i][j];
            for(int k = 0;k<this->neuron_count_per_layer[i+1];k++){
                //DEFINITION:Gradient[a][b][c] = gradient of connection at layer a, between neuron b(on layer l), and neuron c(on layer l+1)
        //if c is (b's # of weights + 1), it is a bias not a weight, and does not connect
                float weightChange = this->Gradient[i][j][k]/divisor;
                victim.connections[k] += weightChange;
            }
            float biasChange = this->Gradient[i][j][this->neuron_count_per_layer[i+1]]/divisor;
            victim.bias += biasChange;
        }
    }
}
float NeuralNetwork::sigmoid(float input){
    return (float)(input / (1+std::abs(input) ** this->sigmoid_power) ** (1/this->sigmoid_power));
}
float NeuralNetwork::derivative_sigmoid(float input){
    return (float)(abs(float) ** this->sigmoid_power + 1) ** (-1/this->sigmoid_power - 1);
}