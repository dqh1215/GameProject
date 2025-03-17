//
// Created by Admin on 3/16/2025.
//

#include "Game.h"
#include <bits/stdc++.h>
using namespace std;

Game::Game() : window(nullptr), renderer(nullptr), player(nullptr), running(false),
               screenWidth(0), screenHeight(0) {}

Game::~Game() {
    clean();
}

bool Game::init(const std::string& title, int width, int height, bool fullscreen) {
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


    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);


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

    this->running = true;
    return this->running;
}


bool Game::loadAssets() {
    // Load texture cho nhan vat
    if (!player->loadTexture("../assets/entities/player.png", "player", renderer)) {
        SDL_Log("Failed to load player");
        return false;
    }
    SDL_Log("Loaded player successful");
    return true;
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
    this->inputHandler.handleEvents(this->running, *this->player);
}

void Game::update() {
    player->update(timer.getDeltaTime());
}

void Game::render() {
    // Xóa màn hình với màu đen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // render nhan vat
    player->render(renderer);

    // hien thi hinh anh
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    if (this->player) {
        delete player;
        player = nullptr;
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