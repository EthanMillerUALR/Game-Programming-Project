#pragma once
#include <SDL2/SDL_mixer.h>  // SDL_mixer for audio
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>

class SoundManager {
public:
    // Initialize SDL_mixer
    static bool init() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            SDL_Log("Failed to initialize SDL_mixer: %s", Mix_GetError());
            return false;
        }
        return true;
    }

    // Clean up SDL_mixer
    static void clean() {
        for (auto& sound : sounds) {
            Mix_FreeChunk(sound.second);  // Free all loaded sound chunks
        }
        sounds.clear();
        if (backgroundMusic) {
            Mix_FreeMusic(backgroundMusic);
            backgroundMusic = nullptr;
        }
        Mix_CloseAudio();
    }

    // Load a sound file into the manager
    static bool loadSound(const std::string& id, const std::string& filepath) {
        if (sounds.find(id) != sounds.end()) return true;  // Already loaded
        Mix_Chunk* chunk = Mix_LoadWAV(filepath.c_str());
        if (!chunk) {
            SDL_Log("Failed to load sound file '%s': %s", filepath.c_str(), Mix_GetError());
            return false;
        }
        sounds[id] = chunk;
        return true;
    }

    // Play a sound by ID
    static void playSound(const std::string& id) {
        auto it = sounds.find(id);
        if (it != sounds.end()) {
            Mix_PlayChannel(-1, it->second, 0);  // Play on the first available channel
        }
        else {
            SDL_Log("Sound ID '%s' not found", id.c_str());
        }
    }
    // Load background music
    static bool loadMusic(const std::string& filepath) {
        if (backgroundMusic) Mix_FreeMusic(backgroundMusic);  // Free old music
        backgroundMusic = Mix_LoadMUS(filepath.c_str());
        if (!backgroundMusic) {
            SDL_Log("Failed to load music file '%s': %s", filepath.c_str(), Mix_GetError());
            return false;
        }
        return true;
    }

    // Play background music
    static void playMusic(int loops = -1) {
        if (backgroundMusic) {
            Mix_PlayMusic(backgroundMusic, loops);  // -1 loops means infinite
        }
        else {
            SDL_Log("No background music loaded to play.");
        }
    }

    // Stop background music
    static void stopMusic() {
        Mix_HaltMusic();
    }

private:
    static std::unordered_map<std::string, Mix_Chunk*> sounds;  // Store loaded sounds
    static Mix_Music* backgroundMusic;  // Store background music
};

