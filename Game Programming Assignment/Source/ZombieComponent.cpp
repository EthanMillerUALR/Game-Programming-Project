#include "ZombieComponent.h"

std::unique_ptr<Component> ZombieComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double speed = element->DoubleAttribute("speed", 5.0);
    int health = element->IntAttribute("health", 3);
    int damage = element->IntAttribute("damage", 3);

    return std::make_unique<ZombieComponent>(parent, speed, health, damage);
}

void ZombieComponent::setSpeed(float speed) {
    slideSpeed = static_cast<float>(speed);
}

void ZombieComponent::setHealth(float initialHealth) {
    health = static_cast<float>(initialHealth);
}

void ZombieComponent::setDamage(float assignedDamage) {
    damage = static_cast<float>(assignedDamage);
}

int ZombieComponent::getDamage() const {
    return damage;  // Return the current damage value
}

void ZombieComponent::takeDamage(int damage) {
    std::cout << "Zombie health was " << health << std::endl;
    health -= damage;  // Reduce health
    std::cout << "But now, Zombie took " << damage << " damage. Health now: " << health << std::endl;
}
