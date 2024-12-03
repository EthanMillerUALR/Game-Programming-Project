#pragma once
#include "Component.h"
#include "BodyComponent.h"
#include "Input.h"
#include "GameObject.h"
#include <iostream>
#include <memory>
#include "tinyxml2.h"
#include "Engine.h"

class HeroComponent : public Component {
public:
    HeroComponent(GameObject& parent, float moveSpeed = 5.0)
        : Component(parent), moveSpeed(moveSpeed) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    void setSpeed(float speed);
    void mouseAngle(b2Body* body);
    void spawnBullet();  // New function to spawn a bullet

    void update() override;

    void draw() override {}

private:
    double moveSpeed;  // Speed of the movement

};


