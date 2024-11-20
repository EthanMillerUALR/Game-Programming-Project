#include "ControllerComponent.h"

std::unique_ptr<Component> ControllerComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double speed = element->IntAttribute("speed", 0);

    auto component = std::make_unique<ControllerComponent>(parent, speed);
    return component;
}

void ControllerComponent::setSpeed(int speed) {
    moveSpeed = static_cast<double>(speed);
}

void ControllerComponent::update() {
    auto body = parent().get<BodyComponent>();
    if (!body) return;

    body->setVelocity(0, 0);
    std::string message;

    bool upPressed = Input::isKeyDown(SDLK_UP);
    bool downPressed = Input::isKeyDown(SDLK_DOWN);
    bool leftPressed = Input::isKeyDown(SDLK_LEFT);
    bool rightPressed = Input::isKeyDown(SDLK_RIGHT);

    if (upPressed) {
        body->setVelocity(body->vx(), -moveSpeed);
    }
    if (downPressed) {
        body->setVelocity(body->vx(), moveSpeed);
    }
    if (leftPressed) {
        body->setVelocity(-moveSpeed, body->vy());
    }
    if (rightPressed) {
        body->setVelocity(moveSpeed, body->vy());
    }

    if (!message.empty()) {
        std::cout << message << std::endl;
    }

    checkBounds(body);
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
