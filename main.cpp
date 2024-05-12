#include <iostream>
#include <vector>
#include <fstream>

unsigned int packColor(const unsigned int r, const unsigned int g, const unsigned int b, const unsigned int a = 255){
    return (a << 24) + (b << 16) + (g << 8) + r;    
}

void unpackColor(const unsigned int buffer, unsigned int &r, unsigned int &g, unsigned int &b, unsigned int &a){
    r = buffer & 255;
    g = (buffer >> 8) & 255;
    b = (buffer >> 16) & 255;
    a = (buffer >> 24) & 255;
}

void dropPpmImage(const std::string &path, const std::vector<unsigned int> buffer, const int width, const int height){
    std::ofstream fout;
    fout.open(path, std::ofstream::out | std::ofstream::binary);
    fout << "P6\n" << width << " " << height << "\n255\n";
    for(int i = 0; i < buffer.size(); i++){
        unsigned int r, g, b, a;
        unpackColor(buffer[i], r, g, b, a);
        fout << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b); 
    }
    fout.close();
}

int main(){
    const int width = 1024;
    const int height = 1024;
    std::vector<unsigned int> framebuffer(width * height, 0);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            unsigned int r = 255.0 * i / height;
            unsigned int g = 255.0 * j / width;
            unsigned int b = 0;
            framebuffer[i * width + j] = packColor(r, g, b);
        }
    }

    dropPpmImage("./out.ppm", framebuffer, width, height);
}