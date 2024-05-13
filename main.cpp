#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int packColor(const unsigned int r, const unsigned int g, const unsigned int b, const unsigned int a = 255)
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

void drawRectangle(std::vector<unsigned int> &buffer, const int width, const int height, const int x, const int y, const int rectWidth, const int rectHeight, const unsigned int color)
{
    for (int i = y; i < y + rectHeight; i++)
    {
        for (int j = x; j < x + rectWidth; j++)
        {
            buffer[i * width + j] = color;
        }
    }
}

bool loadTexture(const std::string filename, std::vector<unsigned int> &texture, int &sz)
{
    int nchannels = -1, w, h;
    unsigned char *pixmap = stbi_load(filename.c_str(), &w, &h, &nchannels, 0);
    if (!pixmap)
    {
        std::cerr << "Error: can not load the textures" << std::endl;
        return false;
    }
    if (4 != nchannels)
    {
        std::cerr << "Error: the texture must be a 32 bit image" << std::endl;
        stbi_image_free(pixmap);
        return false;
    }
    sz = w;
    texture = std::vector<unsigned int>(w * h);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            unsigned int r = pixmap[(i * w + j) * 4 + 0];
            unsigned int g = pixmap[(i * w + j) * 4 + 1];
            unsigned int b = pixmap[(i * w + j) * 4 + 2];
            unsigned int a = pixmap[(i * w + j) * 4 + 3];
            texture[i * w + j] = packColor(r, g, b, a);
        }
    }
    stbi_image_free(pixmap);
    return true;
}

int main()
{
    const int width = 2048;
    const int height = 1024;
    std::vector<unsigned int> framebuffer(width * height, packColor(255, 255, 255));

    const int mapWidth = 16;
    const int mapHeight = 16;
    const char map[] = "0000000000000000"
                       "0   0          0"
                       "0   0          0"
                       "0         000000"
                       "0   0     0    0"
                       "0   0          0"
                       "0   0          0"
                       "0   0000000    0"
                       "0         0    0"
                       "0         0    0"
                       "0   0000000    0"
                       "0    0    0    0"
                       "0    0    0    0"
                       "0    0    0    0"
                       "0              0"
                       "0000000000000000";

    std::vector<unsigned int> texture;
    int textureSize;
    if (!loadTexture("textures/wall.png", texture, textureSize)) {
        std::cerr << "Failed to load wall textures" << std::endl;
        return -1;
    }


    const int rectWidth = width / (2 * mapWidth);
    const int rectHeight = height / mapHeight;
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            if (map[i * mapWidth + j] == ' ')
            {
                continue;
            }
            int rectX = j * rectWidth;
            int rectY = i * rectHeight;
            drawRectangle(framebuffer, width, height, rectX, rectY, rectWidth, rectHeight, texture[0]);
        }
    }

    const float playerX = 7.5;
    const float playerY = 2.5;
    const float playerDir = acos(-1) / 6;
    const float fov = acos(-1) / 3.0f;

    for (int i = 0; i < width / 2; i++)
    {
        float angle = playerDir - fov / 2 + static_cast<float>(i) / (width / 2);
        for (float t = 0.0f; t < 20.0f; t += 0.01f)
        {
            float dirX = playerX + t * cos(angle);
            float dirY = playerY + t * sin(angle);
            framebuffer[(static_cast<int>(dirY * rectHeight) * width) + static_cast<int>(dirX * rectWidth)] = packColor(0, 0, 0);

            if (map[static_cast<int>(dirY) * mapWidth + static_cast<int>(dirX)] != ' ')
            {
                int colomnHeight = height / (t * cos(angle - playerDir));
                float hitX = dirX - floor(dirX);
                float hitY = dirY - floor(dirY);
                int textureX = hitX * textureSize;
                if(abs(dirX - floor(dirX + 0.5)) > abs(dirY - floor(dirY + 0.5))){
                    textureX = hitX * textureSize;
                }
                else{
                    textureX = hitY * textureSize;
                }
                int st = height / 2 - colomnHeight / 2;
                for(int j = st; j < height / 2 + colomnHeight / 2; j++){
                    float tt = static_cast<float>(j - st) / colomnHeight;
                    framebuffer[j * width + width / 2 + i] = texture[static_cast<int>(tt * textureSize) * textureSize + textureX];
                }
                break;
            }
        }
    }

    drawRectangle(framebuffer, width, height, playerX * rectWidth, playerY * rectHeight, 10, 10, packColor(0, 0, 0));

    dropPpmImage("./out.ppm", framebuffer, width, height);
}