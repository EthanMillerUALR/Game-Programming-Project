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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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
        lastTicks = SDL_GetPerformanceCounter();  // Record the start time for the frame loop

        while (isRunning) {
            currentTicks = SDL_GetPerformanceCounter();  // Record the current time for this frame
            deltaTime = (currentTicks - lastTicks) / static_cast<double>(SDL_GetPerformanceFrequency());  // Calculate deltaTime
            lastTicks = currentTicks;  // Set the lastTicks to current for the next frame

            handleEvents();
            update();
            render();

            frameCount++;  // Increment frame count for FPS calculation

            // Calculate FPS every second
            Uint64 elapsedTime = currentTicks - fpsStartTime;
            if (elapsedTime >= SDL_GetPerformanceFrequency()) {
                fps = static_cast<double>(frameCount) / (elapsedTime / static_cast<double>(SDL_GetPerformanceFrequency()));  // FPS = frames per second
                frameCount = 0;  // Reset the frame count after calculating FPS
                fpsStartTime = currentTicks;  // Reset the time for FPS calculation
            }

            // Log FPS every second
            Uint64 logElapsedTime = currentTicks - logStartTime;
            if (logElapsedTime >= SDL_GetPerformanceFrequency()) {
                SDL_Log("FPS: %.2f", fps);  // Log FPS to console every second
                logStartTime = currentTicks;  // Reset the log timer
            }

            // Optional: Add a small delay to prevent the loop from maxing out the CPU (e.g., 1ms delay)
            SDL_Delay(0);  // Delay 1ms to reduce CPU usage
        }

        clean();  // Clean up when the game ends
    }


    static SDL_Renderer* getRenderer() {
        return renderer;
    }
    static double getDeltaTime();  // Getter for deltaTime
    static double getFPS();        // Getter for FPS

    // New methods for frame rate tracking
    static void startFrame();
    static void endFrame();
    static void calculateFrameRate();
    static void logFPS();  // Add this declaration

private:
    static bool isRunning;                               // Engine running state (static)
    static SDL_Window* window;                           // SDL window (static)
    static SDL_Renderer* renderer;                       // SDL renderer (static)
    static std::vector<std::unique_ptr<GameObject>> gameObjects;  // Track game objects

    static Uint64 currentTicks;  // For frame timing
    static Uint64 lastTicks;
    static double deltaTime;     // Time elapsed between frames
    static int frameCount;       // Count frames for FPS calculation
    static double fps;           // FPS calculation
    static Uint64 fpsStartTime;  // Time when FPS calculation starts

    static Uint64 logStartTime;  // For logging FPS every second
};