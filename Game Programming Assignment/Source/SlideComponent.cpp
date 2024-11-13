#include "SlideComponent.h"

std::unique_ptr<Component> SlideComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double speed = element->DoubleAttribute("speed", 5.0);
    int left = element->IntAttribute("leftx", 0);
    int right = element->IntAttribute("rightx", Engine::width);
    return std::make_unique<SlideComponent>(parent, speed, left, right);;
}

void SlideComponent::setSpeed(int speed) {
    slideSpeed = static_cast<double>(speed);
}