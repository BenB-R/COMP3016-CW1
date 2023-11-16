#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <SDL.h>
#include <vector>
#include <string>

struct Screen {
    std::string name;
    SDL_Texture* background;  // A texture for the background image
    // You might want to add a destructor to deallocate the texture when a Screen is destroyed
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
    std::vector<Screen> screens; // Stores Screen objects
    std::vector<Button> buttons;
    Screen* currentScreen; // This could be a pointer to an element in the screens vector

    void changeScreen(Screen* newScreen);
    // Additional private methods and member variables
};

#endif // SCREENMANAGER_H
