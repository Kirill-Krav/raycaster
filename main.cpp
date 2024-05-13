#include "SDL2/SDL.h"

#undef main

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
    fb.clear(packColor(255, 255, 255));
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
    FrameBuffer fb(1536, 768);
    Player player(7.5, 3.5, acos(-1) / 2);
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

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (SDL_CreateWindowAndRenderer(fb.getWidth(), fb.getHeight(), SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer))
    {
        std::cerr << "Couldn't create window and renderer: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Texture *framebuffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, fb.getWidth(), fb.getHeight());
    SDL_UpdateTexture(framebuffer_texture, NULL, reinterpret_cast<void *>(fb.getImage().data()), fb.getWidth() * 4);

    SDL_Event event;
    while (1)
    {
        //SDL_PollEvent(&event);
        if (SDL_PollEvent(&event))
        {
            if (SDL_KEYDOWN==event.type) {
                if ('a'==event.key.keysym.sym){
                    player.turn(-0.1);
                } 
                if ('d'==event.key.keysym.sym){
                    player.turn(0.1);
                } 
                if ('w'==event.key.keysym.sym){
                    player.incXCoord(cos(player.getDirection()) * 0.1);
                    player.incYCoord(sin(player.getDirection()) * 0.1);
                } 
                if ('s'==event.key.keysym.sym){
                    player.incXCoord(cos(player.getDirection()) * -0.1);
                    player.incYCoord(sin(player.getDirection()) * -0.1);
                } 
            }
        }
        //std::cout << player.getXCoord() << " ";
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, framebuffer_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_UpdateTexture(framebuffer_texture, NULL, reinterpret_cast<void *>(fb.getImage().data()), fb.getWidth()*4);
        render(fb, map, player, texture);
    }

    SDL_DestroyTexture(framebuffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    // dropPpmImage("./out.ppm", fb.getImage(), fb.getWidth(), fb.getHeight());
}
