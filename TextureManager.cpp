#include "TextureManager.h"

std::map<std::string, SDL_Texture*> TextureManager::textures;

SDL_Texture* TextureManager::LoadTexture(const std::string& fileName, SDL_Renderer* renderer) {
    if (textures[fileName] == nullptr) {
        SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
        if (tempSurface == nullptr) {
            SDL_Log("Failed to load image %s, SDL_image Error: %s", fileName.c_str(), IMG_GetError());
            return nullptr;
        }
        textures[fileName] = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }

    return textures[fileName];
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, tex, &src, &dest);
}
