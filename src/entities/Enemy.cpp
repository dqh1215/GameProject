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
    speed(150.0f),
    textureID(""),
    frameWidth(128),
    frameHeight(128),
    currentFrame(0) {}

void Enemy::update(float deltaTime) {
    if (!active) {
        return;
    }
    Vector2D direction = target - position;
    float distance = direction.length();

    // di chuyen den vi tri can den theo huong la vector direction
    Vector2D directionNormalized = direction.normalize();
    position += directionNormalized * speed * deltaTime;

    currentFrame = int(SDL_GetTicks() / 40) % 25;

    // Xác định hướng nhìn dựa vào hướng di chuyển
    currentRow = 0;
    if (position.x < target.x && position.y < target.y) {
        currentRow = 5;
    } else if (position.x < target.x && position.y > target.y && position.y < target.y + 128) {
        currentRow = 0;
    } else if (position.x < target.x && position.y > target.y + 128) {
        currentRow = 2;
    } else if (position.x > target.x && position.y < target.y && position.x < target.x + 128) {
        currentRow = 4;
    } else if (position.x > target.x + 128 && position.y < target.y) {
        currentRow = 6;
    } else if (position.x > target.x + 128 && position.y > target.y && position.y < target.y + 128) {
        currentRow = 7;
    } else if (position.x > target.x + 128 && position.y > target.y + 128) {
        currentRow = 3;
    } else if (position.x > target.x && position.x < target.x + 128 && position.y > target.y + 128) {
        currentRow = 1;
    }

    updateRect();
}

void Enemy::render(SDL_Renderer* renderer) {
    if (!active) return;

    TextureManager::Instance()->drawFrame(
        textureID,
        rect.x, rect.y,
        width, height,
        currentRow, currentFrame,
        frameWidth, frameHeight,
        renderer,
        SDL_FLIP_NONE
    );

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
