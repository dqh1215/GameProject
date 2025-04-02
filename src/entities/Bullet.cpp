//
// Created by Admin on 3/17/2025.
//

#include "Bullet.h"
#include "../graphics/TextureManager.h"

Bullet::Bullet(float x, float y, int width, int height)
    : Entity(x, y, width, height),
      direction(0, 0),
      speed(800.0f),
      textureID(""),
      frameWidth(251),
      frameHeight(144),
      active(false),
      lifetime(0.0f),
      angle(0),
      maxLifetime(0.75f),
      flip(SDL_FLIP_NONE) {
}

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

void Bullet::render(SDL_Renderer *renderer) {
    if (!active) return;
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;

    destRect.x = rect.x;
    destRect.y = rect.y;
    destRect.w = width;
    destRect.h = height;

    SDL_RenderCopyEx(
        renderer,
        TextureManager::Instance()->getTexture(textureID),
        &srcRect,
        &destRect,
        angle,
        nullptr,
        flip
    );
}

bool Bullet::loadTexture(const string &filePath, const string &id, SDL_Renderer *renderer) {
    if (TextureManager::Instance()->load(filePath, id, renderer)) {
        textureID = id;
        return true;
    }
    return false;
}

void Bullet::fire(const Vector2D &startPosition, const Vector2D &direction) {
    active = true;
    lifetime = 0.0f;

    position = startPosition;

    this->direction = direction.normalize();
    this->angle = atan2(this->direction.y, this->direction.x) * (180.0 / M_PI);
    updateRect();
}
