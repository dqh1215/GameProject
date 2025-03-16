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
using namespace std;


class Game {
public:
    Game();
    ~Game();

    bool init(const string& title, int width = 0, int height = 0, bool fullscreen = true);
    void run();
    void clean();

private:
    void update();
    void render();
    void handleEvents();

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
