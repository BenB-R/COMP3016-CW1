#ifndef GAME_H
#define GAME_H

#include "ScreenManager.h"

class Game {
public:
    Game();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    ScreenManager* screenManager;
    bool isRunning;

    void init();
    void mainLoop();
    void cleanUp();
};

#endif // GAME_H
