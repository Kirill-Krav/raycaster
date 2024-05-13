#include "utils.hpp"

unsigned int packColor(const unsigned int r, const unsigned int g, const unsigned int b, const unsigned int a)
{
    return (a << 24) + (b << 16) + (g << 8) + r;
}

void unpackColor(const unsigned int buffer, unsigned int &r, unsigned int &g, unsigned int &b, unsigned int &a)
{
    r = buffer & 255;
    g = (buffer >> 8) & 255;
    b = (buffer >> 16) & 255;
    a = (buffer >> 24) & 255;
}

void dropPpmImage(const std::string &path, const std::vector<unsigned int> buffer, const int width, const int height)
{
    std::ofstream fout;
    fout.open(path, std::ofstream::out | std::ofstream::binary);
    fout << "P6\n"
         << width << " " << height << "\n255\n";
    for (int i = 0; i < buffer.size(); i++)
    {
        unsigned int r, g, b, a;
        unpackColor(buffer[i], r, g, b, a);
        fout << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
    }
    fout.close();
}