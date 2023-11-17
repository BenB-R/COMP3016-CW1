#include "ScreenManager.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include <iostream>
#include <SDL_ttf.h>

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

    // Initialize talk buttons for each location
    for (auto& screen : screens) {
        Button talkButton;
        talkButton.rect = { 300, 500, 200, 50 }; // Set the position and size
        talkButton.label = "Talk";
        talkButton.linkedScreen = &screen;
        talkButtons.push_back(talkButton);
    }

    // Initialize SDL_ttf and load a font
    TTF_Init();
    dialogueFont = TTF_OpenFont("Fonts/slkscr.ttf", 24);
    if (!dialogueFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    // Initialize the back button, positioned for example in the top-left corner
    backButton = { {10, 10, 50, 50}, locationSelectorScreen };

    // Set the initial screen to be the location selector
    currentScreen = locationSelectorScreen;

    characterManager.loadCharacterData("CharacterLocations.txt");

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
    TTF_CloseFont(dialogueFont);
    TTF_Quit();
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

        // Check if the current screen is the location selector
        if (currentScreen->isLocationSelector) {
            for (const auto& button : locationButtons) {
                if (SDL_PointInRect(&clickPoint, &button.rect)) {
                    changeScreen(button.linkedScreen);
                    return; // Exit the function after handling the event
                }
            }
        }
        else {
            // Handle the back button click
            if (SDL_PointInRect(&clickPoint, &backButton.rect)) {
                goToLocationSelector();
                timeManager.advanceTime();  // Advance time when the back button is clicked
                return; // Exit the function after handling the event
            }

            // Handle talk button clicks
            for (const auto& button : talkButtons) {
                if (SDL_PointInRect(&clickPoint, &button.rect)) {
                    std::string timeOfDay = timeManager.getCurrentTimeAsString();
                    currentCharacters = characterManager.getCharactersAt(timeOfDay, currentScreen->name);

                    // Debug output to console
                    std::cout << "Talk button clicked for screen: " << currentScreen->name << std::endl;
                    std::cout << "Characters at this location and time: " << std::endl;
                    for (const auto& character : currentCharacters) {
                        std::cout << "Name: " << character.name << ", Dialogue: " << character.dialogue << std::endl;
                    }
                    return; // Exit the function after handling the event
                }
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
        // Render the background
        SDL_RenderCopy(renderer, currentScreen->background, NULL, NULL);

        // Render location selector buttons
        if (currentScreen->isLocationSelector) {
            for (const auto& button : locationButtons) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Example: white color for button
                SDL_RenderFillRect(renderer, &button.rect);
                // Render button label here (if you have one)
            }
        }
        // Render talk buttons when not in location selector
        else {
            for (const auto& button : talkButtons) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Example: green color for talk button
                SDL_RenderFillRect(renderer, &button.rect);
                // Render talk button label here (if you have one)
            }

            // Render character dialogues if available
            if (!currentScreen->isLocationSelector) {
                if (!currentCharacters.empty()) {
                    for (const auto& character : currentCharacters) {
                        SDL_Color textColor = { 255, 255, 255, 255 }; // White color for text
                        SDL_Surface* surfaceMessage = TTF_RenderText_Blended(dialogueFont, character.dialogue.c_str(), textColor);
                        SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
                        SDL_Rect message_rect; // Set the position and size for your text

                        // You can use the character's screen position or set a fixed position for the dialogue
                        message_rect.x = 100;  // For example, let's just say it's at x = 100
                        message_rect.y = 550;  // and y = 550, adjust as needed
                        message_rect.w = surfaceMessage->w; // Use the surface's width
                        message_rect.h = surfaceMessage->h; // and height for the texture rect

                        SDL_RenderCopy(renderer, message, NULL, &message_rect);

                        // Clean up the surface and texture
                        SDL_FreeSurface(surfaceMessage);
                        SDL_DestroyTexture(message);
                    }
                }
            }
        }
        if (!currentScreen->isLocationSelector) {
            // This sets the draw color for the back button - make sure this color stands out
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Example: yellow color for back button
            SDL_RenderFillRect(renderer, &backButton.rect);

            // If you have a texture for the back button, use this instead:
            // SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButton.rect);

            // Render the talk buttons and character dialogues...
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
