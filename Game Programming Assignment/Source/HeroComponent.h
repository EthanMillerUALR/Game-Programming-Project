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
    HeroComponent(GameObject& parent, float moveSpeed = 5.0, int health = 15)
        : Component(parent), moveSpeed(moveSpeed), health(health) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    void setSpeed(float speed);
    void setHealth(float initialHealth);
    void takeDamage(int damage);
    void mouseAngle(b2Body* body);
    void spawnBullet();  // New function to spawn a bullet

    void update() override;

    void draw() override {}

private:
    double moveSpeed;  // Speed of the movement
    int health;

};



