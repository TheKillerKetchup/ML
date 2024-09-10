#ifndef READER_HPP
#define READER_HPP

#include <iostream>
#include <fstream>
#include <string>
template <typename T>

class Reader{
    public:
        Reader();
        std::vector<std::vector<uint8_t> > readImageFile(std::string filePath);
        std::vector<uint8_t> readLabelFile(std::string filePath);
    private:
        T byteswap(T num);
};

#endif