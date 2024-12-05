#include "GameContactListener.h"
#include "Engine.h"
#include "GameObject.h"
#include "HeroComponent.h"
#include "ZombieComponent.h"
#include "ComponentBullet.h"
#include <iostream>


void GameContactListener::BeginContact(b2Contact* contact) {
    // Get the bodies involved in the collision
    auto bodyA = contact->GetFixtureA()->GetBody();
    auto bodyB = contact->GetFixtureB()->GetBody();

    // Get the GameObjects associated with the bodies
    GameObject* gameObjectA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
    GameObject* gameObjectB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);

    // Check if the game objects are valid
    if (gameObjectA && gameObjectB) {
        // Check if one has a HeroComponent and the other has a ZombieComponent
        if (gameObjectA->getComponent<HeroComponent>() && gameObjectB->getComponent<ZombieComponent>()) {
            std::cout << "The hero has collided with a zombie!" << std::endl;
            HeroComponent* heroComponent = gameObjectA->getComponent<HeroComponent>();
            ZombieComponent* zombieComponent = gameObjectB->getComponent<ZombieComponent>();
            if (heroComponent && zombieComponent) {
                int damage = zombieComponent->getDamage();  // Get the damage value from the zombie
                heroComponent->takeDamage(damage);  // Hero takes damage
            }
        }
        else if (gameObjectA->getComponent<ZombieComponent>() && gameObjectB->getComponent<HeroComponent>()) {
            std::cout << "The hero has collided with a zombie!" << std::endl;
            HeroComponent* heroComponent = gameObjectB->getComponent<HeroComponent>();
            ZombieComponent* zombieComponent = gameObjectA->getComponent<ZombieComponent>();
            if (heroComponent && zombieComponent) {
                int damage = zombieComponent->getDamage();  // Get the damage value from the zombie
                heroComponent->takeDamage(damage);  // Hero takes damage
            }
        }
        else if (gameObjectA->getComponent<BulletComponent>() && gameObjectB->getComponent<ZombieComponent>()) {
            Engine::scheduleDeleteGameObject(gameObjectA); // Delete the bullet
            ZombieComponent* zombieComponent = gameObjectB->getComponent<ZombieComponent>();
            if (zombieComponent) {
                zombieComponent->takeDamage(1);  // Zombie takes 1 damage
            }
        }
        else if (gameObjectA->getComponent<ZombieComponent>() && gameObjectB->getComponent<BulletComponent>()) {
            Engine::scheduleDeleteGameObject(gameObjectB); // Delete the bullet
            ZombieComponent* zombieComponent = gameObjectA->getComponent<ZombieComponent>();
            if (zombieComponent) {
                zombieComponent->takeDamage(1);  // Zombie takes 1 damage
            }
        }
        else if (gameObjectA->getComponent<BulletComponent>() && gameObjectB->getComponent<HeroComponent>()) {
        }
        else if (gameObjectA->getComponent<HeroComponent>() && gameObjectB->getComponent<BulletComponent>()) {
        }
        else if (gameObjectA->getComponent<BulletComponent>() && gameObjectB->getComponent<BulletComponent>()) {
        }
        else if (gameObjectA->getComponent<BulletComponent>()) {
            Engine::scheduleDeleteGameObject(gameObjectA); // Delete the bullet
        }
        else if (gameObjectB->getComponent<BulletComponent>()) {
            Engine::scheduleDeleteGameObject(gameObjectB); // Delete the bullet
        }
    }
}

void GameContactListener::EndContact(b2Contact* contact) {
    // Optional: Handle end of contact if needed
}

void GameContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{   // Get the bodies involved in the collision
    auto bodyA = contact->GetFixtureA()->GetBody();
    auto bodyB = contact->GetFixtureB()->GetBody();

    // Get the GameObjects associated with the bodies
    GameObject* gameObjectA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
    GameObject* gameObjectB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);

    if (gameObjectA && gameObjectB) {
        if (gameObjectA->getComponent<HeroComponent>() && gameObjectB->getComponent<ZombieComponent>()) {
            HeroComponent* heroComponent = gameObjectA->getComponent<HeroComponent>();
            ZombieComponent* zombieComponent = gameObjectB->getComponent<ZombieComponent>();
            if (heroComponent && zombieComponent) {
                int damage = zombieComponent->getDamage();  // Get the damage value from the zombie
                heroComponent->takeDamage(damage);  // Hero takes damage
            }
        }
        else if (gameObjectA->getComponent<ZombieComponent>() && gameObjectB->getComponent<HeroComponent>()) {
            HeroComponent* heroComponent = gameObjectB->getComponent<HeroComponent>();
            ZombieComponent* zombieComponent = gameObjectA->getComponent<ZombieComponent>();
            if (heroComponent && zombieComponent) {
                int damage = zombieComponent->getDamage();  // Get the damage value from the zombie
                heroComponent->takeDamage(damage);  // Hero takes damage
            }
        }
    }
}
