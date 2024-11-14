#define SDL_MAIN_HANDLED
#include "Engine.h"
#include "Textures.h"

int main(int argc, char* argv[]) {
    // Initialize the engine (set up SDL, window, and renderer)
    if (!Engine::init("SDL2 Game Engine", 800, 600)) {
        return -1;
    }

    // Load level from XML file and create the Engine instance
    Engine engine("Assets/GameObjects.xml");  // Initializes the engine and loads the level

    // Run the game loop
    engine.run();  // Start the game loop

    return 0;
}




