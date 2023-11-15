#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const std::string& fileName, SDL_Renderer* renderer);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_Renderer* renderer);

private:
    static std::map<std::string, SDL_Texture*> textures; // Stores textures
};

#endif // TEXTUREMANAGER_H
