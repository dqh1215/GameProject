//
// Created by Admin on 3/16/2025.
//

#include "Character.h"
#include <bits/stdc++.h>
using namespace std;

Character::Character(float x, float y, int size)
    : Entity(x, y, size, size), target(x, y), moving(false), speed(200.0f) {}

void Character::update(float deltaTime) {
    if (!moving) return;

    Vector2D direction = target - position;
    float distance = direction.length();

    // dung lai khi den vi tri can den
    if (distance == 0.0f) {
        position = target;
        moving = false;
        updateRect();
        return;
    }

    // di chuyen den vi tri can den theo huong la vector direction
    Vector2D directionNormalized = direction.normalize();
    position += directionNormalized * speed * deltaTime;
    updateRect();
}

void Character::render(SDL_Renderer* renderer) {
    // Vẽ một hình vuông màu đỏ làm nhân vật
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Character::setTarget(float x, float y) {
    target.x = x;
    target.y = y;
    moving = true;
}

bool Character::isMoving() const {
    return moving;
}
