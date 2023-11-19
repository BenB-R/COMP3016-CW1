#ifndef GAME_H
#define GAME_H

#include "ScreenManager.h"
#include "Menu.h"

class Game {
public:
    Game();
    void initStoryScreen();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    ScreenManager* screenManager;
    Menu* menu;
    bool onMenuScreen;
    bool onStoryScreen;
    bool isRunning;
    bool gameEnded;
    SDL_Texture* storyTextTexture;
    TTF_Font* storyFont;
    SDL_Rect storyTextRect;
    std::vector<std::string> lines;
    SDL_Color textColor;
    Button returnToMenuButton;

    void init();
    void mainLoop();
    void render();
    void renderStoryScreen();
    void cleanUp();
};

#endif // GAME_H
