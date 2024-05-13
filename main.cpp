#include "map.hpp"
#include "player.hpp"
#include "framebuffer.hpp"
#include "texture.hpp"
#include "utils.hpp"

int getXCoordOfTexture(const float x, const float y, const int width)
{
    float hitX = x - floor(x);
    float hitY = y - floor(y);
    if (std::abs(hitX - 0.5) < std::abs(hitY - 0.5))
    {
        return hitX * width;
    }
    else
    {
        return hitY * width;
    }
}

void render(FrameBuffer &fb, Map &map, Player &player, Texture &texture)
{
    const int rectWidth = fb.getWidth() / (2 * map.getWidth());
    const int rectHeight = fb.getHeight() / map.getHeight();
    for (int i = 0; i < map.getHeight(); i++)
    {
        for (int j = 0; j < map.getWidth(); j++)
        {
            if (map.isEmpty(j, i))
            {
                continue;
            }
            int rectX = j * rectWidth;
            int rectY = i * rectHeight;
            fb.drawRectangle(rectX, rectY, rectWidth, rectHeight, texture.getPixel(0, 0));
        }
    }
    for (int i = 0; i < fb.getWidth() / 2; i++)
    {
        float angle = player.getDirection() - player.getFov() / 2 + static_cast<float>(i) / (fb.getWidth() / 2);
        for (float t = 0.0f; t < 20.0f; t += 0.01f)
        {
            float dirX = player.getXCoord() + t * cos(angle);
            float dirY = player.getYCoord() + t * sin(angle);
            fb.setPixel(dirX * rectWidth, dirY * rectHeight, packColor(0, 0, 0));
            if (map.isEmpty(dirX, dirY))
            {
                continue;
            }
            int colomnHeight = fb.getHeight() / (t * cos(angle - player.getDirection()));

            int textureX = getXCoordOfTexture(dirX, dirY, texture.getWidth());

            std::vector<unsigned int> column = texture.getColumn(textureX, colomnHeight);
            int st = fb.getHeight() / 2 - colomnHeight / 2;
            for (int j = st; j < fb.getHeight() / 2 + colomnHeight / 2; j++)
            {
                float tt = static_cast<float>(j - st) / colomnHeight;
                fb.setPixel(i + fb.getWidth() / 2, j, column[j - st]);
            }
            break;
        }
    }

    fb.drawRectangle(player.getXCoord() * rectWidth, player.getYCoord() * rectHeight, 10, 10, packColor(0, 0, 0));
}

int main()
{
    FrameBuffer fb(2048, 1024);
    Player player(7.5, 2.5, acos(-1));
    Map map(16, 16, "0000000000000000"
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
                    "0000000000000000");
    Texture texture("textures/wall.png");
    render(fb, map, player, texture);
    dropPpmImage("./out.ppm", fb.getImage(), fb.getWidth(), fb.getHeight());
}
