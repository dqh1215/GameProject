//
// Created by Admin on 3/16/2025.
//

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <bits/stdc++.h>
#include "core/Timer.h"
#include "entities/Character.h"
#include "input/InputHandler.h"
#include "graphics/TextureManager.h"
using namespace std;


class Game {
public:
    Game();
    ~Game();

    bool init(const string& title, int width = 0, int height = 0, bool fullscreen = true);
    void run();
    void clean();
    bool loadTextureBackground(const string& filePath, const string& id, SDL_Renderer* renderer);

private:
    void update();
    void render();
    void handleEvents();
    bool loadAssets();

    SDL_Window* window;
    SDL_Renderer* renderer;

    Timer timer;
    Character* player;
    InputHandler inputHandler;

    bool running;
    int screenWidth;
    int screenHeight;
};

#endif //GAME_H
