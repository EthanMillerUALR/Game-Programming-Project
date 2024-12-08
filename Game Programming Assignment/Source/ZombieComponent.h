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
    Aware,    // When the zombie is close to
};

class ZombieComponent : public Component {
public:
    ZombieComponent(GameObject& parent, double speed = 5.0, int health = 3, int damage = 3, float attackSpeed = 1.0)
        : Component(parent), slideSpeed(speed), goingRight(true), health(health), damage(damage), attackSpeed(attackSpeed), currentState(ZombieState::Unaware) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    void setSpeed(float speed);
    void setHealth(float initialHealth);
    void setDamage(float assignedDamage);
    void setAttackSpeed(float newAttackSpeed);
    void takeDamage(int damage);
    int getDamage();  // Function to return the current damage value
    float getTime();
    void setTime(float newTime);

    void update() override {
        b2Body* body = parent().getBody();
        if (!body) return;

        setTime((timeSinceAttack + Engine::getDeltaTime()));


        // Save initial x position as the bounds for movement (if not already set)
        if (initialX <= 0) {
            initialX = body->GetPosition().x;
            leftx = initialX;
            rightx = initialX + 20.0f;  // Adjust as needed
        }

        if (health <= 0) {
            Engine::scheduleDeleteGameObject(&parent());  // Schedule the GameObject for deletion
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
                    SoundManager::playSound("zombieGroan");
                }
                else if (distance >= 400.0f && currentState == ZombieState::Aware) {
                    currentState = ZombieState::Unaware;
                    body->SetTransform(body->GetPosition(), 0);
                    initialX = 0;

                }
            }
        }
        else {
            currentState = ZombieState::Unaware;
            body->SetTransform(body->GetPosition(), 0);
            initialX = 0;
        }

        

        // Handle state behavior
        switch (currentState) {
        case ZombieState::Unaware:
            handleUnawareState(body);
            break;
        case ZombieState::Aware:
            handleAwareState(body);
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

    float timeSinceAttack = 0;
    float attackSpeed;

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
        // Handle movement towards the hero when in the aware state
        GameObject* heroObject = Engine::findGameObjectComponent<HeroComponent>();
        if (!heroObject) return;  // If no hero, stop movement

        // Get the hero's position
        b2Vec2 heroPosition = heroObject->getBody()->GetPosition();
        b2Vec2 zombiePosition = body->GetPosition();

        // Calculate the direction to the hero
        b2Vec2 direction = heroPosition - zombiePosition;
        float distance = direction.Length();

        if (distance > 0) {
            // Normalize the direction to move at the zombie's speed
            direction.Normalize();
            direction *= slideSpeed;  // Adjust speed as needed

            // Set the zombie's velocity to move toward the hero
            body->SetLinearVelocity(direction);
            // Calculate the angle to the hero
            float angle = atan2f(direction.y, direction.x);  // Calculate angle in radians

            // Set the zombie's angle to always face the hero
            body->SetTransform(body->GetPosition(), angle);
        }

    }
};
