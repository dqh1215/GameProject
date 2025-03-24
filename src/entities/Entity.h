//
// Created by Admin on 3/16/2025.
//

#ifndef ENTITY_H
#define ENTITY_H

// lop co so cua cac entities trong game. Cac lop con ke thua lop Entity

#include "../core/Vector2D.h"
#include <SDL2/SDL.h>
#include <bits/stdc++.h>
using namespace std;

class Entity {
public:
    Entity(float x, float y, int width, int height);
    virtual ~Entity() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    Vector2D getPosition() const;
    void setPosition(float x, float y);
    void setPosition(const Vector2D& position);

    SDL_Rect getRect() const;

protected:
    int width;
    int height;
    Vector2D position;
    Vector2D velocity;
    SDL_Rect rect{};

    void updateRect();
};



#endif //ENTITY_H
