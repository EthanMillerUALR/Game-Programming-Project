#include "ZombieComponent.h"

std::unique_ptr<Component> ZombieComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double speed = element->DoubleAttribute("speed", 5.0);
    int health = element->IntAttribute("health", 3);
    int damage = element->IntAttribute("damage", 3);
    double attackSpeed = element->DoubleAttribute("attackSpeed", 1.0);

    return std::make_unique<ZombieComponent>(parent, speed, health, damage, attackSpeed);
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
void ZombieComponent::setAttackSpeed(float newAttackSpeed) {
    attackSpeed = static_cast<float>(newAttackSpeed);
}

int ZombieComponent::getDamage(){
    // Check if enough time has passed since the last attack
    if (getTime() < attackSpeed) {
        // If the cooldown hasn't passed, no attack is allowed
        return 0;
    }
    else if (getTime() > attackSpeed) {
        setTime(0);
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
        health -= damage;
        SoundManager::playSound("zombieDamage");
    }
}
