#include "ControllerComponent.h"

#include "Engine.h"
#include <tuple>
#include <utility>
#include "SpriteComponent.h"
#include "tinyxml2.h"
#include "ComponentLibrary.h"
#include "BodyComponent.h"
#include <SDL2/SDL.h>

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

    double newSpeed = (moveSpeed * Engine::getDeltaTime()) / 2;

    bool upPressed = Input::isKeyDown(SDLK_w);    // Move up
    bool downPressed = Input::isKeyDown(SDLK_s);  // Move down
    bool leftPressed = Input::isKeyDown(SDLK_a);  // Move left
    bool rightPressed = Input::isKeyDown(SDLK_d); // Move right

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

    if (Input::isMouseButtonDown(SDL_BUTTON_LEFT)) {
        createBullet(body->x(), body->y(), body->getWidth(), body->getHeight());
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
    double angle = atan2(dy, dx) * 180.0 / M_PI;

    // Set the angle in the body
    body->setAngle(angle);
}

void ControllerComponent::createBullet(double playerX, double playerY, double playerWidth, double playerHeight) {
    //// Construct a tinyxml2 structure for the components
    //tinyxml2::XMLDocument doc;

    //// Create the root "Object" element for the bullet
    //tinyxml2::XMLElement* bulletObject = doc.NewElement("Object");

    //// Add BodyComponent
    //tinyxml2::XMLElement* bodyElement = doc.NewElement("BodyComponent");
    //bodyElement->SetAttribute("x", playerX + playerWidth + 10); // Offset from player
    //bodyElement->SetAttribute("y", playerY + (playerHeight / 2.0) + 10);
    //bodyElement->SetAttribute("vx", 0.0); // Set velocity later based on direction
    //bodyElement->SetAttribute("vy", 0.0);
    //bodyElement->SetAttribute("angle", 0);
    //bodyElement->SetAttribute("width", 20);  // Example bullet size
    //bodyElement->SetAttribute("height", 10);
    //bulletObject->InsertEndChild(bodyElement);

    //// Add SpriteComponent
    //tinyxml2::XMLElement* spriteElement = doc.NewElement("SpriteComponent");
    //spriteElement->SetAttribute("texture", "bullet.png");
    //bulletObject->InsertEndChild(spriteElement);

    //// Add components using the ComponentLibrary
    //GameObject* bullet = new GameObject(); // Use raw pointer for the bullet object
    //bool componentAdded = false; // Track if any component was added

    //// Use existing component creation logic to add components
    //for (tinyxml2::XMLElement* compElement = bulletObject->FirstChildElement(); compElement != nullptr; compElement = compElement->NextSiblingElement()) {
    //    const std::string compType = compElement->Name();
    //    Component* component = ComponentLibrary().createComponent(compType, *bullet, compElement);
    //    if (component) {
    //        bullet->addComponent(component); // Pass the raw pointer
    //        componentAdded = true; // Mark that a component was successfully added
    //    }
    //}

    //// Add the bullet to the engine
    //if (componentAdded) {
    //    Engine::addGameObject(bullet); // Pass the raw pointer
    //    SDL_Log("Bullet GameObject added to the Engine");
    //}
    //else {
    //    delete bullet; // Clean up if no components were added
    //    SDL_Log("Failed to create bullet components");
    //}

    SDL_Log("To Be Worked On");

    //Get rid of smart pointers, go back to regular pointers
    //Try to not make an xml dynamically, just add the things more directly
    //Try to use toAdd and toDelete to handle the bullet instead of adding directly
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
