#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "TimeManager.h"
#include "CharacterManager.h"

struct Screen {
    std::string name;
    SDL_Texture* background;
    bool isLocationSelector;
    // Other properties and methods as needed
};

struct Button {
    SDL_Rect rect;
    Screen* linkedScreen;
    std::string label;
    // Other button properties, such as a texture for the button, if desired
};

class ScreenManager {
public:
    ScreenManager(SDL_Renderer* renderer);
    ~ScreenManager();

    TimeManager timeManager;

    SDL_Texture* morningTexture{};
    SDL_Texture* noonTexture{};
    SDL_Texture* afternoonTexture{};
    SDL_Texture* eveningTexture{};
    SDL_Texture* nightTexture{};
    CharacterManager characterManager;

    void handleEvent(const SDL_Event& event);
    void update();
    void render() const;

    // Navigates to the location selector screen
    void goToLocationSelector();

    // Navigates to a specific location screen
    void goToLocation(Screen* locationScreen);

    void loadTimeTextures();

private:
    SDL_Renderer* renderer;
    std::vector<Screen> screens; // Now a member to store screens
    std::vector<Button> locationButtons; // Buttons for selecting locations
    std::vector<Button> talkButtons; // Add a list of talk buttons
    std::vector<CharacterInfo> currentCharacters;
    Button backButton; // A single back button to return to the location selector
    Screen* currentScreen;
    Screen* locationSelectorScreen; // Pointer to the location selector screen
    TTF_Font* dialogueFont;

    void changeScreen(Screen* newScreen);
};

#endif
