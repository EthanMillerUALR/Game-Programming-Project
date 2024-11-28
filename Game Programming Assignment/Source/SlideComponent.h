#pragma once
#include <iostream>
#include "Component.h"
#include "BodyComponent.h"
#include "Input.h"
#include "Engine.h"
#include <memory>
#include "tinyxml2.h"

class SlideComponent : public Component {
public:
    SlideComponent(GameObject& parent, double speed = 5.0, float leftx = 0, float rightx = Engine::width)
        : Component(parent), slideSpeed(speed), leftx(leftx), rightx(rightx), isActive(false), spaceWasPressed(false), goingRight(true) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    void setSpeed(float speed);

    void update() override {
        bool spacePressed = Input::isKeyDown(SDLK_SPACE);

        // Toggle activation when space is pressed
        if (spacePressed && !spaceWasPressed) {
            isActive = !isActive;
            std::cout << (isActive ? "Movement activated" : "Movement deactivated") << std::endl;
        }

        spaceWasPressed = spacePressed;

        // Only update movement when active
        if (isActive) {
            auto body = parent().get<BodyComponent>();
            if (body) {
                // Check boundaries
                if (body->x() >= rightx) {
                    goingRight = false;
                }
                else if (body->x() <= leftx) {
                    goingRight = true;
                }

                // Keep velocity consistent, boundaries will naturally limit movement
                double movement = slideSpeed * Engine::getDeltaTime();
                body->setVelocity(goingRight ? movement : -movement, body->vy());
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
