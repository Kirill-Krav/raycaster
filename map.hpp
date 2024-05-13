#pragma once

#include <string>

class Map
{
public:
    Map(const int w, const int h, const std::string &mp);
    char get(const int x, const int y);
    bool isEmpty(const int x, const int y);
    int getWidth();
    int getHeight();
private:
    int width;
    int height;
    std::string map;
};