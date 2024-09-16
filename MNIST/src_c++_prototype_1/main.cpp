#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <bit>
#include <cstdint>

int main(){
	std::string image_Path = "/Users/kingisaac/Documents/Github/ML/MNIST/t10k-images-idx3-ubyte";
	NeuralNet model;
	Reader reader;
	reader.read_MNIST_Image_File(image_Path, model);
	
	return 0;
}

struct Neuron{
	int layer;
	int connections;
	uint8_t activation;
	//connections with previous layer
	//maybe there should be one for prev, one for next?
	std::vector<float> weights;
	float bias;
};

class NeuralNet{
	public:
		NeuralNet()
		{
			//connections from layer1 to input
			for(int j = 0;j<d2;j++)
			{
				Neuron neuron = Middle_Layers[1][j];
				neuron.connections = 768;
				int n = 768;
				for(int k = 0;k<n;k++)
				{
					neuron.weights.push_back(rand() % 1);
				}
				neuron.bias = (rand() % 32) - 16;
			}
			//connections from layer2 to layer1
			for(int j = 0;j<d2;j++)
			{
				Neuron neuron = Middle_Layers[1][j];
				neuron.connections = 16;
				int n = 16;
				for(int k = 0;k<n;k++)
				{
					neuron.weights.push_back(rand() % 1);
				}
				neuron.bias = (rand() % 32) - 16;
			}
			//connections from output to layer2 
			for(int i = 0;i<10;i++)
			{
				Neuron neuron = Output_Layer[i];
				int n = 10;
				neuron.connections = n;
				for(int k = 0;k<n;k++)
				{
					neuron.weights.push_back(rand() % 1);
				}
				neuron.bias = (rand() % 32) - 16;
			}
		};
		int d1 = 2;
		int d2 = 16;
		Neuron Input_Layer[768];
		Neuron Middle_Layers[2][16];
		Neuron Output_Layer[10]; //0-9 
	private:
	/*
		int d1 = 2;
		int d2 = 16;
		Neuron Input_Layer[768];
		Neuron Middle_Layers[2][16];
		Neuron Output_Layer[10]; //0-9 
		does any of this have to be private? 
	*/
};

class Reader{
	public:
		struct {
			int MAGIC_NUMBER;
			int number_of_images;
			int number_of_rows;
			int number_of_cols;
			int offset;
		} image_File_MetaData;

		void read_MNIST_Image_File(std::string path, NeuralNet model){
			int offset = 0;
			std::ifstream file(path);
			if(!file){
				std::cerr << "Cannot open file!" << std::endl;
				return;
			}
			int32_t MAGIC_NUMBER = 0;
			int32_t num_images = 0;
			int32_t num_rows = 0;
			int32_t num_cols = 0;
			file.read((char*)&MAGIC_NUMBER, sizeof(MAGIC_NUMBER));
			file.read((char*)&num_images, sizeof(num_images));
			file.read((char*)&num_rows, sizeof(num_rows));
			file.read((char*)&num_cols, sizeof(num_cols));
			offset += sizeof(MAGIC_NUMBER);
			offset += sizeof(num_images);
			offset += sizeof(num_rows);	
			offset += sizeof(num_cols);
			MAGIC_NUMBER = byteswap(MAGIC_NUMBER);
			num_images = byteswap(num_images);
			num_rows = byteswap(num_rows);
			num_cols = byteswap(num_cols);
			image_File_MetaData.MAGIC_NUMBER = MAGIC_NUMBER;
			image_File_MetaData.number_of_images = num_images;
			image_File_MetaData.number_of_rows = num_rows;
			image_File_MetaData.number_of_cols = num_cols;
			image_File_MetaData.offset = offset;

			for(int i1 = 0;i1<image_File_MetaData.number_of_images;i1++)
			{
				int imageSize = image_File_MetaData.number_of_rows * image_File_MetaData.number_of_cols;
				Neuron inputLayer[imageSize] = model.Input_Layer;
				for(int i2 = 0;i2<imageSize;i2++){
					input
				}
			}
			
		}
	private:		
		int byteswap(int num)
		{
			int n = sizeof(num);
			union {
				int original;
				std::uint8_t bytes[sizeof(T)];
			} data;
			data.original = num;
			for(int i = 0;i < n/2; i++){
				std::swap(data.bytes[i], data.bytes[n - 1 - i]);
			}

			return data.original;
		}
}