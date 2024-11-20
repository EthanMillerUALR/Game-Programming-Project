#include "Engine.h"

// Definition of static members
bool Engine::isRunning = false;
SDL_Window* Engine::window = nullptr;
SDL_Renderer* Engine::renderer = nullptr;
std::vector<std::unique_ptr<GameObject>> Engine::gameObjects;
int Engine::width = 0;
int Engine::height = 0;

Uint64 Engine::currentTicks = 0;
Uint64 Engine::lastTicks = 0;
double Engine::deltaTime = 0.0;
int Engine::frameCount = 0;
double Engine::fps = 0.0;
Uint64 Engine::fpsStartTime = 0;

Uint64 Engine::logStartTime = 0;  // New timer for logging FPS

Engine::Engine(const std::string& levelPath)
{
    std::cout << "Loading level from: " << levelPath << std::endl;
    loadLevel(levelPath);
    logStartTime = SDL_GetPerformanceCounter();  // Initialize logging timer
}

void Engine::loadLevel(const std::string& levelPath) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(levelPath.c_str()) != tinyxml2::XML_SUCCESS) {
        SDL_Log("Failed to load level file: %s", levelPath.c_str());
        return;
    }

    // Get the root "Level" element
    tinyxml2::XMLElement* root = doc.FirstChildElement("Level");
    if (!root) {
        SDL_Log("Level root element missing in XML.");
        return;
    }

    // Process each Object element in the XML
    for (tinyxml2::XMLElement* elem = root->FirstChildElement("Object"); elem != nullptr; elem = elem->NextSiblingElement("Object")) {
        SDL_Log("Creating a new GameObject");

        // Create a blank GameObject
        auto gameObject = std::make_unique<GameObject>();

        bool componentAdded = false;

        // Add components to GameObject based on XML
        for (tinyxml2::XMLElement* compElement = elem->FirstChildElement(); compElement != nullptr; compElement = compElement->NextSiblingElement()) {
            const std::string compType = compElement->Name();  // Use tag name directly as component type

            SDL_Log("Creating component of type %s", compType.c_str());
            auto component = ComponentLibrary().createComponent(compType, *gameObject, compElement);

            if (component) {
                gameObject->addComponent(std::move(component)); // Use addComponent again
                SDL_Log("Added component of type %s", compType.c_str());
                componentAdded = true;
            }
            else {
                SDL_Log("Failed to create component of type %s", compType.c_str());
            }
        }

        // Only add GameObject to engine if it has at least one component
        if (componentAdded) {
            Engine::addGameObject(std::move(gameObject));
            SDL_Log("Added GameObject to engine");
        } else {
            SDL_Log("GameObject skipped due to no components.");
        }
    }

    // Confirm load completion
    SDL_Log("Level loading completed from %s", levelPath.c_str());
}

// New frame timing methods (Not Needed, To Be Simplified/Deleted)
void Engine::startFrame() {
    lastTicks = SDL_GetPerformanceCounter();  // Record the start time for this frame
}

void Engine::endFrame() {
    currentTicks = SDL_GetPerformanceCounter();  // Record the end time for this frame
    deltaTime = (currentTicks - lastTicks) / static_cast<double>(SDL_GetPerformanceFrequency()); // Calculate deltaTime
    calculateFrameRate();  // Update FPS

}

void Engine::calculateFrameRate() {
    frameCount++;  // Increment the frame count

    Uint64 elapsedTime = currentTicks - fpsStartTime;
    if (elapsedTime >= SDL_GetPerformanceFrequency()) {  // Check if 1 second has passed
        fps = static_cast<double>(frameCount) / (elapsedTime / static_cast<double>(SDL_GetPerformanceFrequency()));  // Calculate FPS
        frameCount = 0;  // Reset the frame count
        fpsStartTime = currentTicks;  // Reset the start time for FPS calculation
        logFPS();  // Log FPS to the console
    }
}

void Engine::logFPS() {
    SDL_Log("FPS: %.2f", fps);  // Log the calculated FPS to the console
}

double Engine::getDeltaTime() {
    return deltaTime;
}

double Engine::getFPS() {
    return fps;
}