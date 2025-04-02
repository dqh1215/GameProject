//
// Created by Admin on 3/16/2025.
//

#include "TextureManager.h"
#include <SDL2/SDL.h>
#include <bits/stdc++.h>
#include <SDL_image.h>
using namespace std;
TextureManager *TextureManager::instance = nullptr;


bool TextureManager::init() {
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Log("Failed to initialize SDL_Image");
        return false;
    }
    SDL_Log("Successfully initialized SDL_Image");
    return true;
}

bool TextureManager::load(const string &fileName, const string &id, SDL_Renderer *renderer) {
    if (textureMap.contains(id)) {
        return true;
    }
    SDL_Surface *tempSurface = IMG_Load(fileName.c_str());

    if (!tempSurface) {
        SDL_Log("Failed to load image: %s", fileName.c_str());
        SDL_Log("SDL_Image Error", SDL_GetError());
        return false;
    } else {
        SDL_Log("Successfully loaded image: %s", fileName.c_str());
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!texture) {
        SDL_Log("Failed to create texture: %s", fileName.c_str());
        SDL_Log("SDL Error", SDL_GetError());
        return false;
    }

    textureMap[id] = texture;
    return true;
}

void TextureManager::draw(const string &id, int x, int y, int displayWidth, int displayHeight,
                          int frameWidth, int frameHeight,
                          SDL_Renderer *renderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = 0;
    srcRect.y = 0;
    destRect.w = displayWidth;
    destRect.h = displayHeight;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;
    destRect.x = x;
    destRect.y = y;

    SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &destRect, 0, nullptr, flip);
}

void TextureManager::draw(const string &id, int x, int y, int displayWidth, int displayHeight,
                               int currentRow, int currentColumn, int frameWidth, int frameHeight,
                               SDL_Renderer *renderer, SDL_RendererFlip flip,
                               double angle, SDL_Point *center) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = frameWidth * currentColumn;
    srcRect.y = frameHeight * currentRow;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;

    destRect.x = x;
    destRect.y = y;
    destRect.w = displayWidth;
    destRect.h = displayHeight;

    SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &destRect, angle, center, flip);
}

void TextureManager::draw(const string &id, int x, int y, int displayWidth, int displayHeight, int currentRow, int currentColumn, int frameWidth, int frameHeight, SDL_Renderer *renderer, int duration, SDL_RendererFlip flip, double angle, SDL_Point *center) {
    if (SDL_GetTicks() > duration) {
        return;
    }
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = frameWidth * currentColumn;
    srcRect.y = frameHeight * currentRow;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;

    destRect.x = x;
    destRect.y = y;
    destRect.w = displayWidth;
    destRect.h = displayHeight;

    SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &destRect, angle, center, flip);
}

void TextureManager::clearFromTextureMap(const string &id) {
    if (textureMap.contains(id)) {
        SDL_DestroyTexture(textureMap[id]);
        textureMap.erase(id);
    }
}

void TextureManager::clean() {
    for (auto &texture: textureMap) {
        SDL_DestroyTexture(texture.second);
    }
    textureMap.clear();
    IMG_Quit();
}
