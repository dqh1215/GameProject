//
// Created by Admin on 3/16/2025.
//

#include "InputHandler.h"
#include "../Game.h"
InputHandler::InputHandler() : mouseButtonDown(false), QKeyDown(false), EKeyDown(false) {}

void InputHandler::handleEvents(bool& running, Character& character, Game& game) {
    SDL_Event event;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                else if (event.key.keysym.sym == SDLK_q && !QKeyDown) {
                    QKeyDown = true;
                    if (character.canShoot()) {
                        character.shoot(mouseX, mouseY);

                        Vector2D playerPos = character.getPosition();
                        Vector2D direction(mouseX - playerPos.x, mouseY - playerPos.y);

                        game.shootProjectile(playerPos, direction);
                    }
                }
                break;

            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_q) {
                    QKeyDown = false;
                }
                else if (event.key.keysym.sym == SDLK_e) {
                    EKeyDown = false;
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