//
// Created by Admin on 3/16/2025.
//

#include "Timer.h"
#include <SDL2/SDL.h>
#include <bits/stdc++.h>
using namespace std;


Timer::Timer() : startTime(0), pausedTime(0), lastTime(0), currentTime(0), deltaTime(0), paused(false), started(false) {}

void Timer::start() {
    started = true;
    paused = false;
    startTime = SDL_GetTicks() / 1000.0f;
    lastTime = startTime;
}

void Timer::stop() {
    started = false;
    paused = false;
    startTime = 0;
    pausedTime = 0;
    deltaTime = 0;
}

void Timer::pause() {
    if (started == true && paused == false) {
        paused = true;
        pausedTime = SDL_GetTicks() / 1000.0f - startTime;
    }
}

void Timer::unpause() {
    if (started == true && paused == true) {
        paused = false;
        startTime = SDL_GetTicks() / 1000.0f - pausedTime;
        pausedTime = 0;
    }
}

float Timer::getElapsedTime() const {
    if (started == true) {
        if (paused == true) {
            return pausedTime;
        } else {
            return SDL_GetTicks() / 1000.0f - startTime;
        }
    }
    return 0;
}

float Timer::getDeltaTime() const {
    return deltaTime;
}

void Timer::tick() {
    if (started == true && paused == false) {
        currentTime = SDL_GetTicks() / 1000.0f;
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }
}

bool Timer::isStarted() const {
    return started;
}

bool Timer::isPaused() const {
    return paused && started;
}