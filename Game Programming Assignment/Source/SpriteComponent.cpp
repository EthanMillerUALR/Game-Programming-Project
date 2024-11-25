#include "SpriteComponent.h"
#include "Engine.h"
#include "ComponentLibrary.h"

SpriteComponent::SpriteComponent(GameObject& parent, const std::string& textureKey)
    : Component(parent), textureKey(textureKey) {}

std::unique_ptr<Component> SpriteComponent::create(GameObject& parent, tinyxml2::XMLElement* element) {
    const char* textureKey = element->Attribute("texture");
    if (textureKey) {
        return std::make_unique<SpriteComponent>(parent, textureKey);
    }
    SDL_Log("No texture attribute found in XML for SpriteComponent.");
    return nullptr;
}

void SpriteComponent::draw() {
    // Construct the full path without adding any extensions
    std::string fullPath = "Assets/Images/" + textureKey;

    SDL_Texture* texture = Textures::getOrLoad(textureKey, fullPath, Engine::getRenderer());
    if (!texture) {
        SDL_Log("Failed to load texture: %s", textureKey.c_str());
        return;
    }

    auto body = parent().get<BodyComponent>();
    if (body) {
        bodyMissingLogged = false;

        int width = body->getWidth();
        int height = body->getHeight();

        // Adjust sprite's position based on the View's center
        double drawX = body->x() - Engine::view.getViewX();
        double drawY = body->y() - Engine::view.getViewY();

        SDL_Rect dst = {
            static_cast<int>(drawX),
            static_cast<int>(drawY),
            width,
            height
        };

        float angle = body->angle();
        SDL_Point center = { width / 2, height / 2 };

        if (SDL_RenderCopyEx(Engine::getRenderer(), texture, nullptr, &dst, angle, &center, SDL_FLIP_NONE) != 0) {
            SDL_Log("SDL_RenderCopyEx error: %s", SDL_GetError());
        }
    }
    else if (!bodyMissingLogged) {
        SDL_Log("BodyComponent is missing. Cannot draw SpriteComponent.");
        bodyMissingLogged = true;
    }
}








