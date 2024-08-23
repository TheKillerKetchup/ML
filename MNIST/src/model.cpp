#include <iostream>
#include <string>
#include <vector>
#include <fstream>


void read_MNIST_Image_File(std::string path){
    std::ifstream file(path);
    
    int32_t MAGIC_NUMBER = 0;
    file >> MAGIC_NUMBER;
    std::cout << MAGIC_NUMBER;

    file.close();
}

int main(){
    std::string image_Path = "../t10k-images-idx3-ubyte";
    std::cout << 1;
    return 0;
}