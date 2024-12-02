#include "ZombieComponent.h"

std::unique_ptr<Component> ZombieComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double speed = element->DoubleAttribute("speed", 5.0);
    int leftx = element->IntAttribute("leftx", 0);
    int rightx = element->IntAttribute("rightx", Engine::width);
    return std::make_unique<ZombieComponent>(parent, speed, leftx, rightx);
}

void ZombieComponent::setSpeed(float speed) {
    slideSpeed = static_cast<float>(speed);
}