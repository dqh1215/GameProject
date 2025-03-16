//
// Created by Admin on 3/16/2025.
//

#ifndef TIMER_H
#define TIMER_H



class Timer {
public:
    Timer();
    void start();
    void stop();
    void pause();
    void unpause();

    float getElapsedTime() const;
    float getDeltaTime() const;
    void tick();

    bool isStarted() const;
    bool isPaused() const;

private:
    float startTime;
    float pausedTime;
    float lastTime;
    float currentTime;
    float deltaTime;

    bool paused;
    bool started;
};



#endif //TIMER_H
