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
    SlideComponent(GameObject& parent, double speed = 5.0, int leftx = 0, int rightx = Engine::width)
        : Component(parent), slideSpeed(speed), leftx(leftx), rightx(rightx), isActive(false), spaceWasPressed(false), goingRight(true) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    void setSpeed(int speed);

    void update() override {
        bool spacePressed = Input::isKeyDown(SDLK_SPACE);

        if (spacePressed && !spaceWasPressed) {
            isActive = !isActive;
            std::cout << (isActive ? "Movement activated" : "Movement deactivated") << std::endl;
        }

        spaceWasPressed = spacePressed;

        if (isActive) {
            auto body = parent().get<BodyComponent>();
            if (body) {
                if (body->x() >= rightx) {
                    goingRight = false;
                    body->x() = rightx;
                }
                else if (body->x() <= leftx) {
                    goingRight = true;
                    body->x() = leftx;
                }

                if (goingRight) {
                    body->x() += slideSpeed;  // Use slideSpeed consistently
                }
                else {
                    body->x() -= slideSpeed;  // Use slideSpeed consistently
                }
            }
        }
    }

    void draw() override {
    }

private:
    double slideSpeed;
    bool isActive;
    bool spaceWasPressed;
    bool goingRight;
    int leftx;
    int rightx;
};
