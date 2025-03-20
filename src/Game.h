//
// Created by Admin on 3/16/2025.
//

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <bits/stdc++.h>
#include "core/Timer.h"
#include "entities/Bullet.h"
#include "entities/Character.h"
#include "entities/Enemy.h"
#include "input/InputHandler.h"
#include "graphics/TextureManager.h"
using namespace std;


class Game {
public:
    Game();
    ~Game();

    bool init(const string& title, int width = 0, int height = 0, bool fullscreen = true);
    void run();
    void clean();
    bool loadTextureBackground(const string& filePath, const string& id, SDL_Renderer* renderer);
    void shootProjectile(const Vector2D& startPos, const Vector2D& direction);
    void teleportPlayer(float x, float y);
private:
    void update();
    void render();
    void handleEvents();
    bool loadAssets();
    void spawnEnemy();
    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
    void gameOver();
    void renderCooldowns();


    string backgroundTextureID;
    SDL_Window* window;
    SDL_Renderer* renderer;

    Timer timer;
    Timer enemySpawnTimer;
    Character* player;
    InputHandler inputHandler;
    vector<Enemy*> enemies;
    Bullet* bullet;

    bool running;
    int screenWidth;
    int screenHeight;
    bool gameOverState;
    float nextEnemySpawn;
    int mouseX, mouseY;
    mt19937 rng;
};

#endif //GAME_H
