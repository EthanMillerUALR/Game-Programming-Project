#include "SpriteComponent.h"
#include "Engine.h"
#include "ComponentLibrary.h"

SpriteComponent::SpriteComponent(GameObject& parent, const std::string& textureKey)
    : Component(parent), textureKey(textureKey) {}  // Just store the key

std::unique_ptr<Component> SpriteComponent::create(GameObject& parent, tinyxml2::XMLElement* element) {
    const char* textureKey = element->Attribute("texture");
    if (textureKey) {
        return std::make_unique<SpriteComponent>(parent, textureKey);
    }
    SDL_Log("No texture attribute found in XML for SpriteComponent.");
    return nullptr;
}

void SpriteComponent::draw() {
    SDL_Texture* texture = Textures::get(textureKey);  // Get the preloaded texture
    if (!texture) {
        SDL_Log("Texture %s not found", textureKey.c_str());
        return;
    }

    auto body = parent().get<BodyComponent>();  // Get the BodyComponent
    if (body) {
        // Reset the missing log flag since we have a BodyComponent
        bodyMissingLogged = false;

        int width = body->getWidth();
        int height = body->getHeight();

        // Setup destination rectangle for drawing
        SDL_Rect dst = { static_cast<int>(body->x()), static_cast<int>(body->y()), width, height };

        // Get angle and center for rotation (if any)
        float angle = body->angle();
        SDL_Point center = { width / 2, height / 2 };

        // Perform the draw operation
        SDL_RenderCopyEx(Engine::getRenderer(), texture, nullptr, &dst, angle, &center, SDL_FLIP_NONE);
    }
    else {
        if (!bodyMissingLogged) {
            SDL_Log("BodyComponent is missing. Cannot draw SpriteComponent.");
            bodyMissingLogged = true;  // Log only once
        }
    }
}



