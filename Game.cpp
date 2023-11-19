#include "Game.h"

#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), screenManager(nullptr), isRunning(true), onMenuScreen(true), onStoryScreen(false), gameEnded(false) {
    init();
    menu = new Menu(renderer);
     initStoryScreen();
     lines = {
     	"You awaken in a quiet village where night seems eternal. A spectral figure looms in the shadows of your slumbering mind, whispering a dire omen:",
        "\"At the stroke of midnight, death's hand will claim a soul. This cursed loop will bind you, replaying this grim tale until the chain is broken.\"",
        "With determination, you resolve to scour the village for clues and engage with its inhabitants. Their words may reveal the path to prevention or lead you astray. Amass evidence to foil the dark fate that looms.",
        "But heed this warning: when midnight's bell tolls, ensure you are not found outside, lest the specter of death turns its gaze upon you.",
        "Gather your wits and courage. The sands of time wait for no one. Can you unravel the mystery and break the cycle before it claims you?",
        "[Press SPACE to continue]"
     };
     textColor = { 255, 255, 255, 255 }; // White text

    // Initialize the button in the Game constructor or a separate initialization method
     Button returnToMenuButton = {{100, 600, 200, 50}, nullptr}; // Adjust size and position
     returnToMenuButton.label = "Return to Start Menu";
}

// Method to initialize story screen resources
void Game::initStoryScreen() {
    // Load the font
    storyFont = TTF_OpenFont("Fonts/slkscr.ttf", 24);
    // Assuming you've already initialized SDL_ttf with TTF_Init()
    SDL_RenderCopy(renderer, storyTextTexture, NULL, &storyTextRect);
    SDL_Color textColor = { 255, 255, 255, 255 }; // White text
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(storyFont, "Your story text here...", textColor, 1080);
    storyTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    storyTextRect = { 100, 100, textSurface->w, textSurface->h };
    SDL_FreeSurface(textSurface);
}

void Game::init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    // Create window
    window = SDL_CreateWindow("Before Midnight",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Initialize ScreenManager
    screenManager = new ScreenManager(renderer);
}


void Game::run() {
    mainLoop();
}

void Game::mainLoop() {
    SDL_Event event;
    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }

            if (onMenuScreen) {
                menu->handleEvent(event);
                if (menu->shouldStartGame()) {
                    onMenuScreen = false;
                    onStoryScreen = true;
                    // Initialize or update story screen if needed
                }
            }
            else if (onStoryScreen) {
                // Handle story screen events
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                    onStoryScreen = false;
                    // Initialize or reset main game state if needed
                }
            }
            else {
                screenManager->handleEvent(event);  // Handle game events
                screenManager->update();
            }
        }

        SDL_RenderClear(renderer);

        if (onMenuScreen) {
            menu->render();
        }
        else if (onStoryScreen) {
            renderStoryScreen();
        }
        else {
            screenManager->render();  // Render main game screen
        }
        
        if (gameEnded && event.type == SDL_MOUSEBUTTONDOWN) {
            SDL_Point clickPoint = { event.button.x, event.button.y };
            if (SDL_PointInRect(&clickPoint, &returnToMenuButton.rect)) {
                // Reset the game state to show the start menu
                onMenuScreen = true;
                gameEnded = false;
                onStoryScreen = false;
                // Reset other necessary game state variables
            }
        }

        SDL_RenderPresent(renderer);
    }
}

void Game::render() {
    
    if (gameEnded) {
        std::cout << "Hello" << std::endl;
        // Render the returnToMenuButton
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set button color (example: black)
        SDL_RenderFillRect(renderer, &returnToMenuButton.rect);

        // Render the button label
        SDL_Surface* surfaceLabel = TTF_RenderText_Blended(storyFont, returnToMenuButton.label.c_str(), textColor);
        SDL_Texture* labelTexture = SDL_CreateTextureFromSurface(renderer, surfaceLabel);
        SDL_Rect labelRect = {
            returnToMenuButton.rect.x + (returnToMenuButton.rect.w - surfaceLabel->w) / 2,
            returnToMenuButton.rect.y + (returnToMenuButton.rect.h - surfaceLabel->h) / 2,
            surfaceLabel->w, surfaceLabel->h
        };
        SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect);

        // Clean up
        SDL_FreeSurface(surfaceLabel);
        SDL_DestroyTexture(labelTexture);
    }
}



void Game::renderStoryScreen() {
    int yPosition = 100; // Starting Y position for the text
    int lineHeight = 30; // Height of each line of text

    for (const auto& line : lines) {
        SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(storyFont, line.c_str(), textColor, 1080);
        if (!textSurface) {
            SDL_Log("Failed to create text surface: %s", TTF_GetError());
            continue;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            SDL_Log("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(textSurface);
            continue;
        }

        SDL_Rect textRect = { 100, yPosition, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture); // Free texture after rendering

        yPosition += lineHeight + textSurface->h; // Move down for the next line
    }
}



void Game::cleanUp() {
    if (screenManager != nullptr) {
        delete screenManager;
        screenManager = nullptr;
    }
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (menu != nullptr) {
        delete menu;
        menu = nullptr;
    }
    if (storyFont) {
        TTF_CloseFont(storyFont);
        storyFont = nullptr;
    }
    if (storyTextTexture) {
        SDL_DestroyTexture(storyTextTexture);
        storyTextTexture = nullptr;
    }

    SDL_Quit();  // Quit SDL
}