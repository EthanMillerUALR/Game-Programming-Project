#pragma once
#include <iostream>
#include "Component.h"
#include "BodyComponent.h"
#include "Input.h"
#include "Engine.h"
#include <memory>
#include "tinyxml2.h"

class ZombieComponent : public Component {
public:
    ZombieComponent(GameObject& parent, double speed = 5.0, float leftx = 0, float rightx = Engine::width)
        : Component(parent), slideSpeed(speed), leftx(leftx), rightx(rightx), isActive(false), spaceWasPressed(false), goingRight(true) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    void setSpeed(float speed);

    void update() override {
        b2Body* body = parent().getBody();
        if (!body) return;

        bool spacePressed = Input::isKeyDown(SDLK_SPACE);

        // Toggle activation when space is pressed
        if (spacePressed && !spaceWasPressed) {
            isActive = !isActive;
            std::cout << (isActive ? "Movement activated" : "Movement deactivated") << std::endl;

            // If movement is deactivated, stop the object
            if (!isActive) {
                body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y)); // Stop horizontal movement
            }
        }

        spaceWasPressed = spacePressed;

        // Only update movement when active
        if (isActive) {
            b2Vec2 position = body->GetPosition();

            if (body) {
                // Check boundaries and determine direction
                if (position.x >= rightx) {
                    goingRight = false;
                }
                else if (position.x <= leftx) {
                    goingRight = true;
                }

                // Keep velocity consistent, boundaries will naturally limit movement
                double movement = slideSpeed;
                b2Vec2 velocity = body->GetLinearVelocity();
                velocity.x = goingRight ? movement : -movement;
                body->SetLinearVelocity(velocity);
            }
        }
    }

    void draw() override {
    }

private:
    double slideSpeed;     // Speed of movement
    bool isActive;         // Whether movement is active
    bool spaceWasPressed;  // Tracks space bar toggle state
    bool goingRight;       // Direction of movement
    int leftx;             // Left boundary
    int rightx;            // Right boundary
};
