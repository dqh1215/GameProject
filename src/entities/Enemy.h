//
// Created by Admin on 3/17/2025.
//

#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(float x, float y, int width, int height);

    void update(float deltaTime) override;

    void render(SDL_Renderer *renderer) override;

    void setTarget(float x, float y);

    [[nodiscard]] bool isActive() const { return active; }

    void die();

    [[nodiscard]] bool isDead() const { return dead; }

    bool loadTexture(const string &filePath, const string &id, SDL_Renderer *renderer);

private:
    Vector2D target;
    bool active;
    float speed;
    string textureID;
    int frameWidth;
    int frameHeight;
    Uint32 dyingTime;
    bool dead;
    int currentColumn;
    int currentRow;
};


#endif //ENEMY_H
