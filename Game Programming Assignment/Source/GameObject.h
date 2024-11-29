#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <stdexcept>
#include <iostream>
#include "Component.h"
#include <vector>
#include "box2d/box2d.h"  // Include the main Box2D header file

class GameObject {
public:
    GameObject() : userData(nullptr), body(nullptr) {}  // Default constructor

    // Add a component to the GameObject by type, using variadic arguments to forward them.
    template <typename T, typename... Args>
    void add(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
        components.push_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
    }

    // Add a pre-created component to the GameObject
    void addComponent(std::unique_ptr<Component> component) {
        components.push_back(std::move(component));
    }

    // Get a component by type as a raw pointer.
    template <typename T>
    T* get() {
        // Iterate through components and return the first match
        for (auto& comp : components) {
            T* castedComp = dynamic_cast<T*>(comp.get());
            if (castedComp) {
                return castedComp;
            }
        }
        return nullptr;
    }

    // Set the b2Body for this GameObject
    void setBody(b2Body* newBody) {
        body = newBody;
        // Set the userData for Box2D to point to this GameObject
        if (body) {
            body->SetUserData(this);  // Point to the GameObject itself
        }
    }

    // Get the b2Body associated with this GameObject
    b2Body* getBody() const {
        return body;
    }

    // Update all components.
    void update() {
        for (auto& comp : components) {
            comp->update();
        }
    }

    // Draw all components.
    void draw() {
        for (auto& comp : components) {
            comp->draw();
        }
    }

    // Set user data (for GameObject).
    void setUserData(void* data) {
        userData = data;
    }

    // Get user data (for GameObject).
    void* getUserData() const {
        return userData;
    }

private:
    void* userData;  // Pointer to hold any user-defined data (can be cast to appropriate type as needed)
    b2Body* body;    // Store the Box2D body directly in GameObject
    std::vector<std::unique_ptr<Component>> components; // Store components in a vector
};

