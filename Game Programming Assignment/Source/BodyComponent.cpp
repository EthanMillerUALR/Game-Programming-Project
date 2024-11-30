#include "BodyComponent.h"
#include "GameObject.h"
#include <SDL2/SDL.h>


std::unique_ptr<Component> BodyComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    const char* typeStr = element->Attribute("btype");
    b2BodyType bodyType = b2_dynamicBody; // Default type
    if (typeStr) {
        if (strcmp(typeStr, "static") == 0) bodyType = b2_staticBody;
        else if (strcmp(typeStr, "kinematic") == 0) bodyType = b2_kinematicBody;
    }
    double x = element->DoubleAttribute("x", 0.0f);
    double y = element->DoubleAttribute("y", 0.0f);
    int width = element->IntAttribute("width", 50);
    int height = element->IntAttribute("height", 50);
    double vx = element->DoubleAttribute("vx", 0.0f);
    double vy = element->DoubleAttribute("vy", 0.0f);
    double angle = element->DoubleAttribute("angle", 0.0f);

    auto component = std::make_unique<BodyComponent>(parent, x, y, width, height, vx, vy, angle);
    component->setBodyType(bodyType); // Store the body type in the component

    // If the GameObject has a b2Body, set its initial state
    if (auto* body = parent.getBody()) {
        body->SetTransform(b2Vec2(x, y), angle);
        body->SetLinearVelocity(b2Vec2(vx, vy));
    }

    return component;
}

float BodyComponent::getX() const {
    auto* body = parent().getBody();
    if(body) {
        // Sync the GameObject position with the body position
        return body->GetPosition().x;
    }
    return 0.0f;  // Default if no body body ? body->GetPosition().x : 0.0f;
}

float BodyComponent::getY() const {
    auto* body = parent().getBody();
    if (body) {
        // Sync the GameObject position with the body position
        return body->GetPosition().y;
    }
    return 0.0f;  // Default if no body
}

float BodyComponent::getAngle() const {
    auto* body = parent().getBody();
    if (body) {
        // Sync the GameObject angle with the body angle
        return body->GetAngle();
    }
    return 0.0f;  // Default if no body
}

b2Vec2 BodyComponent::getVelocity() const {
    auto* body = parent().getBody();
    if (body) {
        // Sync the GameObject velocity with the body velocity
        return body->GetLinearVelocity();
    }
    return b2Vec2(0.0f, 0.0f);  // Default if no body
}

void BodyComponent::setPosition(float x, float y) {
    auto* body = parent().getBody();
    if (body) {
        body->SetTransform(b2Vec2(x, y), body->GetAngle());
        std::cout << "Position set to: (" << x << ", " << y << ")" << std::endl;
    }
    
}

void BodyComponent::setVelocity(float vx, float vy) {
    auto* body = parent().getBody();
    if (body) {
        body->SetLinearVelocity(b2Vec2(vx, vy));
        std::cout << "Velocity set to: (" << vx << ", " << vy << ")" << std::endl;
    }
}

void BodyComponent::setAngle(float angle) {
    auto* body = parent().getBody();
    if (body) {
        body->SetTransform(body->GetPosition(), angle);
        std::cout << "Angle set to: " << angle << std::endl;
    }
}

void BodyComponent::update() {
    // No need to synchronize GameObject properties with the body anymore,
    // as we directly update the body in the GameObject's setBody() method.
}
