#define SDL_MAIN_HANDLED
#include "Engine.h"
#include "Textures.h"

int main(int argc, char* argv[]) {
    // Initialize the engine
    if (!Engine::init("SDL2 Game Engine", 800, 600)) {
        return -1;
    }

    SDL_Renderer* renderer = Engine::getRenderer();

    // Load level from XML file
    Engine engine("Assets/GameObjects.xml");  // Loads level upon initialization

    // Run the game loop
    engine.run();

    return 0;
}



