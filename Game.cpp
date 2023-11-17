#include "Game.h"

Game::Game() : window(nullptr), renderer(nullptr), screenManager(nullptr), isRunning(true) {
    init();
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
        // Event Handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;  // Set the running flag to false if the user wants to quit
            }
            screenManager->handleEvent(event);  // Pass the event to ScreenManager for handling
        }

        // Game State Updates
        screenManager->update();  // Update the current screen state

        // Rendering
        SDL_RenderClear(renderer);  // Clear the screen with the default color
        screenManager->render();    // Render the current screen
        SDL_RenderPresent(renderer);  // Present the rendered image to the screen
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
    SDL_Quit();  // Quit SDL
}

