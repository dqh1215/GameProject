//
// Created by Admin on 3/16/2025.
//

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H


#include <SDL2/SDL.h>
#include "../entities/Character.h"

class InputHandler {
public:
    InputHandler();
    ~InputHandler() = default;

    void handleEvents(bool& quit, Character& character);

private:
    bool mouseButtonDown;
};


#endif //INPUTHANDLER_H
