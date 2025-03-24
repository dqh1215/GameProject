//
// Created by Admin on 3/16/2025.
//

#include "Entity.h"

Entity::Entity(float x, float y, int width, int height)
    : position(x, y), velocity(0, 0),
      width(width), height(height) {
    rect.w = width;
    rect.h = height;
    updateRect();
}

Vector2D Entity::getPosition() const {
    return position;
}

void Entity::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    updateRect();
}

void Entity::setPosition(const Vector2D& position) {
    this->position = position;
    updateRect();
}

SDL_Rect Entity::getRect() const {
    return rect;
}

void Entity::updateRect() {
    rect.x = static_cast<int>(position.x - width / 2);
    rect.y = static_cast<int>(position.y - height / 2);
    rect.w = width;
    rect.h = height;
}
