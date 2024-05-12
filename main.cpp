#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

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
        for (int j = x; j < x + rectHeight; j++)
        {
            buffer[i * width + j] = color;
        }
    }
}

int main()
{
    const int width = 1024;
    const int height = 1024;
    std::vector<unsigned int> framebuffer(width * height, 0);

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

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned int r = 255.0 * i / height;
            unsigned int g = 255.0 * j / width;
            unsigned int b = 0;
            framebuffer[i * width + j] = packColor(r, g, b);
        }
    }

    const int rectWidth = width / mapWidth;
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
            drawRectangle(framebuffer, width, height, rectX, rectY, rectWidth, rectHeight, packColor(0, 255, 255));
        }
    }

    const float playerX = 7.5;
    const float playerY = 4.5;
    const float playerDir = acos(-1) / 6.0f;

    for (float t = 0.0f; t < 20.0f; t += 0.05f)
    {
        float dirX = playerX + t * cos(playerDir);
        float dirY = playerY + t * sin(playerDir);
        if (map[static_cast<int>(dirY) * mapWidth + static_cast<int>(dirX)] != ' ')
        {
            break;
        }

        framebuffer[(static_cast<int>(dirY * rectHeight) * width) + static_cast<int>(dirX * rectWidth)] = packColor(255, 255, 255);
    }

    drawRectangle(framebuffer, width, height, playerX * rectWidth, playerY * rectHeight, 10, 10, packColor(255, 255, 255));

    dropPpmImage("./out.ppm", framebuffer, width, height);
}