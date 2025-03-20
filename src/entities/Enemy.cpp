//
// Created by Admin on 3/17/2025.
//

#include "Enemy.h"
#include <bits/stdc++.h>
#include "../graphics/TextureManager.h"
using namespace std;

Enemy::Enemy(float x, float y, int width, int height)
    : Entity(x, y, width, height, 64, 64),
    target(0, 0),
    active(true),
    speed(150.0f),
    textureID(""),
    frameWidth(width),
    frameHeight(height),
    currentFrame(0),
    currentRow(0),
    flip(SDL_FLIP_NONE) {}

void Enemy::update(float deltaTime) {
    if (!active) {
        return;
    }
    Vector2D direction = target - position;
    float distance = direction.length();

    // di chuyen den vi tri can den theo huong la vector direction
    Vector2D directionNormalized = direction.normalize();
    position += directionNormalized * speed * deltaTime;

    currentFrame = int(SDL_GetTicks() / 100) % 8;

    // Xác định hướng nhìn dựa vào hướng di chuyển
    if (directionNormalized.x > 0) {
        flip = SDL_FLIP_NONE;
    } else if (directionNormalized.x < 0) {
        flip = SDL_FLIP_HORIZONTAL;
    }
    updateRect();
}

void Enemy::render(SDL_Renderer* renderer) {
    if (!active) return;

    TextureManager::Instance()->drawFrame(
        textureID,
        rect.x, rect.y,
        visualWidth, visualHeight,       // Display size
        currentRow, currentFrame,
        frameWidth, frameHeight,         // Source frame size
        renderer,
        flip
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
