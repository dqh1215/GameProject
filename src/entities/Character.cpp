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
      currentColumn(0),
      currentRow(0),
      flip(SDL_FLIP_NONE),
      angle(0),
      shootCooldown(2.0f),
      shootTimer(0.0f),
      teleportCooldown(3.0f),
      teleportTimer(0.0f),
      teleportDistance(100.0f) {
}

void Character::update(float deltaTime) {
    if (shootTimer > 0) {
        shootTimer -= deltaTime;
    }
    if (teleportTimer > 0) {
        teleportTimer -= deltaTime;
    }
    if (!moving) {
        currentColumn = 0;
    } else {
        Vector2D direction = target - position;
        float distance = direction.length();

        // dung lai khi den vi tri can den
        if (distance < 2.0f) {
            position = target;
            currentColumn = 0;
            moving = false;
            updateRect();
            return;
        }

        Vector2D directionNormalized = direction.normalize();
        position += directionNormalized * speed * deltaTime;

        currentColumn = static_cast<int>(SDL_GetTicks() / 200) % 3 + 2;

        if (position.x <= target.x && position.y <= target.y) {
            // NW
            currentRow = 1;
        } else if (position.x <= target.x && position.y >= target.y && position.y <= target.y + 64) {
            // W
            currentRow = 2;
        } else if (position.x <= target.x && position.y >= target.y + 64) {
            // SW
            currentRow = 3;
        } else if (position.x >= target.x && position.y <= target.y && position.x <= target.x + 64) {
            // N
            currentRow = 0;
        } else if (position.x >= target.x + 64 && position.y <= target.y) {
            // NE
            currentRow = 7;
        } else if (position.x >= target.x + 64 && position.y >= target.y && position.y <= target.y + 64) {
            // E
            currentRow = 6;
        } else if (position.x >= target.x + 64 && position.y >= target.y + 64) {
            // SE
            currentRow = 5;
        } else if (position.x >= target.x && position.x <= target.x + 64 && position.y >= target.y + 64) {
            // S
            currentRow = 4;
        }

        updateRect();
    }
}

Vector2D &Character::getPosition() {
    return position;
}

void Character::render(SDL_Renderer *renderer) {
    if (isShooting && SDL_GetTicks() < shootingTime) {
        TextureManager::Instance()->draw(
            textureID,
            rect.x, rect.y,
            width, height,
            1, currentRow,
            frameWidth, frameHeight,
            renderer,
            flip,
            0
        );
    } else {
        if (isShooting && SDL_GetTicks() >= shootingTime) {
            isShooting = false;
        }
        TextureManager::Instance()->draw(
            textureID,
            rect.x, rect.y,
            width, height,
            currentColumn, currentRow,
            frameWidth, frameHeight,
            renderer,
            flip,
            0
        );
    }
}

void Character::setTarget(float x, float y) {
    target.x = x;
    target.y = y;
    moving = true;
}

bool Character::isMoving() const {
    return moving;
}


bool Character::loadTexture(const string &filePath, const string &id, SDL_Renderer *renderer) {
    if (TextureManager::Instance()->load(filePath, id, renderer)) {
        textureID = id;
        return true;
    }
    return false;
}

void Character::shoot(float mouseX, float mouseY) {
    if (this->canShoot()) {
        Vector2D mousePos(mouseX, mouseY);
        moving = false;
        isShooting = true;
        Vector2D direction = mousePos;
        if (direction.x >= position.x + 64 && direction.y >= position.y + 64) {
            // SE
            currentRow = 1;
        } else if (direction.x >= position.x && direction.x <= position.x + 64 && direction.y >= position.y) {
            // S
            currentRow = 0;
        } else if (direction.x >= position.x && direction.y >= position.y && direction.y <= position.y + 64) {
            // E
            currentRow = 2;
        } else if (direction.x >= position.x + 64 && direction.y <= position.y) {
            // NE
            currentRow = 3;
        } else if (direction.x >= position.x && direction.x <= position.x + 64 && direction.y <= position.y) {
            // N
            currentRow = 4;
        } else if (direction.x <= position.x && direction.y <= position.y) {
            // NW
            currentRow = 5;
        } else if (direction.x <= position.x && direction.y >= position.y && direction.y <= position.y + 64) {
            // W
            currentRow = 6;
        } else if (direction.x <= position.x && direction.y >= position.y + 64) {
            // SW
            currentRow = 7;
        }
        shootingTime = SDL_GetTicks() + 200;
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
        Vector2D oldPosition = position;
        position = mousePos;
        updateRect();

        direction = mousePos;
        if (direction.x >= oldPosition.x + 64 && direction.y >= oldPosition.y + 64) {
            // SE
            currentRow = 1;
        } else if (direction.x >= oldPosition.x && direction.x <= oldPosition.x + 64 && direction.y >= oldPosition.y) {
            // S
            currentRow = 0;
        } else if (direction.x >= oldPosition.x && direction.y >= oldPosition.y && direction.y <= oldPosition.y + 64) {
            // E
            currentRow = 2;
        } else if (direction.x >= oldPosition.x + 64 && direction.y <= oldPosition.y) {
            // NE
            currentRow = 3;
        } else if (direction.x >= oldPosition.x && direction.x <= oldPosition.x + 64 && direction.y <= oldPosition.y) {
            // N
            currentRow = 4;
        } else if (direction.x <= oldPosition.x && direction.y <= oldPosition.y) {
            // NW
            currentRow = 5;
        } else if (direction.x <= oldPosition.x && direction.y >= oldPosition.y && direction.y <= oldPosition.y + 64) {
            // W
            currentRow = 6;
        } else if (direction.x <= oldPosition.x && direction.y >= oldPosition.y + 64) {
            // SW
            currentRow = 7;
        }

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
