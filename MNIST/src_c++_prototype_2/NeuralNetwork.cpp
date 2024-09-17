#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(int number_of_layers, std::vector<int> neuron_count_per_layer){
    this->number_of_layers = number_of_layers;
    this->neuron_count_per_layer = neuron_count_per_layer;
    this->sigmoid_power = 2;
    
    for(int i = 0;i<number_of_layers;i++){
        std::vector<Neuron> layer;
        std::vector<float> cost_layer;
        std::vector<std::vector<float> > gradient_layer_a;
        this->model.push_back(layer);
        int neuron_count = neuron_count_per_layer[i];
        for(int j = 0;j<neuron_count;j++){
            Neuron n = {std::vector<float>(), 0, 0, 0};
            int connections_count = (i == number_of_layers-1) ? -1:neuron_count_per_layer[i+1];
            std::vector<float> gradient_layer_b;
            //no safeguard needed b/c if connections == -1, then for loop doesnt run
            for(int k = 0;k<connections_count;k++){
                n.connections.push_back(rand() % 2); //[-1,1]
                gradient_layer_b.push_back(0);
            }
            gradient_layer_b.push_back(0);
            n.bias = (rand() % 2) - 1; //[-1,1]
            this->model[i].push_back(n); 
            cost_layer.push_back(0);
            gradient_layer_a.push_back(gradient_layer_b);
        }
        this->Gradient.push_back(gradient_layer_a);
        this->cost_array.push_back(cost_layer);
    }
    //init the cost array

};

void NeuralNetwork::feedForward(std::vector<uint8_t> image, int label){ 
    int d1 = image.size();
    for(int i = 0;i<d1;i++){ //init pixels into image 
        this->model[0][i].non_linear_activation = image[i];
        //std::cout << "model(" << 0 << "," << i << "): " << this->model[0][i].activation << " ";
    }
    int n = this->number_of_layers;

    for(int i = 0;i<(n-1);i++) //last layer doesnt have connections
    {
        for(int j = 0;j<(this->neuron_count_per_layer[i+1]);j++){
            Neuron neuron_j = this->model[i+1][j];
            for(int k = 0;k<(this->neuron_count_per_layer[i]);k++){
                Neuron neuron_k = this->model[i][k];
                neuron_j.non_linear_activation += (neuron_k.activation*neuron_k.connections[j]);
                //std::cout << "ACTIVATION: " << neuron_k.non_linear_activation << "  CONNECTION: " << neuron_k.connections[j] << "\n";
            }
            neuron_j.non_linear_activation += neuron_j.bias;
            //std::cout << neuron_j.non_linear_activation << "\n";
            //we need to implement the sigmoid squishification function >_< 
            neuron_j.activation = sigmoid(neuron_j.non_linear_activation);
            //std::cout << i << " " << j << " NON-LIN ACTIVATION: " << neuron_j.non_linear_activation << " REAL ACTIVATION: " << neuron_j.activation << "\n";
            this->model[i+1][j] = neuron_j;
        }
    }
    /*
    for(int i = 0;i<(this->neuron_count_per_layer[n-1]);i++){
        int expected = label == i ? 1:0;
        cost_array[this->number_of_layers-1][i] = pow((expected - this->model[n-1][i].activation),2);
    }
    */
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
            float dC_dAjL = 0;//ajL-yj, y = expected):0;
            if(i == output_layer_index){
                dC_dAjL = (neuron_j.activation);
                dC_dAjL -= expected == j ? 1:0;
                dC_dAjL *= 2;
            }
            //std::cout << i << " " << j << ", activation: " << neuron_j.activation << "\n";
            int n = (i == output_layer_index) ? -1:neuron_count_per_layer[i+1]-1;
            for(int k = 0;k<n;k++){
                Neuron neuron_k = this->model[i+1][k];
                dC_dAjL += this->cost_array[i+1][k] * neuron_j.connections[k] * derivative_sigmoid(neuron_k.non_linear_activation);
                float dC_dWjkL = neuron_j.activation * this->cost_array[i+1][k] * derivative_sigmoid(neuron_k.non_linear_activation);
                //std::cout << neuron_j.activation << " " << this->cost_array[i+1][k] << " " << derivative_sigmoid(neuron_k.non_linear_activation) << "\n";
                this->Gradient[i][j][k] = dC_dWjkL;
            }
            float dC_dBjL = derivative_sigmoid(neuron_j.non_linear_activation) * dC_dAjL;
            this->Gradient[i][j].push_back(dC_dBjL);
            this->cost_array[i][j] = dC_dAjL;
            //std::cout << i << " " << j << " " << dC_dAjL << "\n";
            //std::cout << "\n";
        }
    }
    //reset all activations to zero 
}
void NeuralNetwork::train(std::vector<std::vector<uint8_t> > images, std::vector<uint8_t> labels, int stepSize, std::string model_file){
    int n = images.size();
    for(int i = 0;i<n;i++){
        if(i % stepSize == 0)
        {
            applyGradient(stepSize); //divisor 
        }
        feedForward(images[i], labels[i]);
        backpropogate(labels[i]); //this the problem rn
        resetActivations();
        std::cout << "image " << i << " complete." << "\n";
    }
    //print model to model_file
    std::cout << "printing..." << "\n";
    this->print(model_file);

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
                //std::cout << this->Gradient[i][j][k] << " " << divisor << " ";
            }
            float biasChange = this->Gradient[i][j][this->neuron_count_per_layer[i+1]]/divisor;
            victim.bias += biasChange;
            //std::cout << this->Gradient[i][j][this->neuron_count_per_layer[i+1]] << " " << divisor << " ";
            //std::cout << "\n";
        }
    }
}
float NeuralNetwork::sigmoid(float x){
    return (float)(x / std::pow((1.0f+std::pow(std::abs(x), this->sigmoid_power)), (1.0f/this->sigmoid_power)));
    //return 1.0f / (1.0f + std::exp(-x));
}
float NeuralNetwork::derivative_sigmoid(float x){
    //float sig = sigmoid(x);
    //return sig * (1-sig);
    return (float)(std::pow(pow(std::abs(x), this->sigmoid_power) + 1.0f, -1.0f/this->sigmoid_power - 1.0f));
}
/*
float NeuralNetwork::test(std::vector<std::vector<uint8_t> > images, std::vector<uint8_t> labels, std::string model_file, std::string test_results_file){
    std::ifstream input_file;
    input_file.open(model_file);
    std::ofstream output_file;
    output_file.open(test_results_file);
    float n = images.size();
    float correct = 0;
    for(int i = 0;i<n;i++){
        feedForward(images[i], labels[i]);
        file << labels[i];
        int n = this->neuron_count_per_layer[this->number_of_layers-1];
        int greatest_activation = 0;
        int answer = 0;
        for(int j = 0;j<n;j++){
            int act = this->model[this->number_of_layers-1][j].activation;
            file << act;
            if(act > greatest_activation){
                greatest_activation = act;
                answer = j;
            }
        }
        if(answer == labels[i]){ 
            correct++;
            file << "CORRECT!";
        }
        file << "\n";
        resetActivations();
    }
    file.close();
    return correct / n;
}
*/
void NeuralNetwork::resetActivations(){
    for(int i = 0;i<this->number_of_layers;i++){
        for(int j = 0;j<this->neuron_count_per_layer[i];j++){
            Neuron n = this->model[i][j];
            n.activation = 0;
            n.non_linear_activation = 0;
        }
    }
}
//this is supposed to print the neural network into a txt file, perhaps to be pipelined into python for easy excel visualization
void NeuralNetwork::print(std::string output_file){
    std::ofstream file;
    file.open(output_file);
    int d1 = this->number_of_layers;
    file << d1 << " ";
    for(int i = 0;i<d1;i++) file << this->neuron_count_per_layer[i] << " ";
    file << "\n";
    for(int i = 0;i<d1;i++){
        int d2 = this->neuron_count_per_layer[i];
        for(int j = 0;j<d2;j++){
            Neuron neuron = this->model[i][j];
            //we need to print weights and biases to next layer
            int d3 = neuron.connections.size();
            for(int k = 0;k<d3;k++){
                file << neuron.connections[k] << " ";
            }
            file << neuron.bias << " ";
        }
        file << "\n";
    }
    file.close();
}