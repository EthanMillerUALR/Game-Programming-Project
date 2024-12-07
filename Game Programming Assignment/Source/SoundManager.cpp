#include "SoundManager.h"

std::unordered_map<std::string, Mix_Chunk*> SoundManager::sounds;  // Define static member
Mix_Music* SoundManager::backgroundMusic = nullptr;