//
// Created by Admin on 3/16/2025.
//

#include "InputHandler.h"

InputHandler::InputHandler() : mouseButtonDown(false) {}

void InputHandler::handleEvents(bool& running, Character& character) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
            break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    character.setTarget(event.button.x, event.button.y);
                    mouseButtonDown = true;
                }
            break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouseButtonDown = false;
                }
            break;
        }
    }
}