#include "ScreenManager.h"
#include "TextureManager.h"
#include "TimeManager.h"

ScreenManager::ScreenManager(SDL_Renderer* rend) : renderer(rend), currentScreen(nullptr) {
    // Initialize the location selector screen
    locationSelectorScreen = new Screen{ "Location Selector", TextureManager::LoadTexture("Assets/Backgrounds/selector_background.png", renderer), true };

    // Initialize the location screens
    screens.push_back({ "Cafe", TextureManager::LoadTexture("Assets/Backgrounds/cafe_image.png", renderer), false });
    screens.push_back({ "Pond", TextureManager::LoadTexture("Assets/Backgrounds/pond_image.png", renderer), false });
    screens.push_back({ "Police Station", TextureManager::LoadTexture("Assets/Backgrounds/police_station_image.png", renderer), false });

    // Initialize buttons for the location selector screen
    locationButtons.push_back({ {100, 100, 200, 50}, &screens[0] });
    locationButtons.push_back({ {100, 200, 200, 50}, &screens[1] });
    locationButtons.push_back({ {100, 300, 200, 50}, &screens[2] });

    // Initialize the back button, positioned for example in the top-left corner
    backButton = { {10, 10, 50, 50}, locationSelectorScreen };

    // Set the initial screen to be the location selector
    currentScreen = locationSelectorScreen;

    loadTimeTextures();
}





ScreenManager::~ScreenManager() {
    for (auto& screen : screens) {
        if (screen.background) {
            SDL_DestroyTexture(screen.background);
            screen.background = nullptr;
        }
    }
    delete locationSelectorScreen;

    SDL_DestroyTexture(morningTexture);
    SDL_DestroyTexture(noonTexture);
    SDL_DestroyTexture(afternoonTexture);
    SDL_DestroyTexture(eveningTexture);
    SDL_DestroyTexture(nightTexture);
}

void ScreenManager::loadTimeTextures() {
    morningTexture = TextureManager::LoadTexture("Assets/Times/morning.png", renderer);
    noonTexture = TextureManager::LoadTexture("Assets/Times/noon.png", renderer);
    afternoonTexture = TextureManager::LoadTexture("Assets/Times/afternoon.png", renderer);
    eveningTexture = TextureManager::LoadTexture("Assets/Times/evening.png", renderer);
    nightTexture = TextureManager::LoadTexture("Assets/Times/night.png", renderer);
}

void ScreenManager::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point clickPoint = { event.button.x, event.button.y };
        if (currentScreen->isLocationSelector) {
            for (const auto& button : locationButtons) {
                if (SDL_PointInRect(&clickPoint, &button.rect)) {
                    changeScreen(button.linkedScreen);
                    break;
                }
            }
        }
        else {
            if (SDL_PointInRect(&clickPoint, &backButton.rect)) {
                goToLocationSelector();
                timeManager.advanceTime();  // Advance time when the back button is clicked
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
        // Render the buttons appropriate for the current screen
        if (currentScreen->isLocationSelector) {
            for (const auto& button : locationButtons) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Example: white color for button
                SDL_RenderFillRect(renderer, &button.rect);
            }
        }
        else {
            // Render the back button when not on the location selector
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Example: white color for button
            SDL_RenderFillRect(renderer, &backButton.rect);
        }
    }
    
    // Define the size of the time indicator images
    int timeTextureWidth = 100;
    int timeTextureHeight = 50;

    // Position the time indicator in the top right corner for a 720p window
    SDL_Rect timeRect = { 1280 - timeTextureWidth - 10, 10, timeTextureWidth, timeTextureHeight }; // Adjust as needed
    SDL_Texture* timeTexture = nullptr;

    switch (timeManager.getCurrentTime()) {
    case TimeManager::MORNING:
        timeTexture = morningTexture;
        break;
    case TimeManager::NOON:
        timeTexture = noonTexture;
        break;
    case TimeManager::AFTERNOON:
        timeTexture = afternoonTexture;
        break;
    case TimeManager::EVENING:
        timeTexture = eveningTexture;
        break;
    case TimeManager::NIGHT:
        timeTexture = nightTexture;
        break;
    }

    if (timeTexture) {
        SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);
    }


    SDL_RenderPresent(renderer);
}




void ScreenManager::changeScreen(Screen* newScreen) {
    currentScreen = newScreen;
}

void ScreenManager::goToLocationSelector() {
    currentScreen = locationSelectorScreen;
}

void ScreenManager::goToLocation(Screen* locationScreen) {
    currentScreen = locationScreen;
}
