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

    b2Body* body = parent().getBody();
    if (body) {
        bodyMissingLogged = false;

        // Get the size of the sprite from the BodyComponent (if relevant)
        BodyComponent* bodyComponent = parent().get<BodyComponent>();
        int width = bodyComponent ? bodyComponent->getWidth() : 0;
        int height = bodyComponent ? bodyComponent->getHeight() : 0;

        // Use the Box2D body's position, adjusted for the View
        const b2Vec2& position = body->GetPosition();
        double drawX = position.x - Engine::view.getViewX();
        double drawY = position.y - Engine::view.getViewY();

        SDL_Rect dst = {
            static_cast<int>(drawX),
            static_cast<int>(drawY),
            width,
            height
        };

        // Get the angle of the body for rotation
        float angle = body->GetAngle() * (180.0f / M_PI); // Convert from radians to degrees
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








