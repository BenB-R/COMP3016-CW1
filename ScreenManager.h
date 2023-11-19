#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <set>
#include <vector>
#include <string>
#include "TimeManager.h"
#include "CharacterManager.h"
#include "ClueManager.h"
#include "EndingManager.h"

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

struct ClueSlot {
    std::string location;
    std::string time;
    int clueIndex;
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
    SDL_Texture* winBackground{};
    SDL_Texture* loseBackground{};

    CharacterManager characterManager;
    EndingManager endingManager;
    bool gameEnded = false;
    bool playerWins;
    std::string endingMessage;

    std::set<int> discoveredCluesToday;
    int cluesCollected = 0;

    std::string currentClueText;
    int totalWeight{};
    int surfaceCount;
    int textureCount;


    void handleEvent(const SDL_Event& event);
    void handleTalkButton(const Button& button);
    void handleClueButton(const Button& button);
    void update();
    void render();
    //void renderStoryScreen(SDL_Renderer* renderer, TTF_Font* font);
    void initializeClueSlots();
    std::string getClueText(const std::string& locationName, const std::string& timeOfDay);

    void resetForNewDay();  // Call this method when a new day starts

    // Navigates to the location selector screen
    void goToLocationSelector();

    // Navigates to a specific location screen
    void goToLocation(Screen* locationScreen);

    void loadTimeTextures();
    void loadEndingBackgrounds();

    int getClueIndex(const std::string& locationName, const std::string& timeOfDay);

private:
    SDL_Renderer* renderer;
    std::vector<Screen> screens; // Now a member to store screens
    std::vector<Button> locationButtons; // Buttons for selecting locations
    std::vector<Button> talkButtons; // Add a list of talk buttons
    std::vector<Button> clueButtons; // Clue buttons
    Button resetButton;
    std::vector<CharacterInfo> currentCharacters;
    Button backButton; // A single back button to return to the location selector
    Screen* currentScreen;
    Screen* locationSelectorScreen; // Pointer to the location selector screen
    TTF_Font* dialogueFont;
    std::vector<ClueSlot> clueSlots;
    ClueManager clueManager;
    bool clueFound = false;

    void changeScreen(Screen* newScreen);
};

#endif
