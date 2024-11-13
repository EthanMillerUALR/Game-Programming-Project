#include "BodyComponent.h"
#include <SDL2/SDL.h>


std::unique_ptr<Component> BodyComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double x = element->DoubleAttribute("x", 0.0);
    double y = element->DoubleAttribute("y", 0.0);
    double vx = element->DoubleAttribute("vx", 0.0);
    double vy = element->DoubleAttribute("vy", 0.0);
    double angle = element->DoubleAttribute("angle", 0.0);
    int width = element->IntAttribute("width", 50);
    int height = element->IntAttribute("height", 50);

    return std::make_unique<BodyComponent>(parent, x, y, vx, vy, width, height, angle);
}
