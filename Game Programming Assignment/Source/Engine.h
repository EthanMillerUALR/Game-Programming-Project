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

class ComponentLibrary;
class Engine {
public:
    static int width;
    static int height;

    Engine(const std::string& levelPath);  // Constructor to load level from XML

    void loadLevel(const std::string& levelPath);  // Load GameObjects from the given XML

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
        for (auto& gameObject : gameObjects) {
            gameObject->update();  // Update each GameObject
        }
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
        SDL_Quit();
    }

    // Check if the engine is still running (static)
    static bool running() {
        return isRunning;
    }

    // Add a GameObject to the engine (static)
    static void addGameObject(std::unique_ptr<GameObject> gameObject) {
        gameObjects.push_back(std::move(gameObject));  // Add the game object to the list
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

private:
    static bool isRunning;                               // Engine running state (static)
    static SDL_Window* window;                           // SDL window (static)
    static SDL_Renderer* renderer;                       // SDL renderer (static)
    static std::vector<std::unique_ptr<GameObject>> gameObjects;  // Track game objects

    static Uint64 lastTicks;
    static double deltaTime;     // Time elapsed between frames
};

//REMEMBER: The view class should be simple to start, just keep track of the x and y
//Some possible Ideas: Keep Track Of Character Movements

//Engine will keep track of an view x and a view y where you are looking
//Whenever something is being drawn, you can just subtract off the view x and view y
//Now if you change the view x and view y, the camera will move