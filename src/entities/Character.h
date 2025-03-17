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
    void render(SDL_Renderer* renderer) override;

    void setTarget(float x, float y);
    bool isMoving() const;

    bool loadTexture(const string& filePath, const string& id, SDL_Renderer* renderer);
private:
    Vector2D target;
    bool moving;
    float speed;
    string textureID;
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int currentRow;
    SDL_RendererFlip flip;
    Vector2D lastDirection;
};


#endif //CHARACTER_H
