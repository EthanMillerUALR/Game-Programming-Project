#include "ControllerComponent.h"

#include "Engine.h"
#include <tuple>
#include <utility>

std::unique_ptr<Component> ControllerComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double speed = element->IntAttribute("speed", 0);

    auto component = std::make_unique<ControllerComponent>(parent, speed);
    return component;
}

void ControllerComponent::setSpeed(float speed) {
    moveSpeed = static_cast<float>(speed);
}

void ControllerComponent::update() {
    b2Body* body = parent().getBody();
    if (!body) return;

    double newSpeed = moveSpeed;

    bool upPressed = Input::isKeyDown(SDLK_w);
    bool downPressed = Input::isKeyDown(SDLK_s);
    bool leftPressed = Input::isKeyDown(SDLK_a);
    bool rightPressed = Input::isKeyDown(SDLK_d);

    b2Vec2 velocity(0, 0);
    if (upPressed) {
        velocity.y -= newSpeed;
    }
    if (downPressed) {
        velocity.y += newSpeed;
    }
    if (leftPressed) {
        velocity.x -= newSpeed;
    }
    if (rightPressed) {
        velocity.x += newSpeed;
    }

    body->SetLinearVelocity(velocity);

    mouseAngle(body);
}


void ControllerComponent::mouseAngle(b2Body* body) {
    int mouseX;
    int mouseY;
    std::tie(mouseX, mouseY) = Input::getMousePosition();

    b2Vec2 position = body->GetPosition();
    double objectX = position.x;
    double objectY = position.y;

    double dx = mouseX - objectX;
    double dy = mouseY - objectY;

    double angle = atan2(dy, dx) * 180.0 / M_PI;

    body->SetTransform(body->GetPosition(), static_cast<float>(angle) * M_PI / 180.0f); // Convert to radians
}
