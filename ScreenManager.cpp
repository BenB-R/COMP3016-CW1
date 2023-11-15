#include "ScreenManager.h"

ScreenManager::ScreenManager(SDL_Renderer* rend) : renderer(rend), currentScreen(nullptr) {
    // Create screens for Cafe, Pond, Police Station
    Screen cafeScreen;
    cafeScreen.name = "Cafe";
    cafeScreen.background = LoadTexture("path/to/cafe_image.png", renderer);

    Screen pondScreen;
    pondScreen.name = "Pond";
    pondScreen.background = LoadTexture("path/to/pond_image.png", renderer);

    Screen policeStationScreen;
    policeStationScreen.name = "Police Station";
    policeStationScreen.background = LoadTexture("path/to/police_station_image.png", renderer);

    // Store screens in a container if needed
    std::vector<Screen> screens = { cafeScreen, pondScreen, policeStationScreen };

    // Buttons for the location selector screen
    Button cafeButton;
    cafeButton.rect = { 100, 100, 200, 50 }; // Example position and size
    cafeButton.linkedScreen = &screens[0]; // Link to Cafe screen

    Button pondButton;
    pondButton.rect = { 100, 200, 200, 50 }; // Another position and size
    pondButton.linkedScreen = &screens[1]; // Link to Pond screen

    Button policeStationButton;
    policeStationButton.rect = { 100, 300, 200, 50 };
    policeStationButton.linkedScreen = &screens[2]; // Link to Police Station screen

    // Add buttons to the buttons vector
    buttons.push_back(cafeButton);
    buttons.push_back(pondButton);
    buttons.push_back(policeStationButton);

    // currentScreen = &locationSelectorScreen;
}


ScreenManager::~ScreenManager() {
    // Clean up resources
}

void ScreenManager::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        for (const auto& button : buttons) {
            if (SDL_PointInRect(new SDL_Point{ event.button.x, event.button.y }, &button.rect)) {
                changeScreen(button.linkedScreen);
                break;
            }
        }
    }
}

void ScreenManager::update() {
    // Update logic for the current screen
}

void ScreenManager::render() {
    SDL_RenderClear(renderer);
    if (currentScreen) {
        // Render the background of the current screen
        SDL_RenderCopy(renderer, currentScreen->background, NULL, NULL);
    }
    // Render buttons for the location selector screen
    for (const auto& button : buttons) {
        // Render button.rect, etc.
    }
    SDL_RenderPresent(renderer);
}


void ScreenManager::changeScreen(Screen* newScreen) {
    currentScreen = newScreen;
}
