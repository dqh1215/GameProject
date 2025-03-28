//
// Created by Admin on 3/16/2025.
//

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H


#include <SDL2/SDL.h>
#include "../entities/Character.h"
enum class GameState;
class Game;
class InputHandler {
public:
    InputHandler();
    ~InputHandler() = default;

    void handleEvents(bool& running, Character& character, Game& game, GameState& currentState);

private:
    bool mouseButtonDown;
    bool QKeyDown;
    bool EKeyDown;
};


#endif //INPUTHANDLER_H
