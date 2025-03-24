//
// Created by Admin on 3/16/2025.
//

#include "Character.h"
#include "Bullet.h"
#include <bits/stdc++.h>
#include "../graphics/TextureManager.h"
using namespace std;

Character::Character(float x, float y, int width, int height)
    : Entity(x, y, width, height),
    target(x, y),
    moving(false),
    speed(250.0f),
    textureID(""),
    frameWidth(64),
    frameHeight(64),
    currentFrame(0),
    currentRow(0),
    flip(SDL_FLIP_NONE),
    shootCooldown(2.0f),
    shootTimer(0.0f),
    teleportCooldown(3.0f),
    teleportTimer(0.0f),
    teleportDistance(100.0f) {}

void Character::update(float deltaTime) {
    if (shootTimer > 0) {
        shootTimer -= deltaTime;
    }
    if (teleportTimer > 0) {
        teleportTimer -= deltaTime;
    }
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
        textureID,
        rect.x, rect.y,
        width, height,
        currentRow, currentFrame,
        frameWidth, frameHeight,
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


bool Character::loadTexture(const string& filePath, const string& id, SDL_Renderer* renderer) {
    if (TextureManager::Instance()->load(filePath, id, renderer)) {
        textureID = id;
        return true;
    }
    return false;
}

void Character::shoot(float mouseX, float mouseY) {
    if (this->canShoot()) {
        Vector2D mousePos(mouseX, mouseY);
        Vector2D direction = mousePos - position;
        if (direction.x > 0) {
            flip = SDL_FLIP_NONE;
        } else {
            flip = SDL_FLIP_HORIZONTAL;
        }
        shootTimer = shootCooldown;
    }
}

void Character::teleport(float mouseX, float mouseY) {
    if (this->canTeleport()) {
        Vector2D mousePos(mouseX, mouseY);
        Vector2D direction = mousePos - position;
        float distance = direction.length();

        if (distance > teleportDistance) {
            Vector2D normalizedDirection = direction.normalize();
            mousePos = position + (normalizedDirection * teleportDistance);
        }

        position = mousePos;
        updateRect();

        moving = false;
        target = position;

        teleportTimer = teleportCooldown;
    }
}

bool Character::canShoot() const {
    return shootTimer <= 0.0f;
}

bool Character::canTeleport() const {
    return teleportTimer <= 0.0f;
}