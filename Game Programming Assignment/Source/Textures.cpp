#include "Textures.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_log.h>

// Define the static member `textures`
std::unordered_map<std::string, SDL_Texture*> Textures::textures;

SDL_Texture* Textures::getOrLoad(const std::string& textureKey, const std::string& fileName, SDL_Renderer* renderer) {
    auto it = textures.find(textureKey);

    // If texture is already loaded, return it and log only once
    if (it != textures.end()) {
        return it->second; // Return the cached texture
    }

    // Load the texture if not already loaded
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, fileName.c_str());
    if (newTexture) {
        textures[textureKey] = newTexture; // Cache the texture
        SDL_Log("Loaded texture: %s", fileName.c_str()); // Log successful load
    }
    else {
        SDL_Log("Failed to load texture: %s, Error: %s", fileName.c_str(), SDL_GetError());
    }

    return newTexture;
}