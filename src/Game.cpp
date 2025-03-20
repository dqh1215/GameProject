//
// Created by Admin on 3/16/2025.
//

#include "Game.h"
#include <bits/stdc++.h>
using namespace std;

Game::Game() : window(nullptr), renderer(nullptr), player(nullptr), running(false), bullet(nullptr),
               screenWidth(0), screenHeight(0), gameOverState(false), nextEnemySpawn(0.0f), mouseX(0), mouseY(0) {
    rng.seed(std::random_device()());
}

Game::~Game() {
    clean();
}

bool Game::init(const string& title, int width, int height, bool fullscreen) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return false;
    } else {
        SDL_Log("SDL_Init Successful");
    }

    this->screenWidth = width;
    this->screenHeight = height;

    Uint32 flags = SDL_WINDOW_RESIZABLE;
    if (fullscreen == true) {
        flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             screenWidth, screenHeight, flags);

    if (!this->window) {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        return false;
    } else {
        SDL_Log("SDL_CreateWindow Successful");
    }

    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!this->renderer) {
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        return false;
    } else {
        SDL_Log("Renderer created Successful");
    }


    if (!TextureManager::Instance()->init()) {
        SDL_Log("Texture manager could not be initialized");
        return false;
    } else {
        SDL_Log("Texture manager initialized Successful");
    }


    // nhan vat se duoc khoi tao mac dinh o chinh giua man hinh
    const int playerWidth = 64;  // Kich thước sprite
    const int playerHeight = 64;
    player = new Character(screenWidth / 2.0f, screenHeight / 2.0f, playerWidth, playerHeight);

    // Load assets
    if (!loadAssets()) {
        SDL_Log("Failed to load assets");
        return false;
    }

    // khoi tao timer de tinh thoi gian
    timer.start();
    enemySpawnTimer.start();

    // enemy se duoc spawn tu 2 den 4 giay
    uniform_real_distribution<float> dist(2.0f, 4.0f);
    nextEnemySpawn = dist(rng);

    this->running = true;
    this->gameOverState = false;
    return this->running;
}


bool Game::loadAssets() {
    if (!loadTextureBackground("../assets/background/background.png", "background", renderer)) {
        SDL_Log("Failed to load background");
    } else {
        SDL_Log("Loaded background successful");
    }

    // Load texture cho nhan vat
    if (!player->loadTexture("../assets/entities/player.png", "player", renderer)) {
        SDL_Log("Failed to load player");
        return false;
    } else {
        SDL_Log("Loaded player successful");
    }

    // Load texture cho enemy
    string enemyRandomFilePath = "../assets/entities/enemy1.png";
    if (!TextureManager::Instance()->load(enemyRandomFilePath , "enemy", renderer)) {
        SDL_Log("Failed to load enemy texture");
        return false;
    }
    SDL_Log("Loaded enemy texture successful");

    if (!TextureManager::Instance()->load("../assets/entities/enemy1.png", "enemy", renderer)) {
        SDL_Log("Failed to load enemy texture");
        return false;
    }
    SDL_Log("Loaded enemy texture successful");

    // Load texture cho bullet
    if (!TextureManager::Instance()->load("../assets/entities/bullet.png", "bullet", renderer)) {
        SDL_Log("Failed to load bullet texture");
        return false;
    }
    SDL_Log("Loaded bullet texture successful");
    return true;
}

bool Game::loadTextureBackground(const string& filePath, const string& id, SDL_Renderer* renderer) {
    if (TextureManager::Instance()->load(filePath, id, renderer)) {
        backgroundTextureID = id;
        return true;
    }
    return false;
}

void Game::renderCooldowns() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect shootCooldownRect = {20, 20, 30, 30};

    if (player->canShoot()) {
        SDL_RenderFillRect(renderer, &shootCooldownRect);
    } else {
        SDL_RenderDrawRect(renderer, &shootCooldownRect);
    }

    SDL_Rect teleportCooldownRect = {60, 20, 30, 30};

    if (player->canTeleport()) {
        SDL_RenderFillRect(renderer, &teleportCooldownRect);
    } else {
        SDL_RenderDrawRect(renderer, &teleportCooldownRect);
    }
}

void Game::shootProjectile(const Vector2D &startPos, const Vector2D &direction) {
    if (bullet == nullptr) {
        bullet = new Bullet(0, 0, 199, 199);
        bullet->loadTexture("../assets/entities/bullet.png", "bullet", renderer);
    }
    bullet->fire(startPos, direction);
}

void Game::teleportPlayer(float x, float y) {
    player->teleport(x, y);
}

// Game Loop
void Game::run() {
    while (running) {
        timer.tick();
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    SDL_GetMouseState(&mouseX, &mouseY);
    this->inputHandler.handleEvents(this->running, *this->player, *this);
}

void Game::update() {
    if (gameOverState) return;
    // update player
    player->update(timer.getDeltaTime());

    // spawn enemy
    enemySpawnTimer.tick();
    if (enemySpawnTimer.getElapsedTime() >= nextEnemySpawn) {
        spawnEnemy();
        enemySpawnTimer.start();

        // Set next spawn time (2-4 seconds)
        uniform_real_distribution<float> dist(2.0f, 4.0f);
        nextEnemySpawn = dist(rng);
    }

    // Update enemies
    for (auto& enemy : enemies) {
        if (enemy->isActive()) {
            enemy->update(timer.getDeltaTime());
            enemy->setTarget(player->getPosition().x, player->getPosition().y);
            // Check collision with player
            if (checkCollision(player->getRect(), enemy->getRect())) {
                gameOver();
                return;
            }
        }
    }

    // if (bullet->isActive()) {
        bullet->update(timer.getDeltaTime());
        // for (auto& enemy : enemies) {
        //     if (enemy->isActive() && checkCollision(enemy->getRect(), bullet->getRect())) {
        //         bullet->setActive(false);
        //         enemy->setActive(false);
        //         break;
        //     }
        // }
    // }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 102,102,201,0);
    SDL_RenderClear(renderer);
    // render background
    if (!backgroundTextureID.empty()) {
        TextureManager::Instance()->draw(backgroundTextureID, 0, 0, screenWidth, screenHeight, renderer);
    }
    // render nhan vat
    player->render(renderer);

    // render enemies
    for (auto& enemy : enemies) {
        if (enemy->isActive()) {
            enemy->render(renderer);
        }
    }

    if (bullet != nullptr && bullet->isActive()) {
        bullet->render(renderer);
    }

    renderCooldowns();
    // Game over state
    if (gameOverState) {
        TextureManager::Instance()->clearFromTextureMap(backgroundTextureID);
    }

    // hien thi hinh anh
    SDL_RenderPresent(renderer);
}

void Game::spawnEnemy() {
    // Find an inactive enemy or create a new one if there's space
    Enemy* enemy = nullptr;
    for (auto& e : enemies) {
        if (!e->isActive()) {
            enemy = e;
            break;
        }
    }

    if (enemy == nullptr) {
        enemy = new Enemy(0, 0, 64, 64);
        enemy->loadTexture("../assets/entities/enemy" + to_string(rand() % 3 + 1) + ".png", "enemy", renderer);
        enemies.push_back(enemy);
    }

    if (enemy) {
        // Randomly choose a side to spawn from
        std::uniform_int_distribution<int> sideDist(0, 3);
        int side = sideDist(rng);

        float x = 0, y = 0;

        // Determine spawn position based on side
        switch(side) {
            case 0: // Top
                x = std::uniform_real_distribution<float>(0.0f, static_cast<float>(screenWidth))(rng);
                y = -50.0f;
                break;
            case 1: // Right
                x = screenWidth + 50.0f;
                y = std::uniform_real_distribution<float>(0.0f, static_cast<float>(screenHeight))(rng);
                break;
            case 2: // Bottom
                x = std::uniform_real_distribution<float>(0.0f, static_cast<float>(screenWidth))(rng);
                y = screenHeight + 50.0f;
                break;
            case 3: // Left
                x = -50.0f;
                y = std::uniform_real_distribution<float>(0.0f, static_cast<float>(screenHeight))(rng);
                break;
        }

        enemy->setPosition(x, y);
        enemy->setActive(true);
    }
}

bool Game::checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    // Check if two rectangles overlap
    return (a.x < b.x + b.w - 20 &&
            a.x + a.w - 20 > b.x &&
            a.y < b.y + b.h - 20 &&
            a.y + a.h - 20 > b.y);
}

void Game::gameOver() {
    SDL_Log("Game Over!");
    gameOverState = true;

}

void Game::clean() {
    if (this->player) {
        delete player;
        player = nullptr;
    }

    if (this->bullet) {
        delete bullet;
        bullet = nullptr;
    }

    TextureManager::Instance()->clean();

    if (this->renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (this->window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}