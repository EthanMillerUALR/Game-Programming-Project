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

    body->SetLinearVelocity(b2Vec2(0, 0));

    double newSpeed = (moveSpeed * Engine::getDeltaTime()) / 2;

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


    checkBounds(body);
}


void ControllerComponent::mouseAngle(b2Body* body) {
    // Get mouse position

    int mouseX;
    int mouseY;
    std::tie(mouseX, mouseY) = Input::getMousePosition();

    // Get object's current position
    b2Vec2 position = body->GetPosition();
    double objectX = position.x;
    double objectY = position.y;

    // Calculate the difference in position
    double dx = mouseX - objectX;
    double dy = mouseY - objectY;

    // Calculate the angle in degrees
    double angle = atan2(dy, dx) * 180.0 / M_PI;

    // Set the angle in the body
    body->SetTransform(body->GetPosition(), static_cast<float>(angle));
}

void ControllerComponent::checkBounds(b2Body* body) {
    const int screenWidth = Engine::width;
    const int screenHeight = Engine::height;

    b2Vec2 position = body->GetPosition();
    float newX = position.x;
    float newY = position.y;

    if (newX < 0) {
        newX = 0;
    }
    else if (newX > screenWidth) {
        newX = static_cast<float>(screenWidth);
    }

    if (newY < 0) {
        newY = 0;
    }
    else if (newY > screenHeight) {
        newY = static_cast<float>(screenHeight);
    }

    body->SetTransform(b2Vec2(newX, newY), body->GetAngle());
}
