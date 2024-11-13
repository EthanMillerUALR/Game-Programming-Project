#include "Textures.h"
#include <SDL2/SDL_image.h>

// Define the static member textures
std::unordered_map<std::string, SDL_Texture*> Textures::textures;

SDL_Texture* Textures::get(const std::string& textureKey) {
    // Return the texture if it exists in the cache
    if (textures.find(textureKey) != textures.end()) {
        return textures[textureKey];
    }

    // Log an error if the texture is not found
    SDL_Log("Texture not found for key: %s", textureKey.c_str());
    return nullptr;
}

SDL_Texture* Textures::getOrLoad(const std::string& textureKey, const std::string& fileName, SDL_Renderer* renderer) {
    // Check if the texture is already loaded
    if (textures.find(textureKey) != textures.end()) {
        return textures[textureKey];  // Return the already loaded texture
    }

    // If not loaded, load the texture using SDL_image
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, fileName.c_str());
    if (newTexture) {
        // Cache the loaded texture
        textures[textureKey] = newTexture;
    }
    else {
        SDL_Log("Failed to load texture: %s, Error: %s", fileName.c_str(), SDL_GetError());
    }

    return newTexture;
}
