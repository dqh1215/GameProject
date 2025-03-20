//
// Created by Admin on 3/16/2025.
//

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H


#include <SDL2/SDL.h>
#include "../entities/Character.h"

class Game;
class InputHandler {
public:
    InputHandler();
    ~InputHandler() = default;

    void handleEvents(bool& quit, Character& character, Game& game);

private:
    bool mouseButtonDown;
    bool QKeyDown;
    bool EKeyDown;
};


#endif //INPUTHANDLER_H
