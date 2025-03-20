//
// Created by Admin on 3/17/2025.
//

#include "Bullet.h"
#include "../graphics/TextureManager.h"

Bullet::Bullet(float x, float y, int width, int height)
    : Entity(x, y, width, height),
      direction(0, 0),
      speed(500.0f),
      textureID(""),
      active(false),
      lifetime(0.0f),
      maxLifetime(2.0f),
      flip(SDL_FLIP_NONE)
{}

void Bullet::update(float deltaTime) {
    if (!active) return;

    position += direction * speed * deltaTime;
    updateRect();

    lifetime += deltaTime;

    if (lifetime >= maxLifetime) {
        active = false;
        lifetime = 0.0f;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    if (!active) return;

    TextureManager::Instance()->draw(
        textureID,
        rect.x, rect.y,
        rect.w, rect.h,
        renderer,
        flip
    );
}

bool Bullet::loadTexture(const string& filePath, const string& id, SDL_Renderer* renderer) {
    if (TextureManager::Instance()->load(filePath, id, renderer)) {
        textureID = id;
        return true;
    }
    return false;
}

void Bullet::fire(const Vector2D& startPosition, const Vector2D& direction) {
    active = true;
    lifetime = 0.0f;

    position = startPosition;

    this->direction = direction.normalize();

    if (this->direction.x < 0) {
        flip = SDL_FLIP_HORIZONTAL;
    } else {
        flip = SDL_FLIP_NONE;
    }

    updateRect();
}