#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>

class Textures {
public:

    // Method to retrieve an already-loaded texture by key
    static SDL_Texture* get(const std::string& textureKey);

    // Method to get a texture by its key, or load it if not already loaded
    static SDL_Texture* getOrLoad(const std::string& textureKey, const std::string& fileName, SDL_Renderer* renderer);

private:

    // Static map to cache textures by their key
    static std::unordered_map<std::string, SDL_Texture*> textures;

};



//IDEA: have images be assigned xml 'values'
//when an image is called by xml, it checks to see if the image is loaded or now
//if the image isnt loaded, it loads the image. if the image is loaded, it uses the already loaded image