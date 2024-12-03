#include "ComponentBullet.h"
#include <Box2D/Box2D.h>

BulletComponent::BulletComponent(GameObject& parent)
    : Component(parent) {
}

void BulletComponent::update(float deltaTime) {
    // This could be used for bullet-specific behavior, such as lifespan or collision checks.
}

