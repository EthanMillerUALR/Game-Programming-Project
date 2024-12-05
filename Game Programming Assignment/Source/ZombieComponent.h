#pragma once
#include <iostream>
#include "Component.h"
#include "BodyComponent.h"
#include "HeroComponent.h"
#include "Input.h"
#include "Engine.h"
#include <memory>
#include "tinyxml2.h"

enum class ZombieState {
    Unaware,  // Default state
    Aware,    // When the zombie detects the hero
    Active    // When the zombie attacks the hero
};

class ZombieComponent : public Component {
public:
    ZombieComponent(GameObject& parent, double speed = 5.0, int health = 3, int damage = 3)
        : Component(parent), slideSpeed(speed), goingRight(true), health(health), damage(damage), currentState(ZombieState::Unaware) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    void setSpeed(float speed);
    void setHealth(float initialHealth);
    void setDamage(float assignedDamage);
    void takeDamage(int damage);
    int getDamage() const;  // Function to return the current damage value

    void update() override {
        b2Body* body = parent().getBody();
        if (!body) return;


        // Save initial x position as the bounds for movement (if not already set)
        if (initialX < 0) {
            initialX = body->GetPosition().x;
            leftx = initialX;
            rightx = initialX + 50.0f;  // Adjust as needed
        }

        // Check proximity to HeroComponent
        GameObject* heroObject = Engine::findGameObjectComponent<HeroComponent>();
        if (heroObject) {
            // Get the position of the hero and calculate distance
            b2Body* heroBody = heroObject->getBody();
            if (heroBody) {
                b2Vec2 heroPosition = heroBody->GetPosition();
                b2Vec2 zombiePosition = body->GetPosition();

                float distance = (heroPosition - zombiePosition).Length();
                if (distance < 400.0f && currentState == ZombieState::Unaware) {
                    // Transition to the Aware state if the hero is within 400 pixels
                    currentState = ZombieState::Aware;
                    std::cout << "Zombie is now aware of the hero!" << std::endl;
                }
            }
        }

        if (health <= 0) {
            Engine::scheduleDeleteGameObject(&parent());  // Schedule the GameObject for deletion
            std::cout << "Zombie has died" << std::endl;
        }

        // Handle state behavior
        switch (currentState) {
        case ZombieState::Unaware:
            handleUnawareState(body);
            break;
        case ZombieState::Aware:
            handleAwareState(body);
            break;
        case ZombieState::Active:
            // Active state behavior can be added later
            break;
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
    ZombieState currentState;  // Current state of the zombie

    void handleUnawareState(b2Body* body) {
        // Sliding logic in the unaware state
        b2Vec2 position = body->GetPosition();
        if (position.x >= rightx) {
            goingRight = false;
        }
        else if (position.x <= leftx) {
            goingRight = true;
        }

        double movement = slideSpeed;
        b2Vec2 velocity = body->GetLinearVelocity();
        velocity.x = goingRight ? movement : -movement;
        body->SetLinearVelocity(velocity);
    }

    void handleAwareState(b2Body* body) {
        // In the aware state, stop sliding and look at the hero
        b2Vec2 position = body->GetPosition();
        GameObject* heroObject = Engine::findGameObjectComponent<HeroComponent>();
        if (heroObject) {
            b2Body* heroBody = heroObject->getBody();
            if (heroBody) {
                b2Vec2 heroPosition = heroBody->GetPosition();
                // Calculate direction to look at the hero
                b2Vec2 direction = heroPosition - position;
                float angle = atan2(direction.y, direction.x);
                body->SetTransform(position, angle);  // Rotate zombie to face the hero
                // Stop movement
                b2Vec2 velocity = body->GetLinearVelocity();
                velocity.x = 0.0f;
                body->SetLinearVelocity(velocity);
            }
        }
    }

    void handleActiveState(b2Body* body) {
    }
};
