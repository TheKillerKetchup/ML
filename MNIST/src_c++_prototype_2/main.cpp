#include "NeuralNetwork.cpp"
#include "Reader.cpp"

int main(){
    int layers;
    std::string network_config_path = "network_configs.txt";
    std::ifstream file(network_config_path);
    
    if(!file){
        std::cerr << "Cannot open file!" << std::endl;
    }
    file >> layers;
    std::vector<int> neuron_count_per_layer;
    for(int i = 0;i<layers;i++){
        int neuron_count;
        file >> neuron_count;
        neuron_count_per_layer.push_back(neuron_count);     
    }
    int n = neuron_count_per_layer.size();
    NeuralNetwork skynet(layers, neuron_count_per_layer); 
    Reader reader;
    std::string training_images_path = "/Users/kingisaac/Documents/Github/ML/MNIST/train-images-idx3-ubyte";
    std::string training_labels_path = "/Users/kingisaac/Documents/Github/ML/MNIST/train-labels-idx1-ubyte";
    std::string testing_images_path = "/Users/kingisaac/Documents/Github/ML/MNIST/t10k-images-idx3-ubyte";
    std::string testing_labels_path = "/Users/kingisaac/Documents/Github/ML/MNIST/t10k-labels-idx1-ubyte";
    std::string print_file = "neural_network.txt";
    std::string test_results_file = "test_results.txt";
    std::string model_file = "model.txt";
    //skynet.print(print_file);
    skynet.train(reader.readImageFile(training_images_path), reader.readLabelFile(training_labels_path), 20, model_file);
    skynet.test(reader.readImageFile(testing_images_path), reader.readLabelFile(testing_labels_path), test_results_file);
    return 0;
}