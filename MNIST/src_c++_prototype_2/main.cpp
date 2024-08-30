#include "NeuralNetwork.cpp"

int main(){
    int layers = 4;
    static const int arr[] = {768, 16, 16, 10};
    std::vector<int> neuron_count_per_layer(arr, arr+sizeof(arr) / sizeof(arr[0]));
    NeuralNetwork skynet(layers, neuron_count_per_layer); 

}