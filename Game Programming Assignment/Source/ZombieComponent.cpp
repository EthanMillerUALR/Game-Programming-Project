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

int ZombieComponent::getDamage(){
    // Check if enough time has passed since the last attack
    if (getTime() < 1.0f) {
        // If the cooldown hasn't passed, no attack is allowed
        return 0;
    }
    else if (getTime() > 1.0f) {
        setTime(0);  // Reset timer
        std::cout << "Zombie is attacking! Damage: " << damage << std::endl;
        return damage;  // Return the damage value
    }
}

float ZombieComponent::getTime()
{
    return timeSinceAttack;
}

void ZombieComponent::setTime(float newTime)
{
    timeSinceAttack = newTime;
}

void ZombieComponent::takeDamage(int damage) {
    if (damage > 0) {
        std::cout << "Zombie health was " << health << std::endl;
        health -= damage;  // Reduce health
        std::cout << "But now, Zombie took " << damage << " damage. Health now: " << health << std::endl;
        SoundManager::playSound("zombieDamage");
    }
}
