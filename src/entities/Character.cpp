//
// Created by Admin on 3/16/2025.
//

#include "Character.h"
#include <bits/stdc++.h>
#include "../graphics/TextureManager.h"
using namespace std;

Character::Character(float x, float y, int width, int height)
    : Entity(x, y, width, height),
    target(x, y),
    moving(false),
    speed(200.0f),
    textureID(""),
    frameWidth(width),
    frameHeight(height),
    currentFrame(0),
    currentRow(0),
    flip(SDL_FLIP_NONE),
    lastDirection(0, 1) // mac dinh la huong xuong
{}

void Character::update(float deltaTime) {
    if (!moving) return;

    Vector2D direction = target - position;
    float distance = direction.length();

    // dung lai khi den vi tri can den
    if (distance < 2.0f) {
        position = target;
        moving = false;
        updateRect();
        return;
    }

    // di chuyen den vi tri can den theo huong la vector direction
    Vector2D directionNormalized = direction.normalize();
    position += directionNormalized * speed * deltaTime;

    currentFrame = int(SDL_GetTicks() / 100) % 4;

    if (distance > 0) {
        lastDirection = directionNormalized;
    }

    // Xác định hướng nhìn dựa vào hướng di chuyển
    if (abs(directionNormalized.x) > abs(directionNormalized.y)) {
        // Di chuyen sang ngang
        currentRow = 0;
        if (directionNormalized.x > 0) {
            flip = SDL_FLIP_NONE; // khong flip khi di sang phai
        } else {
            flip = SDL_FLIP_HORIZONTAL; // flip ngang khi di sang trai
        }
    } else {
        // Di chuyen doc len xuong
        currentRow = 0;
        // Không flip khi di chuyển lên/xuống
        flip = (lastDirection.x > 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    }
    updateRect();
}

void Character::render(SDL_Renderer* renderer) {
    // Ve texture voi fram hien tai va flip thich hop
    TextureManager::Instance()->drawFrame(
        textureID,
        rect.x, rect.y,
        frameWidth, frameHeight,
        currentRow, currentFrame,
        renderer,
        flip
    );

}

void Character::setTarget(float x, float y) {
    target.x = x;
    target.y = y;
    moving = true;
}

bool Character::isMoving() const {
    return moving;
}

bool Character::loadTexture(const std::string& filePath, const std::string& id, SDL_Renderer* renderer) {
    if (TextureManager::Instance()->load(filePath, id, renderer)) {
        textureID = id;
        return true;
    }
    return false;
}