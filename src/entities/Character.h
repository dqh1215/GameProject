//
// Created by Admin on 3/16/2025.
//

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Entity.h"

class Character : public Entity {
public:
    Character(float x, float y, int size);

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

    void setTarget(float x, float y);
    bool isMoving() const;

private:
    Vector2D target;
    bool moving;
    float speed;
};


#endif //CHARACTER_H
