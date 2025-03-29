//
// Created by Admin on 3/16/2025.
//

#include "Game.h"
#include <bits/stdc++.h>

using namespace std;

Game::Game() : window(nullptr), renderer(nullptr), player(nullptr), running(false), bullet(nullptr),
screenWidth(0), screenHeight(0), nextEnemySpawn(0.0f), mouseX(0), mouseY(0), currentState(GameState::MAIN_MENU),
score(0), scoreFont(nullptr), menuFont(nullptr){
    rng.seed(random_device()());
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

    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());
        return false;
    } else {
        SDL_Log("SDL_ttf Initialized Successful");
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

    scoreFont = TTF_OpenFont("../assets/entities/arial.ttf", 24);
    menuFont = TTF_OpenFont("../assets/entities/arial.ttf", 36);
    if (!scoreFont || !menuFont) {
        SDL_Log("Failed to load font! SDL_ttf Error: %s", TTF_GetError());
        return false;
    }

    textColor = {255, 255, 255, 255};
    resumeButtonTexture = "../assets/entities/resume.png";
    restartButtonTexture = "../assets/entities/restart.png";
    quitButtonTexture = "../assets/entities/home.png";
    startButtonTexture = "../assets/entities/home_play.png";


    if (!TextureManager::Instance()->init()) {
        SDL_Log("Texture manager could not be initialized");
        return false;
    } else {
        SDL_Log("Texture manager initialized Successful");
    }


    // nhan vat se duoc khoi tao mac dinh o chinh giua man hinh
    const int playerWidth = 128;
    const int playerHeight = 128;
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
    return this->running;
}

void Game::renderScore() {
    string scoreText = "High Score " + to_string(score);


    SDL_Surface* scoreSurface = TTF_RenderText_Solid(scoreFont, scoreText.c_str(), textColor);
    if (!scoreSurface) {
        SDL_Log("Unable to render score text surface! SDL_ttf Error: %s", TTF_GetError());
        return;
    }

    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    if (!scoreTexture) {
        SDL_Log("Unable to create score texture! SDL Error: %s", SDL_GetError());
        SDL_FreeSurface(scoreSurface);
        return;
    }

    int textWidth = scoreSurface->w;
    int textHeight = scoreSurface->h;

    SDL_Rect renderQuad = {
        screenWidth - textWidth - 20,
        20,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, scoreTexture, nullptr, &renderQuad);

    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
}

void Game::renderGameOver() {

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 1);
    SDL_RenderClear(renderer);


    if (TextureManager::Instance()->load(restartButtonTexture, "restart_button", renderer)) {
        int buttonWidth = 80;
        int buttonHeight = 83;
        SDL_Rect buttonRect = {
            (screenWidth - buttonWidth) / 2,
            (screenHeight - buttonHeight) / 2,
            buttonWidth,
            buttonHeight
        };

        TextureManager::Instance()->draw("restart_button", buttonRect.x, buttonRect.y,
                                         buttonWidth, buttonHeight, buttonWidth, buttonHeight,
                                         renderer);
    } else {
        SDL_Log("Unable to load restart texture! SDL Error: %s", SDL_GetError());
    }

    SDL_RenderPresent(renderer);
}

void Game::handleGameOverEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
            break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    int buttonWidth = 80;
                    int buttonHeight = 83;
                    SDL_Rect buttonRect = {
                        (screenWidth - buttonWidth) / 2,
                        (screenHeight - buttonHeight) / 2,
                        buttonWidth,
                        buttonHeight
                    };

                    if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
                        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
                        // Start game
                        currentState = GameState::PLAYING;
                        resetGame();
                        }
                }
            break;
        }
    }
}


void Game::renderMainMenu() {

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 1);
    SDL_RenderClear(renderer);

    TextureManager::Instance()->load("../assets/entities/home_background.png", "home_background", renderer);
    TextureManager::Instance()->draw("home_background", 0, 0, 1600, 900, 1600, 900, renderer);

    if (TextureManager::Instance()->load(startButtonTexture, "start_button", renderer)) {
        int buttonWidth = 160;
        int buttonHeight = 83;
        SDL_Rect buttonRect = {
            (screenWidth - buttonWidth) / 2,
            (screenHeight - buttonHeight) / 2,
            buttonWidth,
            buttonHeight
        };

        TextureManager::Instance()->draw("start_button", buttonRect.x, buttonRect.y,
                                         buttonWidth, buttonHeight, buttonWidth, buttonHeight,
                                         renderer);
    } else {
        SDL_Log("Unable to load start_button texture! SDL Error: %s", SDL_GetError());
    }

    SDL_RenderPresent(renderer);
}

void Game::renderPauseMenu() {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 1);
    SDL_Rect overlayRect = {0, 0, screenWidth, screenHeight};
    SDL_RenderFillRect(renderer, &overlayRect);

    int buttonWidth = 80;
    int buttonHeight = 83;
    int spacing = 20;

    // Resume button
    if (TextureManager::Instance()->load(resumeButtonTexture, "resume_button", renderer)) {
        SDL_Rect resumeRect = {
            (screenWidth - buttonWidth) / 2,
            (screenHeight - (buttonHeight * 3 + spacing * 2)) / 2,
            buttonWidth,
            buttonHeight
        };
        TextureManager::Instance()->draw("resume_button", resumeRect.x, resumeRect.y,
                                         buttonWidth, buttonHeight, buttonWidth, buttonHeight,
                                         renderer);
    }

    // Restart button
    if (TextureManager::Instance()->load(restartButtonTexture, "restart_button", renderer)) {
        SDL_Rect restartRect = {
            (screenWidth - buttonWidth) / 2,
            (screenHeight - (buttonHeight * 3 + spacing * 2)) / 2 + buttonHeight + spacing,
            buttonWidth,
            buttonHeight
        };
        TextureManager::Instance()->draw("restart_button", restartRect.x, restartRect.y,
                                         buttonWidth, buttonHeight, buttonWidth, buttonHeight,
                                         renderer);
    }

    // Quit button
    if (TextureManager::Instance()->load(quitButtonTexture, "quit_button", renderer)) {
        SDL_Rect quitRect = {
            (screenWidth - buttonWidth) / 2,
            (screenHeight - (buttonHeight * 3 + spacing * 2)) / 2 + (buttonHeight + spacing) * 2,
            buttonWidth,
            buttonHeight
        };
        TextureManager::Instance()->draw("quit_button", quitRect.x, quitRect.y,
                                         buttonWidth, buttonHeight, buttonWidth, buttonHeight,
                                         renderer);
    }
}

bool Game::loadAssets() {
    if (!loadTextureBackground("../assets/background/background.png", "background", renderer)) {
        SDL_Log("Failed to load background");
        return false;
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

    if (!TextureManager::Instance()->load("../assets/entities/enemy.png", "enemy", renderer)) {
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

void Game::handleMainMenuEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
            break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    // Check if start button was clicked
                    int buttonWidth = 200;
                    int buttonHeight = 100;
                    SDL_Rect buttonRect = {
                        (screenWidth - buttonWidth) / 2,
                        (screenHeight - buttonHeight) / 2,
                        buttonWidth,
                        buttonHeight
                    };

                    if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
                        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
                        // Start game
                        currentState = GameState::PLAYING;
                        resetGame();
                        }
                }
            break;
        }
    }
}

void Game::handlePauseMenuEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    currentState = GameState::PLAYING;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    int buttonWidth = 200;
                    int buttonHeight = 100;
                    int spacing = 20;

                    // Resume button
                    SDL_Rect resumeRect = {
                        (screenWidth - buttonWidth) / 2,
                        (screenHeight - (buttonHeight * 3 + spacing * 2)) / 2,
                        buttonWidth,
                        buttonHeight
                    };

                    // Restart button
                    SDL_Rect restartRect = {
                        (screenWidth - buttonWidth) / 2,
                        (screenHeight - (buttonHeight * 3 + spacing * 2)) / 2 + buttonHeight + spacing,
                        buttonWidth,
                        buttonHeight
                    };

                    // Quit button
                    SDL_Rect quitRect = {
                        (screenWidth - buttonWidth) / 2,
                        (screenHeight - (buttonHeight * 3 + spacing * 2)) / 2 + (buttonHeight + spacing) * 2,
                        buttonWidth,
                        buttonHeight
                    };

                    // Check resume button
                    if (mouseX >= resumeRect.x && mouseX <= resumeRect.x + resumeRect.w &&
                        mouseY >= resumeRect.y && mouseY <= resumeRect.y + resumeRect.h) {
                        currentState = GameState::PLAYING;
                    }
                    // Check restart button
                    else if (mouseX >= restartRect.x && mouseX <= restartRect.x + restartRect.w &&
                             mouseY >= restartRect.y && mouseY <= restartRect.y + restartRect.h) {
                        resetGame();
                        currentState = GameState::PLAYING;
                    }
                    // Check quit button
                    else if (mouseX >= quitRect.x && mouseX <= quitRect.x + quitRect.w &&
                             mouseY >= quitRect.y && mouseY <= quitRect.y + quitRect.h) {
                        currentState = GameState::MAIN_MENU;
                    }
                }
                break;
        }
    }
}

void Game::resetGame() {
    // Reset player position
    player->setPosition(screenWidth / 2.0f, screenHeight / 2.0f);

    // Clear enemies
    for (auto& enemy : enemies) {
        enemy->setActive(false);
    }

    // Reset score
    score = 0;

    // reset direction
    player->setTarget(player->getPosition().x, player->getPosition().y);

    // Reset timers
    timer.start();
    enemySpawnTimer.start();

    // Reset enemy spawn time
    uniform_real_distribution<float> dist(2.0f, 4.0f);
    nextEnemySpawn = dist(rng);
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
        bullet = new Bullet(0, 0, 32, 32);
        if (!bullet->loadTexture("../assets/entities/bullet.png", "bullet", renderer)) {
            SDL_Log("Failed to load bullet texture");
            delete bullet;
            bullet = nullptr;
            return;
        }
    }
    Vector2D bulletStartPos = startPos;

    if(bullet != nullptr){
        bullet->fire(bulletStartPos, direction);

    }
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

    switch (currentState) {
        case GameState::PLAYING:
            this->inputHandler.handleEvents(this->running, *this->player, *this, currentState);
        break;

        case GameState::MAIN_MENU:
            handleMainMenuEvents();
        break;

        case GameState::PAUSED:
            handlePauseMenuEvents();
        break;

        case GameState::GAME_OVER:
            handleGameOverEvents();
        break;
        default:
            break;
    }
}

void Game::update() {
    switch (currentState) {
        case GameState::PLAYING:

            // Update player
            player->update(timer.getDeltaTime());

            // Spawn enemy
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

            // Update bullet and check enemy collisions
            if (bullet != nullptr && bullet->isActive()) {
                bullet->update(timer.getDeltaTime());
                for (auto& enemy : enemies) {
                    if (enemy->isActive() && checkCollision(enemy->getRect(), bullet->getRect())) {
                        bullet->setActive(false);
                        enemy->setActive(false);
                        score++;
                        break;
                    }
                }
            }
            break;

        case GameState::MAIN_MENU:
            handleMainMenuEvents();
            break;

        case GameState::PAUSED:
            handlePauseMenuEvents();
            break;
        case GameState::GAME_OVER:
            handleGameOverEvents();
            break;
        default:
            break;
    }
}

void Game::render() {

    SDL_SetRenderDrawColor(renderer, 102,102,201,0);
    SDL_RenderClear(renderer);

    switch (currentState) {
        case GameState::MAIN_MENU:
            renderMainMenu();
        break;

        case GameState::PLAYING:
            // Render background
                if (!backgroundTextureID.empty()) {
                    TextureManager::Instance()->draw(backgroundTextureID, 0, 0, screenWidth, screenHeight,1024, 577, renderer);
                }

        // Render player
        player->render(renderer);

        // Render enemies
        for (auto& enemy : enemies) {
            if (enemy->isActive()) {
                enemy->render(renderer);
            }
        }

        // Render bullet
        if (bullet != nullptr && bullet->isActive()) {
            bullet->render(renderer);
        }

        // Render cooldowns and score
        renderCooldowns();
        renderScore();
        break;

        case GameState::PAUSED:
            if (!backgroundTextureID.empty()) {
                TextureManager::Instance()->draw(backgroundTextureID, 0, 0, screenWidth, screenHeight,screenWidth, screenHeight, renderer);
            }

            // Render player
            player->render(renderer);

            // Render enemies
            for (auto& enemy : enemies) {
                if (enemy->isActive()) {
                    enemy->render(renderer);
                }
            }

            // Render bullet
            if (bullet != nullptr && bullet->isActive()) {
                bullet->render(renderer);
            }

            // Then render pause menu overlay
            renderPauseMenu();
            break;

        case GameState::GAME_OVER:
            renderGameOver();
            break;

        default:
            break;
    }

    SDL_RenderPresent(renderer);
}

void Game::spawnEnemy() {

    Enemy* enemy = nullptr;

    if (enemy == nullptr) {

        enemy = new Enemy(0, 0, 200, 200);
        enemy->loadTexture("../assets/entities/enemy.png", "enemy", renderer);
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
    currentState = GameState::GAME_OVER;
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