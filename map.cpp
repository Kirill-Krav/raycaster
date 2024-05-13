#include "map.hpp"

Map::Map(const int w, const int h, const std::string &mp): width(w), height(h) {
    map.resize(mp.size());
    for(int i = 0; i < mp.size(); i++){
        map[i] = mp[i];
    }
}

char Map::get(const int x, const int y)
{
    return map[y * width + x];
}

bool Map::isEmpty(const int x, const int y)
{
    return ' ' == get(x, y);
}

int Map::getWidth(){
    return width;
}

int Map::getHeight(){
    return height;
}