#pragma once

class View {
public:
    View(double centerX = 0, double centerY = 0) : viewX(centerX), viewY(centerY) {}

    void setViewCenter(double x, double y) {
        viewX = x;
        viewY = y;
    }

    void moveView(double dx, double dy) {
        viewX += dx;
        viewY += dy;
    }

    double getViewX() const { return viewX; }
    double getViewY() const { return viewY; }

    // Transform a point to view space
    void transformPoint(double& x, double& y) const {
        x -= viewX;
        y -= viewY;
    }

    // Transform a rectangle (top-left corner and dimensions)
    void transformRect(double& x, double& y, double width, double height) const {
        x -= viewX;
        y -= viewY;
    }

private:
    double viewX; // X-coordinate of the view's center
    double viewY; // Y-coordinate of the view's center
};
