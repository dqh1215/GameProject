//
// Created by Admin on 3/17/2025.
//

#ifndef BULLET_H
#define BULLET_H
#include "Entity.h"
#include <bits/stdc++.h>
using namespace std;

class Bullet : public Entity {
public:
    Bullet(float x, float y, int width, int height);

    void update(float deltaTime) override;

    void render(SDL_Renderer *renderer) override;

    bool loadTexture(const string &filePath, const string &id, SDL_Renderer *renderer);

    void fire(const Vector2D &startPosition, const Vector2D &direction);

    [[nodiscard]] bool isActive() const { return active; }
    void setActive(bool active) { this->active = active; }

    // Hàm để lấy thời gian tồn tại
    [[nodiscard]] float getLifetime() const { return lifetime; }

private:
    Vector2D direction;
    float speed;
    string textureID;
    int frameWidth;
    int frameHeight;
    bool active;
    float lifetime;
    float angle;
    float maxLifetime;
    SDL_RendererFlip flip;
};


#endif //BULLET_H
