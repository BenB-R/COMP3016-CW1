#include "Menu.h"

#include "TextureManager.h"

Menu::Menu(SDL_Renderer* rend) : renderer(rend), startGame(false), backgroundTexture(nullptr), font(nullptr) {
    // Load the background texture
    backgroundTexture = IMG_LoadTexture(renderer, "Assets/Backgrounds/menu.png"); // Update the path

    // Initialize the start button rectangle
    startButtonRect = { 500, 500, 280, 75 }; // Update as needed

    // Load the font
    font = TTF_OpenFont("Fonts/slkscr.ttf", 74); // Update the path and font size as needed
}

Menu::~Menu() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
    if (font) {
        TTF_CloseFont(font);
    }
}

void Menu::handleEvent(const SDL_Event& event) {
    // Check for mouse button click events
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY); // Get the mouse coordinates
        SDL_Point clickedPoint = { mouseX, mouseY };
        if (SDL_PointInRect(&clickedPoint, &startButtonRect)) {
            startGame = true; // Set startGame to true if the "Start" button was clicked
        }
    }
}

void Menu::render() {
    // Render the background
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Render the "Start" button
    // Draw the rectangle for the button
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for the button
    SDL_RenderFillRect(renderer, &startButtonRect);

    // Render the "Start" text on the button
    SDL_Color textColor = { 255, 255, 255, 255 }; // White text
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Start", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {
        startButtonRect.x + (startButtonRect.w - textSurface->w) / 2,
        startButtonRect.y + (startButtonRect.h - textSurface->h) / 2,
        textSurface->w, textSurface->h
    };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Cleanup
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

bool Menu::shouldStartGame() const {
    return startGame;
}
