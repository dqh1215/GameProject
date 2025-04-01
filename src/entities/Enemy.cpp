//
// Created by Admin on 3/17/2025.
//

#include "Enemy.h"
#include <bits/stdc++.h>
#include "../graphics/TextureManager.h"
using namespace std;

Enemy::Enemy(float x, float y, int width, int height)
    : Entity(x, y, width, height),
    target(0, 0),
    active(true),
    speed(80.0f),
    textureID(""),
    frameWidth(64),
    frameHeight(64),
    currentColumn(0),
    dead(false) {}

void Enemy::update(float deltaTime) {
    if (!active) {
        return;
    }
    Vector2D direction = target - position;
    float distance = direction.length();

    // di chuyen den vi tri can den theo huong la vector direction
    Vector2D directionNormalized = direction.normalize();
    position += directionNormalized * speed * deltaTime;

    currentColumn = int(SDL_GetTicks() / 200) % 5 + 1;

    // Xác định hướng nhìn dựa vào hướng di chuyển
    currentRow = 0;
    if (position.x < target.x && position.y < target.y) { // NW
        currentRow = 1;
    } else if (position.x < target.x && position.y > target.y && position.y < target.y + 64) { // W
        currentRow = 2;
    } else if (position.x < target.x && position.y > target.y + 64) { // SW
        currentRow = 3;
    } else if (position.x > target.x && position.y < target.y && position.x < target.x + 64) { // N
        currentRow = 0;
    } else if (position.x > target.x + 64 && position.y < target.y) { // NE
        currentRow = 7;
    } else if (position.x > target.x + 64 && position.y > target.y && position.y < target.y + 64) { // E
        currentRow = 6;
    } else if (position.x > target.x + 64 && position.y > target.y + 64) { // SE
        currentRow = 5;
    } else if (position.x > target.x && position.x < target.x + 64 && position.y > target.y + 64) {
        currentRow = 4;
    }

    updateRect();
}

void Enemy::render(SDL_Renderer* renderer) {
    if (dead) {
        return;
    }
    if (!active) {
        if (SDL_GetTicks() >= dyingTime) {
            dead = true;
        } else {
            currentColumn = int((600 - (dyingTime - SDL_GetTicks())) / 150) % 4 + 9;
            TextureManager::Instance()->drawFrame(
                textureID,
                rect.x, rect.y,
                width, height,
                currentColumn, currentRow,
                frameWidth, frameHeight,
                renderer,
                SDL_FLIP_NONE);
        }
    }

    TextureManager::Instance()->drawFrame(
        textureID,
        rect.x, rect.y,
        width, height,
        currentColumn, currentRow,
        frameWidth, frameHeight,
        renderer,
        SDL_FLIP_NONE
    );

}

void Enemy::die() {
    dyingTime = SDL_GetTicks() + 600;
    active = false;
}



void Enemy::setTarget(float x, float y) {
    target.x = x;
    target.y = y;
}


bool Enemy::loadTexture(const string& filePath, const string& id, SDL_Renderer* renderer) {
    if (TextureManager::Instance()->load(filePath, id, renderer)) {
        textureID = id;
        return true;
    }
    return false;
}
