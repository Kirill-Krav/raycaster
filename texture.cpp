#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string filename)
{
    int nchannels = -1;
    unsigned char *pixmap = stbi_load(filename.c_str(), &width, &height, &nchannels, 0);
    if (!pixmap)
    {
        std::cerr << "Error: can not load the textures" << std::endl;
        return;
    }
    if (4 != nchannels)
    {
        std::cerr << "Error: the texture must be a 32 bit image" << std::endl;
        stbi_image_free(pixmap);
        return;
    }
    img = std::vector<unsigned int>(width * height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned int r = pixmap[(i * width + j) * 4 + 0];
            unsigned int g = pixmap[(i * width + j) * 4 + 1];
            unsigned int b = pixmap[(i * width + j) * 4 + 2];
            unsigned int a = pixmap[(i * width + j) * 4 + 3];
            img[i * width + j] = packColor(r, g, b, a);
        }
    }
    stbi_image_free(pixmap);
}

unsigned int &Texture::getPixel(const int x, const int y)
{
        return img[y * width + x];
}

std::vector<unsigned int> Texture::getColumn(const int textureX, const int columnHeight)
{
    std::vector<unsigned int> column(columnHeight);
    for(int i = 0; i < columnHeight; i++){
        column[i] = getPixel(textureX, i * height / columnHeight);
    }
    return column;
}

int Texture::getWidth(){
    return width;
}

int Texture::getHeight(){
    return height;
}