#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <memory>  // For std::unique_ptr
#include <string>
#include <chrono>  // For time tracking
#include "Input.h"
#include <iostream>
#include "GameObject.h"
#include "ComponentLibrary.h"
#include "tinyxml2.h"  // Include the XML parser
#include "View.h"
#include <SDL2/SDL.h>
#include <box2d/box2d.h> // Include Box2D
#include "GameContactListener.h"

class ComponentLibrary;
class Engine {
public:
    static int width;
    static int height;

    Engine(const std::string& levelPath);  // Constructor to load level from XML

    void loadLevel(const std::string& levelPath);  // Load GameObjects from the given XML

    static View view;


    // Initialize the Engine (static)
    static bool init(const char* title, int width, int height) {
        Engine::width = width;
        Engine::height = height;
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
            return false;
        }

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
        if (!window) {
            SDL_Log("Failed to create window: %s", SDL_GetError());
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            SDL_Log("Failed to create renderer: %s", SDL_GetError());
            return false;
        }

        world = new b2World(gravity);
        world->SetContactListener(&contactListener);  // Assign the contact listener

        isRunning = true;
        return true;
    }

    // Handle events (static)
    static void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            Input::processEvent(event);  // Call static Input method
        }
    }

    // Update all game objects (static)
    static void update() {

        double deltaX = 0, deltaY = 0;

        if (Input::isKeyDown(SDLK_LEFT)) {
            deltaX -= 150 * Engine::getDeltaTime(); // Move view left
        }
        if (Input::isKeyDown(SDLK_RIGHT)) {
            deltaX += 150 * Engine::getDeltaTime();; // Move view right
        }
        if (Input::isKeyDown(SDLK_UP)) {
            deltaY -= 150 * Engine::getDeltaTime();; // Move view up
        }
        if (Input::isKeyDown(SDLK_DOWN)) {
            deltaY += 150 * Engine::getDeltaTime();; // Move view down
        }

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

        // Process additions and deletions after all game objects are updated
        processScheduledAdditions();
        processScheduledDeletions();
    }

    // Render all game objects (static)
    static void render() {
        SDL_SetRenderDrawColor(renderer, 34, 179, 34, 255);
        SDL_RenderClear(renderer);

        for (auto& gameObject : gameObjects) {
            gameObject->draw();  // Draw each GameObject
        }

        SDL_RenderPresent(renderer);
    }

    // Clean up SDL resources (static)
    static void clean() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        if (world) {
            delete world;  // Clean up Box2D world
            world = nullptr;
        }
        SDL_Quit();
    }

    // Check if the engine is still running (static)
    static bool running() {
        return isRunning;
    }

    // Run the engine (static)
    static void run() {
        const int targetFPS = 60;
        const double targetFrameTime = 1000.0 / targetFPS;

        while (isRunning) {
            Uint64 frameStart = SDL_GetPerformanceCounter();

            handleEvents();
            update();
            render();

            Uint64 frameEnd = SDL_GetPerformanceCounter();
            double frameDuration = (frameEnd - frameStart) * 1000.0 / SDL_GetPerformanceFrequency();

            // Delay to maintain target FPS
            if (frameDuration < targetFrameTime) {
                SDL_Delay(static_cast<Uint32>(targetFrameTime - frameDuration));
            }

            // Recalculate total frame time, including delay
            Uint64 frameComplete = SDL_GetPerformanceCounter();
            deltaTime = (frameComplete - frameStart) / static_cast<double>(SDL_GetPerformanceFrequency());

            // Log FPS
            double fps = 1.0 / deltaTime;
        }

        clean();
    }


    static SDL_Renderer* getRenderer() {
        return renderer;
    }
    static double getDeltaTime();  // Getter for deltaTime

    // Add a GameObject to the engine
    static void addGameObject(std::unique_ptr<GameObject> gameObject) {
        gameObjects.push_back(std::move(gameObject));  // Add the game object to the list
    }

    // Schedule a GameObject for addition
    static void scheduleAddGameObject(GameObject* gameObject) {
        toBeAdded.push_back(gameObject);
    }

    // Schedule a GameObject for deletion
    static void scheduleDeleteGameObject(GameObject* gameObject) {
        // If the GameObject has a b2Body, destroy it
        if (auto body = gameObject->getBody()) {
            if (world) {
                world->DestroyBody(body);  // Destroy the body from the Box2D world
            }
            gameObject->setBody(nullptr);  // Nullify the pointer to avoid dangling
        }

        toBeDeleted.push_back(gameObject);  // Schedule the GameObject for deletion
    }


    // Process additions after the main game loop
    static void processScheduledAdditions() {
        for (GameObject* gameObject : toBeAdded) {
            gameObjects.push_back(std::unique_ptr<GameObject>(gameObject));  // Add new GameObject
        }
        toBeAdded.clear();  // Clear the scheduled additions list
    }

    // Process deletions after the main game loop
    static void processScheduledDeletions() {
        for (GameObject* gameObject : toBeDeleted) {
            // Remove the GameObject from the active list
            auto it = std::remove_if(gameObjects.begin(), gameObjects.end(),
                [gameObject](const std::unique_ptr<GameObject>& obj) {
                    return obj.get() == gameObject;  // Compare raw pointers
                });

            if (it != gameObjects.end()) {
                gameObjects.erase(it, gameObjects.end());
            }

            // Clean up memory for the deleted object
            delete gameObject;
        }
        toBeDeleted.clear();  // Clear the scheduled deletions list
    }


    static b2World* getWorld() { return world; }

private:
    static bool isRunning;                               // Engine running state (static)
    static SDL_Window* window;                           // SDL window (static)
    static SDL_Renderer* renderer;                       // SDL renderer (static)
    static std::vector<std::unique_ptr<GameObject>> gameObjects;  // Track game objects
    static std::vector<GameObject*> toBeAdded;
    static std::vector<GameObject*> toBeDeleted;

    static Uint64 lastTicks;
    static double deltaTime;     // Time elapsed between frames

    // Box2D world and gravity
    static b2World* world;  // Pointer to the Box2D world
    static b2Vec2 gravity;  // Gravity vector

    static GameContactListener contactListener; // Declare a contact listener
};