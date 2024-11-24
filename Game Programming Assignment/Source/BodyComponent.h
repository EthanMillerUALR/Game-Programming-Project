#pragma once
#include "Component.h"
#include "tinyxml2.h"
#include <memory>

class BodyComponent : public Component {
public:
    BodyComponent(GameObject& owner, float x = 0, float y = 0, int width = 50, int height = 50, float vx = 0, float vy = 0, float angle = 0)
        : Component(owner), _x(x), _y(y), _vx(vx), _vy(vy), _angle(angle), width(width), height(height) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    float& x() { return _x; }
    float& y() { return _y; }
    float& vx() { return _vx; }
    float& vy() { return _vy; }
    float& angle() { return _angle; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setPosition(float x, float y) {
        _x = x;
        _y = y;
    }

    void setVelocity(float vx, float vy) {
        _vx = vx;
        _vy = vy;
    }

    void setAngle(float angle) {
        _angle = angle;
    }

    void setDimensions(int w, int h) {
        width = w;
        height = h;
    }

    void update() override {
        _x += _vx;
        _y += _vy;
    }

    void draw() override {
    }

private:
    float _x, _y;   
    float _vx, _vy;
    float _angle;  
    int width;     
    int height;  
};

