#include "ScreenManager.h"
#include "TextureManager.h"
#include <iostream>
#include <random>
#include <SDL_ttf.h>
#include "ClueManager.h"

ScreenManager::ScreenManager(SDL_Renderer* rend) : renderer(rend), currentScreen(nullptr), endingManager(clueManager) {
    // Initialize the location selector screen
    locationSelectorScreen = new Screen{ "Location Selector", TextureManager::LoadTexture("Assets/Backgrounds/selector_background.png", renderer), true };

    // Initialize the location screens
    screens.push_back({ "Cafe", TextureManager::LoadTexture("Assets/Backgrounds/cafe.png", renderer), false });
    screens.push_back({ "Pond", TextureManager::LoadTexture("Assets/Backgrounds/pond.png", renderer), false });
    screens.push_back({ "Police Station", TextureManager::LoadTexture("Assets/Backgrounds/police_station.png", renderer), false });
    screens.push_back({ "Gym", TextureManager::LoadTexture("Assets/Backgrounds/gym.png", renderer), false });
    screens.push_back({ "Shop", TextureManager::LoadTexture("Assets/Backgrounds/shop.png", renderer), false });
    screens.push_back({ "Alex's House", TextureManager::LoadTexture("Assets/Backgrounds/mansion.png", renderer), false });
    screens.push_back({ "Library", TextureManager::LoadTexture("Assets/Backgrounds/library.png", renderer), false });

    // Initialize buttons for the location selector screen
    locationButtons.push_back({ {100, 150, 200, 50}, &screens[0] });
    locationButtons.push_back({ {100, 300, 200, 50}, &screens[1] });
    locationButtons.push_back({ {100, 450, 200, 50}, &screens[2] });
    locationButtons.push_back({ {1000, 150, 200, 50}, &screens[3] });
    locationButtons.push_back({ {1000, 300, 200, 50}, &screens[4] });
    locationButtons.push_back({ {1000, 450, 200, 50}, &screens[5] });
    locationButtons.push_back({ {550, 600, 200, 50}, &screens[6] });
    resetButton = { {40, 650, 220, 50}, nullptr }; // 'nullptr' because it's not linked to a screen

    // Initialize talk buttons for each location
    for (auto& screen : screens) {
        Button talkButton;
        talkButton.rect = { 200, 200, 200, 50 }; // Set the position and size
        talkButton.label = "Talk";
        talkButton.linkedScreen = &screen;
        talkButtons.push_back(talkButton);
    }

    // Initialize clue buttons for each location
    for (auto& screen : screens) {
        Button clueButton;
        clueButton.rect = { 200, 350, 200, 50 }; // Adjust position and size as needed
        clueButton.label = "Search Clue";
        clueButton.linkedScreen = &screen;
        clueButtons.push_back(clueButton);
    }

    std::cout << "Screens and buttons initialized." << std::endl;
    for (const auto& screen : screens) {
        std::cout << "Screen initialized: " << screen.name << std::endl;
    }
    for (const auto& button : talkButtons) {
        std::cout << "Talk button initialized for screen: "
            << (button.linkedScreen ? button.linkedScreen->name : "null") << std::endl;
    }

    // Initialize SDL_ttf and load a font
    TTF_Init();
    dialogueFont = TTF_OpenFont("Fonts/slkscr.ttf", 20);
    if (!dialogueFont) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    // Initialize the back button, positioned for example in the top-left corner
    backButton = { {10, 10, 50, 50}, locationSelectorScreen };

    // Set the initial screen to be the location selector
    currentScreen = locationSelectorScreen;

    characterManager.loadCharacterData("CharacterLocations.txt");

    initializeClueSlots();

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
        std::cout << "Mouse button down at (" << clickPoint.x << ", " << clickPoint.y << ")" << std::endl;

        // Check if the current screen is the location selector
        if (currentScreen->isLocationSelector) {
            std::cout << "Current screen is the location selector." << std::endl;
            for (const auto& button : locationButtons) {
                if (SDL_PointInRect(&clickPoint, &button.rect)) {
                    std::cout << "Location button clicked. Changing screen to: " << button.linkedScreen->name << std::endl;
                    changeScreen(button.linkedScreen);
                    return;  // Exit after handling the event
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point clickPoint = { event.button.x, event.button.y };

                // Handle the reset button click
                if (SDL_PointInRect(&clickPoint, &resetButton.rect)) {
                    std::cout << "Reset button clicked. Resetting to morning." << std::endl;
                    timeManager.setDayTime(TimeManager::MORNING); // This function should exist in your TimeManager class
                    resetForNewDay(); // Assuming you have this function to reset variables for a new day
                    return; // Exit after handling the event
                }
            }
        }
        else {
            std::cout << "Current screen is not the location selector." << std::endl;

            // Handle the back button click
            if (SDL_PointInRect(&clickPoint, &backButton.rect)) {
                std::cout << "Back button clicked. Returning to location selector." << std::endl;
                goToLocationSelector();
                timeManager.advanceTime();
                return;  // Exit after handling the event
            }

            // Handle talk button clicks
            for (const auto& button : talkButtons) {
                if (SDL_PointInRect(&clickPoint, &button.rect)) {
                    handleTalkButton(button);
                    return;  // Exit after handling the event
                }
            }

            // Handle clue button clicks
            for (const auto& button : clueButtons) {
                if (SDL_PointInRect(&clickPoint, &button.rect)) {
                    handleClueButton(button);
                    clueFound = true; // Set the flag when a clue button is clicked
                    return; // Exit after handling the event
                }
            }

        }
    }
}

void ScreenManager::handleTalkButton(const Button& button) {
    std::string timeOfDay = timeManager.getCurrentTimeAsString();
    currentCharacters = characterManager.getCharactersAt(timeOfDay, currentScreen->name);

    std::cout << "Talk button clicked for screen: " << currentScreen->name << std::endl;
    std::cout << "Characters at this location and time: " << std::endl;
    if (currentCharacters.empty()) {
        std::cout << "No characters found." << std::endl;
    }
    for (const auto& character : currentCharacters) {
        std::cout << "Name: " << character.name << ", Dialogue: " << character.dialogue << std::endl;
    }
}

void ScreenManager::handleClueButton(const Button& button) {
    std::string timeOfDay = timeManager.getCurrentTimeAsString();
    int clueIndex = getClueIndex(currentScreen->name, timeOfDay);

    int clueWeight = 0; // Variable to store the weight of the clue
    currentClueText = clueManager.getDynamicClue(clueIndex, clueWeight); // Get the clue text and weight

    if (clueIndex != -1 && discoveredCluesToday.find(clueIndex) == discoveredCluesToday.end()) {
        // Clue is being discovered for the first time today
        cluesCollected++;
        discoveredCluesToday.insert(clueIndex);

        totalWeight += clueWeight; // Add the weight of the newly discovered clue
        clueFound = true; // Now we only set this when the clue button is pressed

        std::cout << "You found this clue: " << currentClueText << std::endl;
        std::cout << "Total Weight: " << totalWeight << std::endl;
    }
}

void ScreenManager::update() {
    if (!gameEnded && currentScreen->name == clueManager.getCurrentScenario().location && timeManager.getCurrentTimeAsString() == "Night") {
        gameEnded = true;
        bool playerWins = totalWeight >= 10;
        endingMessage = endingManager.getEndingMessage(playerWins);
    }
}

void ScreenManager::render() {
    SDL_RenderClear(renderer);

    if (gameEnded) {
        // Render the ending message
        SDL_Surface* surfaceMessage = TTF_RenderText_Blended(dialogueFont, endingMessage.c_str(), { 255, 255, 255, 255 });
        SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_Rect messageRect = { (1280 - surfaceMessage->w) / 2, (720 - surfaceMessage->h) / 2, surfaceMessage->w, surfaceMessage->h };

        SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(messageTexture);
    }
    else
    {
        if (currentScreen) {
            // Render the background
            SDL_RenderCopy(renderer, currentScreen->background, NULL, NULL);

            // Render location selector buttons
            if (currentScreen->isLocationSelector) {
                for (const auto& button : locationButtons) {
                    // Set button color to black
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &button.rect);

                    // Render button label in white
                    SDL_Surface* surfaceLabel = TTF_RenderText_Blended(dialogueFont, button.linkedScreen->name.c_str(), { 255, 255, 255, 255 });
                    SDL_Texture* labelTexture = SDL_CreateTextureFromSurface(renderer, surfaceLabel);
                    SDL_Rect labelRect = {
                        button.rect.x + (button.rect.w - surfaceLabel->w) / 2,
                        button.rect.y + (button.rect.h - surfaceLabel->h) / 2,
                        surfaceLabel->w,
                        surfaceLabel->h
                    };
                    SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect);

                    // Clean up
                    SDL_FreeSurface(surfaceLabel);
                    SDL_DestroyTexture(labelTexture);
                }
                // Render clue count text
                std::string clueCountText = "Clues Collected: " + std::to_string(cluesCollected);
                SDL_Surface* surfaceClueCount = TTF_RenderText_Blended(dialogueFont, clueCountText.c_str(), { 255, 255, 255, 255 });
                SDL_Texture* clueCountTexture = SDL_CreateTextureFromSurface(renderer, surfaceClueCount);
                SDL_Rect clueCountRect = { 1280 - surfaceClueCount->w - 30, 720 - surfaceClueCount->h - 30, surfaceClueCount->w, surfaceClueCount->h };
                SDL_RenderCopy(renderer, clueCountTexture, NULL, &clueCountRect);

                SDL_RenderCopy(renderer, clueCountTexture, NULL, &clueCountRect);

                if (timeManager.getCurrentTimeAsString() == "Night") {
                    std::string warningMessage = "Be careful when going out at night,\n"
                        "there is a murderer on the loose.\n"
                        "Stay alert and stay safe.";
                    SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(dialogueFont, warningMessage.c_str(), { 255, 255, 255, 255 }, 1200); // width slightly less than screen width
                    SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

                    // Centering the message
                    int messageWidth = surfaceMessage->w;
                    int messageHeight = surfaceMessage->h;
                    SDL_Rect messageRect = { (2030 - messageWidth) / 2, (720 - messageHeight) / 2, messageWidth, messageHeight };

                    // Calculate and render the black background box with padding
                    int padding = 10;
                    SDL_Rect backgroundRect = { messageRect.x - padding, messageRect.y - padding, messageRect.w -720, messageRect.h + padding * 2 };
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color
                    SDL_RenderFillRect(renderer, &backgroundRect);

                    // Render the text over the background box
                    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);

                    // Clean up
                    SDL_FreeSurface(surfaceMessage);
                    SDL_DestroyTexture(messageTexture);


                    
                }
                // Define the reset button properties
                SDL_Rect resetButtonRect = { 40, 650, 220, 50 }; // x, y, width, height
                std::string resetButtonText = "Sleep to Morning";

                // Render the reset button
                SDL_SetRenderDrawColor(renderer, 87, 17, 12, 255); // Red color for reset button
                SDL_RenderFillRect(renderer, &resetButtonRect);

                // Render the reset button text
                SDL_Surface* resetButtonSurface = TTF_RenderText_Blended(dialogueFont, resetButtonText.c_str(), { 255, 255, 255, 255 });
                SDL_Texture* resetButtonTexture = SDL_CreateTextureFromSurface(renderer, resetButtonSurface);
                SDL_Rect resetButtonTextRect = {
                    resetButtonRect.x + (resetButtonRect.w - resetButtonSurface->w) / 2,
                    resetButtonRect.y + (resetButtonRect.h - resetButtonSurface->h) / 2,
                    resetButtonSurface->w,
                    resetButtonSurface->h
                };
                SDL_RenderCopy(renderer, resetButtonTexture, NULL, &resetButtonTextRect);
            }
            else {
                // Render talk buttons and clue buttons when not in location selector
                for (const auto& button : talkButtons) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Example: green color for talk button
                    SDL_RenderFillRect(renderer, &button.rect);
                    // Render talk button label here (if you have one)
                }

                for (const auto& button : clueButtons) {
                    SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); // Example: orange color for clue button
                    SDL_RenderFillRect(renderer, &button.rect);
                    // Render clue button label here (if you have one)
                }

                // Render character dialogues if available
                if (!currentCharacters.empty()) {
                    for (const auto& character : currentCharacters) {
                        // Render the character's name with a black background box
                        SDL_Surface* surfaceName = TTF_RenderText_Blended(dialogueFont, character.name.c_str(), { 255, 255, 255, 255 }); // White color for text
                        SDL_Texture* nameTexture = SDL_CreateTextureFromSurface(renderer, surfaceName);
                        SDL_Rect name_rect;
                        name_rect.x = 100;
                        name_rect.y = 260;
                        name_rect.w = surfaceName->w;
                        name_rect.h = surfaceName->h;

                        // Draw a black background box for the name
                        SDL_Rect name_background_rect = name_rect;
                        name_background_rect.w += 20;  // Padding
                        name_background_rect.h += 20;
                        name_background_rect.x -= 10;
                        name_background_rect.y -= 10;
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color
                        SDL_RenderFillRect(renderer, &name_background_rect);

                        SDL_RenderCopy(renderer, nameTexture, NULL, &name_rect);

                        // Render the character's dialogue
                        SDL_Color textColor = { 255, 255, 255, 255 };
                        SDL_Surface* surfaceMessage = TTF_RenderText_Blended(dialogueFont, character.dialogue.c_str(), textColor);
                        SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
                        SDL_Rect message_rect;
                        message_rect.x = 100;
                        message_rect.y = 300;  // Below the character's name
                        message_rect.w = surfaceMessage->w;
                        message_rect.h = surfaceMessage->h;

                        // Draw a black background box for the text
                        SDL_Rect background_rect = message_rect;
                        background_rect.w += 20;
                        background_rect.h += 20;
                        background_rect.x -= 10;
                        background_rect.y -= 10;
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color
                        SDL_RenderFillRect(renderer, &background_rect);

                        SDL_RenderCopy(renderer, message, NULL, &message_rect);

                        // Clean up
                        SDL_FreeSurface(surfaceMessage);
                        SDL_DestroyTexture(message);
                        SDL_FreeSurface(surfaceName);
                        SDL_DestroyTexture(nameTexture);
                    }
                }

                // Define a string variable for clue text
                std::string timeOfDay = timeManager.getCurrentTimeAsString();

                if (clueFound)
                {
                    std::string clueText = getClueText(currentScreen->name, timeOfDay);

                    // Render clue text if available
                    if (!clueText.empty()) {
                        // Render currentClueText with a black background box
                        SDL_Surface* surfaceClue = TTF_RenderText_Blended(dialogueFont, currentClueText.c_str(), { 255, 255, 255, 255 });
                        SDL_Texture* clueTexture = SDL_CreateTextureFromSurface(renderer, surfaceClue);
                        SDL_Rect clue_rect = { 100, 550, surfaceClue->w, surfaceClue->h };

                        // Draw a black background box for the clue text
                        SDL_Rect background_rect = { clue_rect.x - 10, clue_rect.y - 10, clue_rect.w + 20, clue_rect.h + 20 };
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color
                        SDL_RenderFillRect(renderer, &background_rect);
                        SDL_RenderCopy(renderer, clueTexture, NULL, &clue_rect);

                        // Clean up
                        SDL_FreeSurface(surfaceClue);
                        SDL_DestroyTexture(clueTexture);
                    }
                }


                // Render the back button
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Example: yellow color for back button
                SDL_RenderFillRect(renderer, &backButton.rect);

            }
            // Render backButtonTexture if you have one

            // Render the time indicator
            int timeTextureWidth = 100;
            int timeTextureHeight = 50;
            SDL_Rect timeRect = { 1280 - timeTextureWidth - 10, 10, timeTextureWidth, timeTextureHeight };
            SDL_Texture* timeTexture = nullptr;

            switch (timeManager.getCurrentTime()) {
            case TimeManager::MORNING: timeTexture = morningTexture; break;
            case TimeManager::NOON: timeTexture = noonTexture; break;
            case TimeManager::AFTERNOON: timeTexture = afternoonTexture; break;
            case TimeManager::EVENING: timeTexture = eveningTexture; break;
            case TimeManager::NIGHT: timeTexture = nightTexture; break;
            }

            if (timeTexture) {
                SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);
            }

            SDL_RenderPresent(renderer);
        }
    }
}


std::string ScreenManager::getClueText(const std::string& locationName, const std::string& timeOfDay) {
    for (const auto& slot : clueSlots) {
        if (slot.location == locationName && slot.time == timeOfDay) {
            int tempWeight; // Temporary variable to hold the weight
            return clueManager.getDynamicClue(slot.clueIndex, tempWeight);
        }
    }
    return "No clue found for this time and location.";
}


int ScreenManager::getClueIndex(const std::string& locationName, const std::string& timeOfDay) {
    for (const auto& slot : clueSlots) {
        if (slot.location == locationName && slot.time == timeOfDay) {
            return slot.clueIndex;
        }
    }
    return -1; // Indicates no clue found for the given location and time
}


void ScreenManager::initializeClueSlots() {
    std::vector<std::string> times = { "Morning", "Noon", "Afternoon", "Evening" };
    std::vector<std::string> locations = { "Gym", "Police Station", "Shop", "Cafe", "Pond", "Library", "Alex's House" };

    std::random_device rd;
    std::mt19937 gen(rd());

    for (const auto& time : times) {
        for (const auto& location : locations) {
            std::uniform_int_distribution<> distr(0, 21 - 1);
            int clueIndex = distr(gen);
            clueSlots.push_back({ location, time, clueIndex });
        }
    }
}

void ScreenManager::resetForNewDay() {
    discoveredCluesToday.clear();
    cluesCollected = 0;
}

void ScreenManager::changeScreen(Screen* newScreen) {
    std::cout << "Changing screen from: " << (currentScreen ? currentScreen->name : "null")
        << " to: " << (newScreen ? newScreen->name : "null") << std::endl;
    currentScreen = newScreen;
    clueFound = false; // Reset clue found state when changing screens
}


void ScreenManager::goToLocationSelector() {
    currentCharacters.clear(); // Clear the current character dialogues
    currentScreen = locationSelectorScreen;
}

void ScreenManager::goToLocation(Screen* locationScreen) {
    currentCharacters.clear(); // Clear the current character dialogues
    currentScreen = locationScreen;
}