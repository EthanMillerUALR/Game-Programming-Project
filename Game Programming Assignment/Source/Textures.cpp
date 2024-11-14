#include "Textures.h"
#include <SDL2/SDL_image.h>

// Define the static member textures and image folder path
std::unordered_map<std::string, SDL_Texture*> Textures::textures;
const std::string Textures::imageFolderPath = "Assets/Images/";

SDL_Texture* Textures::get(const std::string& textureKey) {
    auto it = textures.find(textureKey);
    if (it != textures.end()) {
        return it->second;
    }

    SDL_Log("Texture %s not found in cache", textureKey.c_str());
    return nullptr;
}

SDL_Texture* Textures::getOrLoad(const std::string& textureKey, SDL_Renderer* renderer) {
    // Check if the texture is already loaded
    auto it = textures.find(textureKey);
    if (it != textures.end()) {
        return it->second;  // Return the cached texture without logging
    }

    // Build the full path to the image file in the Assets/Images directory
    std::string fullPath = imageFolderPath + textureKey;

    // Attempt to load the texture from the file
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, fullPath.c_str());
    if (newTexture) {
        textures[textureKey] = newTexture;
        SDL_Log("Loaded texture %s from %s", textureKey.c_str(), fullPath.c_str());  // Log only on first load
    }
    else {
        SDL_Log("Failed to load texture %s from %s, Error: %s", textureKey.c_str(), fullPath.c_str(), SDL_GetError());
    }

    return newTexture;
}


