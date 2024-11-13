#define SDL_MAIN_HANDLED
#include "Engine.h"
#include "Textures.h"

int main(int argc, char* argv[]) {
    // Initialize the engine
    if (!Engine::init("SDL2 Game Engine", 800, 600)) {
        return -1;
    }

    SDL_Renderer* renderer = Engine::getRenderer();

    // Load textures
    if (Textures::getOrLoad("hero", "dino.png", renderer) == nullptr) {
        SDL_Log("Failed to load hero texture");
        return -1;
    }
    if (Textures::getOrLoad("enemy", "squirrel.png", renderer) == nullptr) {
        SDL_Log("Failed to load enemy texture");
        return -1;
    }

    // Load level from XML file
    Engine engine("Assets/GameObjects.xml");  // Loads level upon initialization

    // Run the game loop
    engine.run();

    return 0;
}



