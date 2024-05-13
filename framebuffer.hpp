#pragma once

#include <vector>

#include "utils.hpp"

class FrameBuffer{
public:
    FrameBuffer(const int w, const int h, const unsigned int color = packColor(255, 255, 255));
    void clear(const unsigned int color);
    void setPixel(const int x, const int y, const unsigned int color);
    void drawRectangle(const int x, const int y, const int w, const int h, const unsigned int color);
    int getWidth();
    int getHeight();
    std::vector<unsigned int> getImage();
private:
    int width;
    int height;
    std::vector<unsigned int> img;
};