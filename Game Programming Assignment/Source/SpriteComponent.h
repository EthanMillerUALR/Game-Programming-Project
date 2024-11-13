#pragma once
#include "Component.h"
#include "BodyComponent.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include "Textures.h"
#include "tinyxml2.h"

class SpriteComponent : public Component {
public:
    SpriteComponent(GameObject& parent, const std::string& textureKey);

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    void update() override {}
    void draw() override;

private:
    std::string textureKey;  // Store key for the preloaded texture
    bool bodyMissingLogged = false;
};


