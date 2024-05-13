#include "framebuffer.hpp"

#include <iostream>

FrameBuffer::FrameBuffer(const int w, const int h, const unsigned int color) : width(w), height(h)
{
    clear(color);
}

void FrameBuffer::clear(const unsigned int color)
{
    img = std::vector<unsigned int>(width * height, color);
}

void FrameBuffer::setPixel(const int x, const int y, const unsigned int color)
{
    if(x >= width || y >= height || y < 0 || x < 0){
        return;
    }
    img[y * width + x] = color;
}

void FrameBuffer::drawRectangle(const int x, const int y, const int w, const int h, const unsigned int color)
{
    for (int i = y; i < y + h; i++)
    {
        for (int j = x; j < x + w; j++)
        {
            img[i * width + j] = color;
        }
    }
}

int FrameBuffer::getWidth()
{
    return width;
}

int FrameBuffer::getHeight()
{
    return height;
}

std::vector<unsigned int> FrameBuffer::getImage()
{
    return img;
}