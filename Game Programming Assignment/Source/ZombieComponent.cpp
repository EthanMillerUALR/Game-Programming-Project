#include "ZombieComponent.h"

std::unique_ptr<Component> ZombieComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double speed = element->DoubleAttribute("speed", 5.0);
    int leftx = element->IntAttribute("leftx", 0);
    int rightx = element->IntAttribute("rightx", Engine::width);
    int health = element->DoubleAttribute("health", 3.0);

    return std::make_unique<ZombieComponent>(parent, speed, leftx, rightx, health);
}

void ZombieComponent::setSpeed(float speed) {
    slideSpeed = static_cast<float>(speed);
}
void ZombieComponent::setHealth(float initialHealth) {
    health = static_cast<float>(initialHealth);
}

void ZombieComponent::takeDamage(int damage) {
    health -= damage;  // Reduce health
    std::cout << "Zombie took " << damage << " damage. Health now: " << health << std::endl;
}