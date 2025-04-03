//
// Created by Admin on 3/16/2025.
//

#include "Game.h"
#include <bits/stdc++.h>

using namespace std;

Game::Game() : window(nullptr), renderer(nullptr), player(nullptr), bullet(nullptr),
               currentState(GameState::MAIN_MENU), score(0), running(false), gameOverState(false),
               screenWidth(0),
               screenHeight(0), nextEnemySpawn(0.0f), mouseX(0), mouseY(0) {
    rng.seed(random_device()());
}

Game::~Game() {
    clean();
}

bool Game::init(const string &title, int width, int height, bool fullscreen) {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 1);
    SDL_RenderClear(renderer);
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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

    font = TTF_OpenFont("../assets/fonts/ChangaOne-Regular.ttf", 24);
    if (!font) {
        SDL_Log("Failed to load font! SDL_ttf Error: %s", TTF_GetError());
        return false;
    }

    if (!TextureManager::Instance()->init()) {
        SDL_Log("Texture manager could not be initialized");
        return false;
    } else {
        SDL_Log("Texture manager initialized Successful");
    }

    player = new Character(screenWidth / 2.0f, screenHeight / 2.0f, 64, 64);

    // Load assets
    if (!loadAssets()) {
        SDL_Log("Failed to load assets");
        return false;
    }

    // Load audio
    if (!loadAudio()) {
        SDL_Log("Failed to load audio");
        return false;
    }

    // khoi tao timer de tinh thoi gian
    timer.start();
    enemySpawnTimer.start();

    this->running = true;
    return this->running;

}

bool Game::loadAudio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer không thể khởi tạo! Lỗi SDL_mixer: %s", Mix_GetError());
        return false;
    }
    if (!Mix_LoadWAV("../assets/sounds/gunshot.wav")) {
        SDL_Log("Unable to load gun shot sound: %s", Mix_GetError());
        return false;
    }
    if (!Mix_LoadWAV("../assets/sounds/flash.wav")) {
        SDL_Log("Unable to load flash sound: %s", Mix_GetError());
        return false;
    }
    return true;
}

TTF_Font* Game::getFontWithSize(int size) {
    if (fontMap.find(size) != fontMap.end()) {
        return fontMap[size];
    }

    TTF_Font* newFont = TTF_OpenFont("../assets/fonts/ChangaOne-Regular.ttf", size);
    if (!newFont) {
        SDL_Log("Failed to load font with size %d! SDL_ttf Error: %s", size, TTF_GetError());
        return font;
    }

    fontMap[size] = newFont;
    return newFont;
}

void Game::renderText(const string& text, int x, int y, SDL_Color color, int fontSize) {
    if (text.empty()) return;
    string cacheKey = text + "_" + to_string(fontSize) + "_" +
                      to_string(color.r) + "_" + to_string(color.g) + "_" +
                      to_string(color.b) + "_" + to_string(color.a);

    SDL_Texture* textTexture = nullptr;
    int textWidth = 0, textHeight = 0;

    auto it = textTextureCache.find(cacheKey);
    if (it != textTextureCache.end()) {
        textTexture = it->second;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
    } else {
        TTF_Font* currentFont = getFontWithSize(fontSize);

        SDL_Surface* textSurface = TTF_RenderText_Solid(currentFont, text.c_str(), color);
        if (!textSurface) {
            SDL_Log("Unable to render text surface! SDL_ttf Error: %s", TTF_GetError());
            return;
        }

        textWidth = textSurface->w;
        textHeight = textSurface->h;

        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        if (!textTexture) {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s", SDL_GetError());
            return;
        }

        textTextureCache[cacheKey] = textTexture;
    }

    int posX = x;
    int posY = y;

    if (x < 0) posX = (screenWidth - textWidth) / 2;
    if (y < 0) posY = (screenHeight - textHeight) / 2;

    SDL_Rect destRect = {
        posX,
        posY,
        textWidth,
        textHeight
    };
    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
}

void Game::renderScore() {
    string scoreText = "High Score " + to_string(score);
    renderText(scoreText, screenWidth - 170, 20, {255, 0, 0, 255}, 28);
}

void Game::renderGameOverText() {
    renderText("GAME OVER", -1, screenHeight / 2 - 50, {0, 60, 0, 1}, 100);
}

void Game::renderMainMenuText() {
    renderText("ZOMBIE DODGE", -1, screenHeight / 2 - 150, {58, 91, 0, 225}, 100);
}

void Game::renderPauseText() {
    renderText("PAUSE", -1, screenHeight / 2 - 50, {0, 60, 0, 255}, 100);
}

void Game::renderGameOver() {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 1);
    SDL_RenderClear(renderer);

    if (TextureManager::Instance()->load("../assets/buttons/restart.png", "restart_button", renderer)) {
        int buttonWidth = 80;
        int buttonHeight = 83;
        SDL_Rect buttonRect = {
            screenWidth / 2 - buttonWidth - 20,
            screenHeight / 2,
            buttonWidth,
            buttonHeight
        };
        TextureManager::Instance()->draw("restart_button", buttonRect.x, buttonRect.y,
                                         buttonWidth, buttonHeight, buttonWidth, buttonHeight,
                                         renderer);
    } else {
        SDL_Log("Unable to load restart texture! SDL Error: %s", SDL_GetError());
    }

    if (TextureManager::Instance()->load("../assets/buttons/home.png", "home_button", renderer)) {
        int buttonWidth = 80;
        int buttonHeight = 83;
        SDL_Rect buttonRect = {
            screenWidth / 2 + 20,
            screenHeight / 2,
            buttonWidth,
            buttonHeight
        };
        TextureManager::Instance()->draw("home_button", buttonRect.x, buttonRect.y,
                                         buttonWidth, buttonHeight, buttonWidth, buttonHeight,
                                         renderer);
    } else {
        SDL_Log("Unable to load home button texture! SDL Error: %s", SDL_GetError());
    }
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
                    SDL_Rect buttonRect1 = {
                        screenWidth / 2 - buttonWidth - 20,
                        screenHeight / 2,
                        buttonWidth,
                        buttonHeight
                    };

                    if (mouseX >= buttonRect1.x && mouseX <= buttonRect1.x + buttonRect1.w &&
                        mouseY >= buttonRect1.y && mouseY <= buttonRect1.y + buttonRect1.h) {
                        // Start playing again
                        resetGame();
                        currentState = GameState::PLAYING;
                    }

                    SDL_Rect buttonRect2 = {
                        screenWidth / 2 + 20,
                        screenHeight / 2,
                        buttonWidth,
                        buttonHeight
                    };

                    if (mouseX >= buttonRect2.x && mouseX <= buttonRect2.x + buttonRect2.w &&
                        mouseY >= buttonRect2.y && mouseY <= buttonRect2.y + buttonRect2.h) {
                        // return to main menu
                        resetGame();
                        currentState = GameState::MAIN_MENU;
                    }
                }
                break;
            default:
                break;
        }
    }
}


void Game::renderMainMenu() {
    if (TextureManager::Instance()->load("../assets/background/home_background.png", "home_background", renderer)) {
        TextureManager::Instance()->draw("home_background", 0, 0, 1600, 900, 1600, 900, renderer);
    }

    if (TextureManager::Instance()->load("../assets/buttons/start.png", "start_button", renderer)) {
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
}

void Game::renderPauseMenu() {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 1);
    SDL_Rect overlayRect = {0, 0, screenWidth, screenHeight};
    SDL_RenderFillRect(renderer, &overlayRect);

    int buttonWidth = 80;
    int buttonHeight = 83;

    // Resume button
    if (TextureManager::Instance()->load("../assets/buttons/resume.png", "resume_button", renderer)) {
        SDL_Rect resumeRect = {
            screenWidth / 2 - 140,
            screenHeight / 2 + 20,
            buttonWidth,
            buttonHeight
        };
        TextureManager::Instance()->draw("resume_button", resumeRect.x, resumeRect.y,
                                         buttonWidth, buttonHeight, buttonWidth, buttonHeight,
                                         renderer);
    }

    // Restart button
    if (TextureManager::Instance()->load("../assets/buttons/restart.png", "restart_button", renderer)) {
        SDL_Rect restartRect = {
            screenWidth / 2 - 40,
            screenHeight / 2 + 20,
            buttonWidth,
            buttonHeight
        };
        TextureManager::Instance()->draw("restart_button", restartRect.x, restartRect.y,
                                         buttonWidth, buttonHeight, buttonWidth, buttonHeight,
                                         renderer);
    }

    // Quit button
    if (TextureManager::Instance()->load("../assets/buttons/home.png", "home_button", renderer)) {
        SDL_Rect quitRect = {
            screenWidth / 2 + 60,
            screenHeight / 2 + 20,
            buttonWidth,
            buttonHeight
        };
        TextureManager::Instance()->draw("home_button", quitRect.x, quitRect.y,
                                         buttonWidth, buttonHeight, buttonWidth, buttonHeight,
                                         renderer);
    }
}

bool Game::loadAssets() {
    if (!TextureManager::Instance()->load("../assets/background/mainground.png", "mainground", renderer)) {
        SDL_Log("Failed to load mainground");
        return false;
    }
    // Load texture cho nhan vat
    if (!player->loadTexture("../assets/entities/player.png", "player", renderer)) {
        SDL_Log("Failed to load player");
        return false;
    }

    if (!TextureManager::Instance()->load("../assets/entities/enemy.png", "enemy", renderer)) {
        SDL_Log("Failed to load enemy texture");
        return false;
    }

    // Load texture cho bullet
    if (!TextureManager::Instance()->load("../assets/entities/bullet.png", "bullet", renderer)) {
        SDL_Log("Failed to load bullet texture");
        return false;
    }
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
                    int buttonWidth = 160;
                    int buttonHeight = 83;
                    SDL_Rect buttonRect = {
                        (screenWidth - buttonWidth) / 2,
                        (screenHeight - buttonHeight) / 2,
                        buttonWidth,
                        buttonHeight
                    };
                    if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
                        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
                        // Start the game
                        currentState = GameState::PLAYING;
                        resetGame();
                    }
                }
                break;
            default:
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

                    int buttonWidth = 80;
                    int buttonHeight = 83;

                    // Resume button
                    SDL_Rect resumeRect = {
                        screenWidth / 2 - 140,
                        screenHeight / 2 + 20,
                        buttonWidth,
                        buttonHeight
                    };

                    // Restart button
                    SDL_Rect restartRect = {
                        screenWidth / 2 - 40,
                        screenHeight / 2 + 20,
                        buttonWidth,
                        buttonHeight
                    };

                    // Quit button
                    SDL_Rect quitRect = {
                        screenWidth / 2 + 60,
                        screenHeight / 2 + 20,
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
                        resetGame();
                    }
                }
                break;
            default:
                break;
        }
    }
}

void Game::resetGame() {
    // Reset player position
    player->setPosition(screenWidth / 2.0f, screenHeight / 2.0f);

    // Clear enemies
    for (auto& enemy : enemies) {
        if (enemy != nullptr) {
            delete enemy;
        }
    }
    enemies.clear();

    // Reset score
    score = 0;

    // reset direction
    player->setTarget(player->getPosition().x, player->getPosition().y);

    // Reset timers
    timer.start();
    enemySpawnTimer.start();
}

void Game::renderCooldowns() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    if (player->canShoot()) {
        if (TextureManager::Instance()->load("../assets/entities/skillshoot.png", "skillshoot", renderer)) {
            TextureManager::Instance()->draw("skillshoot", screenWidth / 2 - 70, screenHeight - 100, 50, 50, 64, 64, renderer);
        }
    } else {
        if (TextureManager::Instance()->load("../assets/entities/skillshoot_cooldown.png", "skillshoot_cooldown", renderer)) {
            TextureManager::Instance()->draw("skillshoot_cooldown", screenWidth / 2 - 70, screenHeight - 100, 50, 50, 64, 64,
                                         renderer);
        }
    }
    if (player->canTeleport()) {
        if (TextureManager::Instance()->load("../assets/entities/flash.png", "flash", renderer)) {
            TextureManager::Instance()->draw("flash", screenWidth / 2 + 20, screenHeight - 100, 50, 50, 64, 64, renderer);
        }
    } else {
        if (TextureManager::Instance()->load("../assets/entities/flash_cooldown.png", "flash_cooldown", renderer)) {
            TextureManager::Instance()->draw("flash_cooldown", screenWidth / 2 + 20, screenHeight - 100, 50, 50, 64, 64,
                                         renderer);
        }
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
    bullet->fire(bulletStartPos, direction);
    Mix_PlayChannel(-1, Mix_LoadWAV("../assets/sounds/gunshot.wav"), 0);

}

void Game::teleportPlayer(float x, float y) {
    player->teleport(x, y);
    Mix_PlayChannel(-1, Mix_LoadWAV("../assets/sounds/flash.wav"), 0);

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
            handlePlayingEvents();
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

void Game::handlePlayingEvents() {
    // Update player
    player->update(timer.getDeltaTime());

    // Spawn enemy
    enemySpawnTimer.tick();
    if (enemySpawnTimer.getElapsedTime() >= nextEnemySpawn) {
        spawnEnemy();
        enemySpawnTimer.start();
        uniform_real_distribution<float> dist(1.0f, 3.0f);
        nextEnemySpawn = dist(rng);
    }

    // Update enemies
    for (auto &enemy: enemies) {
        if (!enemy->isDead()) {
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
        for (auto &enemy: enemies) {
            if (!enemy->isDead() && checkCollision(enemy->getRect(), bullet->getRect())) {
                bullet->setActive(false);
                enemy->die();
                score++;
                break;
            }
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 102, 102, 201, 0);
    SDL_RenderClear(renderer);

    switch (currentState) {
        case GameState::MAIN_MENU:
            renderMainMenu();
            renderMainMenuText();
            break;

        case GameState::PLAYING:
            renderPlaying();
            renderCooldowns();
            renderScore();
            break;

        case GameState::PAUSED:
            renderPauseMenu();
            renderPauseText();
            break;

        case GameState::GAME_OVER:
            renderGameOver();
            renderGameOverText();
            break;

        default:
            break;
    }

    SDL_RenderPresent(renderer);
}

void Game::renderPlaying() {
    TextureManager::Instance()->draw("mainground", 0, 0, screenWidth, screenHeight, 1600, 900,
                                                 renderer);
    player->render(renderer);
    for (auto &enemy: enemies) {
        if (!enemy->isDead()) {
            enemy->render(renderer);
        }
    }
    if (bullet != nullptr && bullet->isActive()) {
        bullet->render(renderer);
    }
}

void Game::spawnEnemy() {
    Enemy *enemy = nullptr;

    enemy = new Enemy(0, 0, 64, 64);
    enemy->loadTexture("../assets/entities/enemy.png", "enemy", renderer);
    enemies.push_back(enemy);

    // Randomly choose a side to spawn from
    std::uniform_int_distribution<int> sideDist(0, 3);
    int side = sideDist(rng);

    float x = 0, y = 0;

    // Determine spawn position based on side
    switch (side) {
        case 0: // Top
            x = std::uniform_real_distribution<float>(500.0f, static_cast<float>(screenWidth - 407))(rng);
            y = -50.0f;
            break;
        case 1: // Right
            x = screenWidth + 50.0f;
            y = std::uniform_real_distribution<float>(280.0f, static_cast<float>(screenHeight - 310))(rng);
            break;
        case 2: // Bottom
            x = std::uniform_real_distribution<float>(540.0f, static_cast<float>(screenWidth - 326))(rng);
            y = screenHeight + 50.0f;
            break;
        case 3: // Left
            x = -50.0f;
            y = std::uniform_real_distribution<float>(160.0f, static_cast<float>(screenHeight - 248))(rng);
            break;
        default:
            break;
    }

    enemy->setPosition(x, y);

}

bool Game::checkCollision(const SDL_Rect &a, const SDL_Rect &b) {
    return (a.x < b.x + b.w &&
            (a.x + a.w) > b.x &&
            a.y < b.y + b.h &&
            (a.y + a.h) > b.y);
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
    for (auto& enemy : enemies) {
        if (enemy != nullptr) {
            delete enemy;
            enemy = nullptr;
        }
    }
    enemies.clear();


    TextureManager::Instance()->clean();

    if (this->renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (this->window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    for (auto& pair : textTextureCache) {
        if (pair.second != nullptr) {
            SDL_DestroyTexture(pair.second);
        }
    }
    textTextureCache.clear();

    for (auto& pair : fontMap) {
        if (pair.second != nullptr && pair.second != font) {
            TTF_CloseFont(pair.second);
        }
    }
    fontMap.clear();
    SDL_Quit();
}