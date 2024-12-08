#include "Engine.h"
#include "GameObject.h"
#include "BodyComponent.h"  // Include to use BodyComponent if needed
#include "SpriteComponent.h"
#include "ZombieComponent.h"
#include "HeroComponent.h"


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

GameContactListener Engine::contactListener;

int Engine::currentLevel = 0;  // Start with Level0.xml
int Engine::heroDelay = 0;  // Start delay with 0
int Engine::zombieDelay = 0;  // Start delay with 0


Engine::Engine(const std::string& levelPath)
{
    loadLevel(levelPath);
    // Load Sounds
    SoundManager::loadSound("zombieDamage", "Assets/SoundFiles/zombieDamage.wav");
    SoundManager::loadSound("backgroundMusic", "Assets/SoundFiles/backgroundMusic.wav");
    SoundManager::loadSound("gunshot", "Assets/SoundFiles/gunshot.wav");
    SoundManager::loadSound("levelClear", "Assets/SoundFiles/levelClear.wav");
    SoundManager::loadSound("zombieGroan", "Assets/SoundFiles/zombieGroan.wav");
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
}

void Engine::update()
{

        double deltaX = 0, deltaY = 0;

        if (deltaX != 0 || deltaY != 0) {
            view.moveView(deltaX, deltaY);
        }

        // Step the Box2D world
        if (world) {
            world->Step(static_cast<float>(deltaTime), 8, 3);  // Box2D step
        }

        for (auto& gameObject : gameObjects) {
            gameObject->update();  // Update each GameObject
        }

        GameObject* zombieGameObject = findGameObjectComponent<ZombieComponent>();
        if (!zombieGameObject) {
            zombieDelay++;
            if (zombieDelay >= 3) {
                currentLevel++;
                if (currentLevel <= 4) {  // Ensure we don't go beyond Level4.xml
                    std::string nextLevel = "Assets/XMLFiles/Level" + std::to_string(currentLevel) + ".xml";
                    loadLevel(nextLevel);  // Load the next level
                    SoundManager::playSound("levelClear");
                }
                else {
                    SDL_Log("All levels completed! The Hero Has Won!");
                    isRunning = false;  // Optionally, stop the engine
                }
            }
        }
        else {
            zombieDelay = 0;
        }

        GameObject* heroGameObject = findGameObjectComponent<HeroComponent>();
        if (!heroGameObject) {
            heroDelay++;
            if (heroDelay >= 3) {  // Ensure we don't go more than 2 updates without a hero
                SDL_Log("The hero has died, the game is over..");
                isRunning = false;  // Optionally, stop the engine    
            }
        }
        else {
            heroDelay = 0;
        }

        // Process additions and deletions after all game objects are updated
        processScheduledAdditions();
        processScheduledDeletions();
}

double Engine::getDeltaTime() {
    return deltaTime;
}


