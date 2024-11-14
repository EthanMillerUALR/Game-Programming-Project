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
    SDL_Texture* texture = Textures::getOrLoad(textureKey, Engine::getRenderer());  // Load or retrieve the texture
    if (!texture) {
        SDL_Log("Texture %s not found", textureKey.c_str());
        return;
    }

    auto body = parent().get<BodyComponent>();  // Get the BodyComponent
    if (body) {
        bodyMissingLogged = false;

        int width = body->getWidth();
        int height = body->getHeight();

        SDL_Rect dst = { static_cast<int>(body->x()), static_cast<int>(body->y()), width, height };

        float angle = body->angle();
        SDL_Point center = { width / 2, height / 2 };

        SDL_RenderCopyEx(Engine::getRenderer(), texture, nullptr, &dst, angle, &center, SDL_FLIP_NONE);
    }
    else if (!bodyMissingLogged) {
        SDL_Log("BodyComponent is missing. Cannot draw SpriteComponent.");
        bodyMissingLogged = true;
    }
}




