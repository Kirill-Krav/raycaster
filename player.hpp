#pragma once

#include <cmath>

class Player
{
public:
    Player(const float x = 0, const float y = 0, const float d = 0, const float f = acos(-1) / 3.0f) : xCoord(x), yCoord(y), direction(d), fov(f){};
    float getXCoord() { return xCoord; }
    float getYCoord() { return yCoord; }
    float getDirection() { return direction; }
    void turn() { direction += statusTrn; }
    void mv()
    {
        xCoord += cos(getDirection()) * statusMv;
        yCoord += sin(getDirection()) * statusMv;
    }
    float getFov() { return fov; }
    void setStatusTrn(float st) { statusTrn = st; }
    void setStatusMv(float st) { statusMv = st; }
    float getStatusMv() { return statusMv; }

private:
    float xCoord, yCoord;
    float direction;
    float fov;
    float statusMv = 0.0;
    float statusTrn = 0.0;
};
