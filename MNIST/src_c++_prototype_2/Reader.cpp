#include "Reader.hpp"

Reader::Reader(){
    //theres literally nothing to construct
}
int Reader::byteswap(int num)
    {
        int n = sizeof(num);
        union {
            int original;
            std::uint8_t bytes[sizeof(int)];
        } data;
        data.original = num;
        for(int i = 0;i < n/2; i++){
            std::swap(data.bytes[i], data.bytes[n - 1 - i]);
        }

        return data.original;
    }  

std::vector<std::vector<uint8_t> > Reader::readImageFile(std::string filePath){
    std::ifstream file(filePath);
    if(!file){
        std::cerr << "Cannot open file!" << std::endl;
    }
    int32_t MAGIC_NUMBER = 0; //this is not used rn 
    int32_t num_images = 0;
    int32_t num_rows = 0;
    int32_t num_cols = 0;
    file.read((char*)&MAGIC_NUMBER, sizeof(MAGIC_NUMBER));
    file.read((char*)&num_images, sizeof(num_images));
    file.read((char*)&num_rows, sizeof(num_rows));
    file.read((char*)&num_cols, sizeof(num_cols));
    MAGIC_NUMBER = byteswap(MAGIC_NUMBER);
    num_images = byteswap(num_images);
    num_rows = byteswap(num_rows);
    num_cols = byteswap(num_cols);
    
    int length = num_rows * num_cols; 
    std::vector<std::vector<uint8_t> > OUT_images; //this is what we return!
    for(int i = 0;i<num_images;i++){
        std::vector<uint8_t> blank_image;
        for(int j = 0;j<length;j++){
            uint8_t pixel;
            file.read((char*)&pixel, sizeof(pixel));
            //cout << pixel << " ";
            blank_image.push_back(pixel);
        }
        OUT_images.push_back(blank_image);
    }

    return OUT_images;

}
std::vector<uint8_t> Reader::readLabelFile(std::string filePath){
    std::ifstream file(filePath);
    
    if(!file){
        std::cerr << "Cannot open file!" << std::endl;
    }
    int32_t MAGIC_NUMBER = 0;
    int32_t num_labels = 0;
    file.read((char*)&MAGIC_NUMBER, sizeof(MAGIC_NUMBER));
    file.read((char*)&num_labels, sizeof(num_labels));
    MAGIC_NUMBER = byteswap(MAGIC_NUMBER);
    num_labels = byteswap(num_labels);
    
    std::vector<uint8_t> OUT_labels;
    
    for(int i = 0;i<num_labels;i++){
        uint8_t label;
        file.read((char*)&label, sizeof(label));
        //cout << pixel << " ";
    }

    return OUT_labels;
} 