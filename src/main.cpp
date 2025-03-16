#include<SDL2/SDL.h>
#include<bits/stdc++.h>
#include "Game.h"
using namespace std;

int main(int argc, char* argv[]) {
    Game game;

    if (!game.init("LoL Dodge Game", 1600, 900, false)) {
        SDL_Log("Failed to initialize SDL");
        return 0;
    }

    game.run();

    return 0;
}