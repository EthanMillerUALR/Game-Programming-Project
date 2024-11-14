#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>

class Textures {
public:
    // Add `fileName` as a parameter
    static SDL_Texture* getOrLoad(const std::string& textureKey, const std::string& fileName, SDL_Renderer* renderer);

private:
    // Declare `textures` to store both the texture pointer and the loaded-flag boolean
    static std::unordered_map<std::string, std::pair<SDL_Texture*, bool>> textures;
};






//IDEA: have images be assigned xml 'values'
//when an image is called by xml, it checks to see if the image is loaded or now
//if the image isnt loaded, it loads the image. if the image is loaded, it uses the already loaded image