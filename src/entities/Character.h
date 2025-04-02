//
// Created by Admin on 3/16/2025.
//

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Entity.h"

class Character : public Entity {
public:
    Character(float x, float y, int width, int height);

    void update(float deltaTime) override;

    void render(SDL_Renderer *renderer) override;

    void setTarget(float x, float y);

    [[nodiscard]] bool isMoving() const;

    Vector2D &getPosition();

    // Các chiêu thức mới
    void shoot(float mouseX, float mouseY);

    void teleport(float mouseX, float mouseY);

    // Thêm cooldown cho các chiêu thức
    [[nodiscard]] bool canShoot() const;

    [[nodiscard]] bool canTeleport() const;

    bool loadTexture(const string &filePath, const string &id, SDL_Renderer *renderer);

private:
    Vector2D target;
    bool moving;
    float speed;
    string textureID;
    int frameWidth;
    int frameHeight;
    int currentColumn;
    int currentRow;
    SDL_RendererFlip flip;

    float angle;
    int shootingTime = 0;
    bool isShooting = false;
    float shootCooldown;
    float shootTimer;
    float teleportCooldown;
    float teleportTimer;
    float teleportDistance;
};


#endif //CHARACTER_H
