#pragma once
#include "Component.h"
#include "tinyxml2.h"
#include <memory>
#include "Box2D/Box2D.h" // For b2Body, b2Vec2, etc.

class BodyComponent : public Component {
public:
    BodyComponent(GameObject& owner, float x = 0, float y = 0, int width = 50, int height = 50, float vx = 0, float vy = 0, float angle = 0)
        : Component(owner), width(width), height(height) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    // Functions to synchronize with the b2Body
    float getX() const;
    float getY() const;
    float getAngle() const;
    b2Vec2 getVelocity() const;

    void setPosition(float x, float y);
    void setVelocity(float vx, float vy);
    void setAngle(float angle);

    void setDimensions(int w, int h) {
        width = w;
        height = h;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setBodyType(b2BodyType type) { bodyType = type; }
    b2BodyType getBodyType() const { return bodyType; }

    void update() override;
    void draw() override {}

private:
    int width;   // Width of the object
    int height;  // Height of the object  
    b2BodyType bodyType = b2_dynamicBody; // Default body type
};

