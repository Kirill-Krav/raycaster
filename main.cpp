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
        for (int j = x; j < x + rectWidth; j++)
        {
            buffer[i * width + j] = color;
        }
    }
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
            drawRectangle(framebuffer, width, height, rectX, rectY, rectWidth, rectHeight, packColor(0, 255, 255));
        }
    }

    const float playerX = 7.5;
    const float playerY = 4.5;
    const float playerDir = acos(-1) / 6.0f;
    const float fov = acos(-1) / 3.0f;

    for (int i = 0; i < width / 2; i++)
    {
        float angle = playerDir - fov / 2 + static_cast<float>(i) / (width / 2);
        for (float t = 0.0f; t < 20.0f; t += 0.05f)
        {
            float dirX = playerX + t * cos(angle);
            float dirY = playerY + t * sin(angle);
            framebuffer[(static_cast<int>(dirY * rectHeight) * width) + static_cast<int>(dirX * rectWidth)] = packColor(0, 0, 0);
            
            if (map[static_cast<int>(dirY) * mapWidth + static_cast<int>(dirX)] != ' ')
            {
                int colomnHeight = height / t;
                drawRectangle(framebuffer, width, height, width / 2 + i, height / 2 - colomnHeight / 2, 1, colomnHeight, packColor(0, 255, 255));
                break;
            }
        }
    }

    drawRectangle(framebuffer, width, height, playerX * rectWidth, playerY * rectHeight, 10, 10, packColor(0, 0, 0));

    dropPpmImage("./out.ppm", framebuffer, width, height);
}