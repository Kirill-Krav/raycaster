#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "utils.hpp"

class Texture
{
public:
    Texture(const std::string filename);
    unsigned int& getPixel(const int x, const int y);
    std::vector<unsigned int> getColumn(const int textureX, const int columnHeight);
    int getWidth();
    int getHeight();
private:
    int width;
    int height;
    std::vector<unsigned int> img;
};