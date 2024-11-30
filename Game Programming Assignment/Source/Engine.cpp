#include "Engine.h"
#include "GameObject.h"
#include "BodyComponent.h"  // Include to use BodyComponent if needed


// Definition of static members
bool Engine::isRunning = false;
SDL_Window* Engine::window = nullptr;
SDL_Renderer* Engine::renderer = nullptr;
std::vector<std::unique_ptr<GameObject>> Engine::gameObjects;
int Engine::width = 0;
int Engine::height = 0;

Uint64 Engine::lastTicks = 0;
double Engine::deltaTime = 0.0;

View Engine::view; // Define the static View instance

b2World* Engine::world = nullptr;  // Initialize the Box2D world pointer
b2Vec2 Engine::gravity(0.0f, 0.0f);  // Default gravity vector (downward)

std::vector<GameObject*> Engine::toBeAdded;
std::vector<GameObject*> Engine::toBeDeleted;

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

        // Create a blank GameObject
        auto gameObject = std::make_unique<GameObject>();

        bool componentAdded = false;

        // Add components to GameObject based on XML
        for (tinyxml2::XMLElement* compElement = elem->FirstChildElement(); compElement != nullptr; compElement = compElement->NextSiblingElement()) {
            const std::string compType = compElement->Name();  // Use tag name directly as component type

            auto component = ComponentLibrary().createComponent(compType, *gameObject, compElement);

            if (component) {
                gameObject->addComponent(std::move(component)); // Use addComponent again
                componentAdded = true;
            }
            else {
                SDL_Log("Component creation failed for: %s", compType.c_str());
            }
        }


        // Only add GameObject to engine if it has at least one component
        if (componentAdded) {
            Engine::scheduleAddGameObject(gameObject.release());
        } else {
        }
    }

    // Confirm load completion
    SDL_Log("Level loading completed from %s", levelPath.c_str());
}

double Engine::getDeltaTime() {
    return deltaTime;
}

