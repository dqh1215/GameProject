//
// Created by Admin on 3/16/2025.
//

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>
using namespace std;

class TextureManager {
public:
    static TextureManager* Instance() {
        if (instance == nullptr) {
            instance = new TextureManager();
        }
        return instance;
    }

    bool init();
    bool load(const string& fileName, const string& id, SDL_Renderer* renderer);
    void draw(const string &id, int x, int y, int displayWidth, int displayHeight, int frameWidth, int frameHeight,
                          SDL_Renderer *renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawFrame(const string& id, int x, int y, int displayWidth, int displayHeight,
               int currentRow, int currentFrame, int frameWidth, int frameHeight,
               SDL_Renderer* renderer, SDL_RendererFlip flip = SDL_FLIP_NONE,
               double angle = 0.0, SDL_Point* center = nullptr);
    void clearFromTextureMap(const string& id);
    void clean();
    SDL_Texture* getTexture(const string& id) {
        return textureMap[id];
    }
private:
    TextureManager() {}
    ~TextureManager() {}

    static TextureManager* instance;
    map<std::string, SDL_Texture*> textureMap;
};
#endif //TEXTUREMANAGER_H
