#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <SDL.h>
#include <vector>
#include <string>

struct Screen {
    std::string name;
    SDL_Texture* background;  // A texture for the background image
};

struct Button {
    SDL_Rect rect;  // Button dimensions
    Screen* linkedScreen;  // Pointer to the linked screen
};

class ScreenManager {
public:
    ScreenManager(SDL_Renderer* renderer);
    ~ScreenManager();

    void handleEvent(const SDL_Event& event);
    void update();
    void render();

private:
    SDL_Renderer* renderer;
    std::vector<Button> buttons;
    Screen* currentScreen;

    void changeScreen(Screen* newScreen);
    // Additional private methods and member variables
};

#endif // SCREENMANAGER_H
