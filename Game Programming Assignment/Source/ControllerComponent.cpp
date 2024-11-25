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
    auto body = parent().get<BodyComponent>();
    if (!body) return;

    body->setVelocity(0, 0);
    std::string message;

    double newSpeed = (moveSpeed * Engine::getDeltaTime()) / 2;

    bool upPressed = Input::isKeyDown(SDLK_UP);
    bool downPressed = Input::isKeyDown(SDLK_DOWN);
    bool leftPressed = Input::isKeyDown(SDLK_LEFT);
    bool rightPressed = Input::isKeyDown(SDLK_RIGHT);

    if (upPressed) {
        body->setVelocity(body->vx(), -newSpeed);
    }
    if (downPressed) {
        body->setVelocity(body->vx(), newSpeed);
    }
    if (leftPressed) {
        body->setVelocity(-newSpeed, body->vy());
    }
    if (rightPressed) {
        body->setVelocity(newSpeed, body->vy());
    }

    if (!message.empty()) {
        std::cout << message << std::endl;
    }
    mouseAngle(body);


    checkBounds(body);
}


void ControllerComponent::mouseAngle(BodyComponent* body) {
    // Get mouse position

    int mouseX;
    int mouseY;
    std::tie(mouseX, mouseY) = Input::getMousePosition();

    // Get object's current position
    double objectX = body->x() + (body->getWidth() / 2); // Center of the object
    double objectY = body->y() + (body->getHeight() / 2);

    // Calculate the difference in position
    double dx = mouseX - objectX;
    double dy = mouseY - objectY;

    // Calculate the angle in degrees
    double angle = (atan2(dy, dx) * 180.0 / M_PI) - 180;

    // Set the angle in the body
    body->setAngle(angle);
}

void ControllerComponent::checkBounds(BodyComponent* body) {
    const int screenWidth = Engine::width;
    const int screenHeight = Engine::height;

    double newX = body->x() + body->vx();
    double newY = body->y() + body->vy();

    if (newX < 0) {
        newX = 0;
    }
    else if (newX + body->getWidth() > screenWidth) {
        newX = screenWidth - body->getWidth();
    }

    if (newY < 0) {
        newY = 0;
    }
    else if (newY + body->getHeight() > screenHeight) {
        newY = screenHeight - body->getHeight();
    }

    body->x() = newX;
    body->y() = newY;
}
