//
// Created by Admin on 3/16/2025.
//

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
#include "core/Timer.h"
#include <SDL_ttf.h>
#include "entities/Bullet.h"
#include "entities/Character.h"
#include "entities/Enemy.h"
#include "input/InputHandler.h"
#include "graphics/TextureManager.h"
using namespace std;

enum class GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
public:
    Game();

    ~Game();

    bool init(const string &title, int width = 0, int height = 0, bool fullscreen = true);

    void run();

    void clean();

    void shootProjectile(const Vector2D &startPos, const Vector2D &direction);

    void teleportPlayer(float x, float y);

private:
    void update();

    void render();

    void handleEvents();

    bool loadAssets();

    void spawnEnemy();

    bool checkCollision(const SDL_Rect &a, const SDL_Rect &b);

    void gameOver();

    void renderPlaying();

    void handlePlayingEvents();

    void renderCooldowns();

    TTF_Font* getFontWithSize(int size);

    void renderText(const string& text, int x, int y, SDL_Color color, int fontSize = 24);

    void renderScore();

    void renderGameOverText();

    void renderMainMenuText();

    void renderPauseText();

    bool loadAudio();

    void renderPauseMenu();

    void renderMainMenu();

    void renderGameOver();

    void handleGameOverEvents();

    void handleMainMenuEvents();

    void handlePauseMenuEvents();

    void resetGame();

    SDL_Window *window;
    SDL_Renderer *renderer;

    Timer timer;
    Timer enemySpawnTimer;
    Character *player;
    InputHandler inputHandler;
    vector<Enemy *> enemies;
    Bullet *bullet;
    Mix_Music *themeMusic;
    TTF_Font *font;
    map<int, TTF_Font*> fontMap;
    map<string, SDL_Texture*> textTextureCache;

    GameState currentState;
    int score;
    bool running;
    bool gameOverState;
    int screenWidth;
    int screenHeight;
    float nextEnemySpawn;
    int mouseX, mouseY;
    mt19937 rng;
};

#endif //GAME_H
