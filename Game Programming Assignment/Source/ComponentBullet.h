#pragma once
#include "Component.h"
#include "Box2D/Box2D.h"  // For b2Vec2

class BulletComponent : public Component {
public:
    BulletComponent(GameObject& parent, float lifetime, float speed, float directionX, float directionY);

    void update() override;
    void draw() override;

    ~BulletComponent() override;

private:
    float _lifetime;         // Time remaining for the bullet
    float _speed;            // Speed of the bullet
    b2Vec2 _direction;       // Normalized direction vector
};
