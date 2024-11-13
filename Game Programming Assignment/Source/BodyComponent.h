#pragma once
#include "Component.h"
#include "tinyxml2.h"
#include <memory>

class BodyComponent : public Component {
public:
    BodyComponent(GameObject& owner, double x = 0, double y = 0, double vx = 0, double vy = 0, double angle = 0, int width = 50, int height = 50)
        : Component(owner), _x(x), _y(y), _vx(vx), _vy(vy), _angle(angle), width(width), height(height) {}

    static std::unique_ptr<Component> create(GameObject& parent, tinyxml2::XMLElement* element);

    double& x() { return _x; }
    double& y() { return _y; }
    double& vx() { return _vx; }
    double& vy() { return _vy; }
    double& angle() { return _angle; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setPosition(double x, double y) {
        _x = x;
        _y = y;
    }

    void setVelocity(double vx, double vy) {
        _vx = vx;
        _vy = vy;
    }

    void setAngle(double angle) {
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
    double _x, _y;   
    double _vx, _vy;
    double _angle;  
    int width;     
    int height;  
};

