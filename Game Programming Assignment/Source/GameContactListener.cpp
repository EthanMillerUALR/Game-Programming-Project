#include "GameContactListener.h"
#include "GameObject.h"
#include "ControllerComponent.h"
#include "SlideComponent.h"
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
        // Check if one has a ControllerComponent and the other has a SlideComponent
        if (gameObjectA->getComponent<ControllerComponent>() && gameObjectB->getComponent<SlideComponent>()) {
            std::cout << "The hero has collided with a zombie!" << std::endl;
        }
        else if (gameObjectB->getComponent<ControllerComponent>() && gameObjectA->getComponent<SlideComponent>()) {
            std::cout << "The hero has collided with a zombie!" << std::endl;
        }
    }
}

void GameContactListener::EndContact(b2Contact* contact) {
    // Optional: Handle end of contact if needed
}