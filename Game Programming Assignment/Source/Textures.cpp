#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Textures.h"

// Define the static member textures with std::pair for caching and flagging already-loaded status
std::unordered_map<std::string, std::pair<SDL_Texture*, bool>> Textures::textures;

SDL_Texture* Textures::getOrLoad(const std::string& textureKey, const std::string& fileName, SDL_Renderer* renderer) {
    auto it = textures.find(textureKey);

    // If texture already loaded, return it and log once for "already loaded"
    if (it != textures.end()) {
        if (!it->second.second) {  // Check if the "already loaded" log was shown
            /*SDL_Log("Texture %s already loaded", textureKey.c_str());*/
            it->second.second = true;  // Set flag to prevent future logs
        }
        return it->second.first;
    }

    // Load the texture if not already loaded
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, fileName.c_str());
    if (newTexture) {
        textures[textureKey] = std::make_pair(newTexture, false);  // Use make_pair to create the pair explicitly
        /*SDL_Log("Loaded texture: %s", fileName.c_str());*/  // Log successful load
    }
    else {
        /*SDL_Log("Failed to load texture: %s, Error: %s", fileName.c_str(), SDL_GetError());*/
    }

    return newTexture;
}





