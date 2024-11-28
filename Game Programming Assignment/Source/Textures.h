#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>

class Textures {
public:
    static SDL_Texture* getOrLoad(const std::string& textureKey, const std::string& fileName, SDL_Renderer* renderer);

private:
    static std::unordered_map<std::string, SDL_Texture*> textures;
};

