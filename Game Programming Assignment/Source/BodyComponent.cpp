#include "BodyComponent.h"
#include "GameObject.h"
#include <SDL2/SDL.h>


std::unique_ptr<Component> BodyComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double x = element->DoubleAttribute("x", 0.0f);
    double y = element->DoubleAttribute("y", 0.0f);
    int width = element->IntAttribute("width", 50);
    int height = element->IntAttribute("height", 50);
    double vx = element->DoubleAttribute("vx", 0.0f);
    double vy = element->DoubleAttribute("vy", 0.0f);
    double angle = element->DoubleAttribute("angle", 0.0f);

    auto component = std::make_unique<BodyComponent>(parent, x, y, width, height, vx, vy, angle);

    // If the GameObject has a b2Body, set its initial state
    if (auto* body = parent.getBody()) {
        body->SetTransform(b2Vec2(x, y), angle);
        body->SetLinearVelocity(b2Vec2(vx, vy));
    }

    return component;
}

float BodyComponent::getX() const {
    auto* body = parent().getBody();
    return body ? body->GetPosition().x : 0.0f;
}

float BodyComponent::getY() const {
    auto* body = parent().getBody();
    return body ? body->GetPosition().y : 0.0f;
}

float BodyComponent::getAngle() const {
    auto* body = parent().getBody();
    return body ? body->GetAngle() : 0.0f;
}

b2Vec2 BodyComponent::getVelocity() const {
    auto* body = parent().getBody();
    return body ? body->GetLinearVelocity() : b2Vec2(0.0f, 0.0f);
}

void BodyComponent::setPosition(float x, float y) {
    auto* body = parent().getBody();
    if (body) {
        body->SetTransform(b2Vec2(x, y), body->GetAngle());
    }
}

void BodyComponent::setVelocity(float vx, float vy) {
    auto* body = parent().getBody();
    if (body) {
        body->SetLinearVelocity(b2Vec2(vx, vy));
    }
}

void BodyComponent::setAngle(float angle) {
    auto* body = parent().getBody();
    if (body) {
        body->SetTransform(body->GetPosition(), angle);
    }
}



void BodyComponent::update() {
    // No need to synchronize GameObject properties with the body anymore,
    // as we directly update the body in the GameObject's setBody() method.
}
