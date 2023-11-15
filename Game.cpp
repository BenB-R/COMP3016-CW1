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
    window = SDL_CreateWindow("Game Title",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return
    }

    // Initialize ScreenManager
    screenManager = new ScreenManager(renderer);
}


void Game::run() {
    mainLoop();
}

void Game::mainLoop() {
    while (isRunning) {
        // Handle events, update game state, and render
    }
}

void Game::cleanUp() {
    // Clean up resources
}
