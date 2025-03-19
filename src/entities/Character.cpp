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
    speed(250.0f),
    textureID(""),
    frameWidth(width),
    frameHeight(height),
    currentFrame(0),
    currentRow(0),
    flip(SDL_FLIP_NONE){}

void Character::update(float deltaTime) {
    if (!moving) {
        currentFrame = int(SDL_GetTicks() / 100) % 4;
        currentRow = 0;
    }
    else {
        Vector2D direction = target - position;
        float distance = direction.length();
        currentRow = 1;

        // dung lai khi den vi tri can den
        if (distance < 1.0f) {
            position = target;
            moving = false;
            updateRect();
            return;
        }

        // di chuyen den vi tri can den theo huong la vector direction
        Vector2D directionNormalized = direction.normalize();
        position += directionNormalized * speed * deltaTime;

        currentFrame = int(SDL_GetTicks() / 100) % 6;


        // Xác định hướng nhìn dựa vào hướng di chuyển
        if (directionNormalized.x > 0) {
            flip = SDL_FLIP_NONE;
        } else if (directionNormalized.x < 0) {
            flip = SDL_FLIP_HORIZONTAL;
        }
        updateRect();
    }

}

Vector2D& Character::getPosition() {
    return position;
}
void Character::render(SDL_Renderer* renderer) {
    // Ve texture voi frame hien tai va flip thich hop
    TextureManager::Instance()->drawFrame(
    "player",
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

bool Character::loadTextureWalking(const string& filePath, const string& id, SDL_Renderer* renderer) {
    if (TextureManager::Instance()->load(filePath, id, renderer)) {
        textureID = id;
        return true;
    }
    return false;
}

bool Character::loadTexture(const string& filePath, const string& id, SDL_Renderer* renderer) {
    if (TextureManager::Instance()->load(filePath, id, renderer)) {
        textureID = id;
        return true;
    }
    return false;
}
