#include "ComponentBullet.h"
#include "GameObject.h"
#include "Engine.h"
#include <iostream>

BulletComponent::BulletComponent(GameObject& parent, float lifetime, float speed, float directionX, float directionY)
    : Component(parent), _lifetime(lifetime), _speed(speed), _direction(directionX, directionY) {}

void BulletComponent::update() {
    // Reduce lifetime
    _lifetime -= Engine::getDeltaTime();
    if (_lifetime <= 0) {
        // Schedule the bullet for removal
        Engine::scheduleDeleteGameObject(&parent());
        return;
    }

    // Move the bullet based on its direction and speed
    auto* body = parent().getBody();  // Get the physics body from the GameObject
    if (body) {
        b2Vec2 velocity = _direction;
        velocity.Normalize();  // Ensure the direction vector is a unit vector
        velocity *= _speed;    // Scale by speed
        body->SetLinearVelocity(velocity);  // Set the body's velocity
    }
}

void BulletComponent::draw() {
}

BulletComponent::~BulletComponent() {
}


