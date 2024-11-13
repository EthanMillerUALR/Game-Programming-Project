#include "Engine.h"
#include "tinyxml2.h"

// Definition of static members
bool Engine::isRunning = false;
SDL_Window* Engine::window = nullptr;
SDL_Renderer* Engine::renderer = nullptr;
std::vector<std::unique_ptr<GameObject>> Engine::gameObjects;
int Engine::width = 0;
int Engine::height = 0;

Engine::Engine(const std::string& levelPath)
{
    std::cout << "Loading level from: " << levelPath << std::endl;
    loadLevel(levelPath);
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






