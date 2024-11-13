#pragma once
#include "Component.h"
#include "BodyComponent.h"
#include "Input.h"
#include "GameObject.h"
#include <iostream>
#include <memory>
#include "tinyxml2.h"
#include "Engine.h"

class ControllerComponent : public Component {
public:
    ControllerComponent(GameObject& parent, double moveSpeed = 5.0)
        : Component(parent), moveSpeed(moveSpeed) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    void setSpeed(int speed);

    void update() override;

    void draw() override {}

private:
    double moveSpeed;  // Speed of the movement

    void checkBounds(BodyComponent* body);

};



