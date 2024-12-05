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
    ZombieComponent(GameObject& parent, double speed = 5.0, int health = 3, int damage = 3)
        : Component(parent), slideSpeed(speed), goingRight(true), health(health), damage(damage) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    void setSpeed(float speed);
    void setHealth(float initialHealth);
    void setDamage(float assignedDamage);
    void takeDamage(int damage);
    int getDamage() const;  // Function to return the current damage value

    void update() override {
        b2Body* body = parent().getBody();
        if (!body) return;

        bool spacePressed = Input::isKeyDown(SDLK_SPACE);

        // Save initial x position as the bounds for movement (if not already set)
        if (initialX < 0) {
            initialX = body->GetPosition().x;
            leftx = initialX;
            rightx = initialX + 50.0f;  // Adjust as needed
        }

        if (health <= 0) {
            Engine::scheduleDeleteGameObject(&parent());  // Schedule the GameObject for deletion
            std::cout << "Zombie has died" << std::endl;
        }

        // Handle movement logic
        b2Vec2 position = body->GetPosition();

        if (body) {
            // Check boundaries and determine direction
            if (position.x >= rightx) {
                goingRight = false;
            }
            else if (position.x <= leftx) {
                goingRight = true;
            }

            // Keep velocity consistent
            double movement = slideSpeed;
            b2Vec2 velocity = body->GetLinearVelocity();
            velocity.x = goingRight ? movement : -movement;
            body->SetLinearVelocity(velocity);
        }
    }

    void draw() override {
    }

private:
    double slideSpeed;     // Speed of movement
    bool goingRight;       // Direction of movement
    float initialX;        // Initial x position of the zombie
    int leftx;             // Left boundary
    int rightx;            // Right boundary
    int health;
    int damage;
};
