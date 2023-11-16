#include "ScreenManager.h"
#include "TextureManager.h"

ScreenManager::ScreenManager(SDL_Renderer* rend) : renderer(rend), currentScreen(nullptr) {
    // Initialize screens in the member vector
    screens.push_back({ "Cafe", TextureManager::LoadTexture("Assets/Backgrounds/cafe_image.png", renderer) });
    screens.push_back({ "Pond", TextureManager::LoadTexture("Assets/Backgrounds/pond_image.png", renderer) });
    screens.push_back({ "Police Station", TextureManager::LoadTexture("Assets/Backgrounds/police_station_image.png", renderer) });

    // Initialize buttons and link them to the corresponding screens
    buttons.push_back({ {100, 100, 200, 50}, &screens[0] });
    buttons.push_back({ {100, 200, 200, 50}, &screens[1] });
    buttons.push_back({ {100, 300, 200, 50}, &screens[2] });

    // Set the initial screen
    currentScreen = &screens[0]; // For example, start with the Cafe screen
}



ScreenManager::~ScreenManager() {
    for (auto& screen : screens) {
        if (screen.background) {
            SDL_DestroyTexture(screen.background);
            screen.background = nullptr;
        }
    }
    // Add any additional clean up for buttons if necessary
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
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Set the draw color to red for the buttons
    for (const auto& button : buttons) {
        SDL_RenderFillRect(renderer, &button.rect);  // Fill a rectangle for the button
    }
    SDL_RenderPresent(renderer);
}



void ScreenManager::changeScreen(Screen* newScreen) {
    currentScreen = newScreen;
}
