#include "SlideComponent.h"

std::unique_ptr<Component> SlideComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double speed = element->DoubleAttribute("speed", 5.0);
    int leftx = element->IntAttribute("leftx", 0);
    int rightx = element->IntAttribute("rightx", Engine::width);
    return std::make_unique<SlideComponent>(parent, speed, leftx, rightx);
}

void SlideComponent::setSpeed(float speed) {
    slideSpeed = static_cast<float>(speed);
}