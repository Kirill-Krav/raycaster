#pragma once

#include <cmath>

class Player
{
public:
    Player(const float x = 0, const float y = 0, const float d = 0, const float f = acos(-1) / 3.0f) : xCoord(x), yCoord(y), direction(d), fov(f){};
    float getXCoord() { return xCoord; }
    float getYCoord() { return yCoord; }
    void incXCoord(float x) { xCoord += x; }
    void incYCoord(float y) { yCoord += y; }
    float getDirection() { return direction; }
    void turn(float t) { direction += t; }
    float getFov() { return fov; }

private:
    float xCoord, yCoord;
    float direction;
    float fov;
};
