#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();
    void handleEvent(const SDL_Event& event);
    void render();
    bool shouldStartGame() const;

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture; // For the menu background image
    SDL_Rect startButtonRect;       // Rectangle for the start button
    TTF_Font* font;                 // Font for rendering text
    bool startGame;
};
