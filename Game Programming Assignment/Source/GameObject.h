#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <stdexcept>
#include <iostream>
#include "Component.h"
#include <vector>
#include "box2d/box2d.h"  // Include the main Box2D header file
#include "BodyComponent.h"

class GameObject {
public:
    GameObject() : body(nullptr) {}  // Default constructor

    // Add a component to the GameObject by type, using variadic arguments to forward them.
    template <typename T, typename... Args>
    void add(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
        components.push_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
        std::cout << "Added component of type: " << typeid(T).name() << std::endl;
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

    template <typename T>
    T* getComponent() {
        // Iterate through the components and check if the type matches
        for (auto& comp : components) {
            T* castedComp = dynamic_cast<T*>(comp.get());
            if (castedComp) {
                return castedComp;  // Return the component if found
            }
        }
        return nullptr;  // Return nullptr if no component of the requested type is found
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

    // Set the b2Body for this GameObject
    void setBody(b2Body* newBody) {
        body = newBody;
        // Set the userData for Box2D to point to this GameObject
        if (body) {
            body->GetUserData().pointer = uintptr_t(this);
            std::cout << "b2Body successfully assigned to GameObject." << std::endl;
        }
        else {
            //std::cout << "Failed to assign b2Body to GameObject." << std::endl;
        }
    }

    // Get the b2Body associated with this GameObject
    b2Body* getBody() const {
        return body;
    }

    void initializeBody(b2World& world, float x, float y, float width, float height, b2BodyType type) {
        if (body != nullptr) {
            // Body is already initialized
            return;
        }

        b2BodyDef bodyDef;
        bodyDef.type = type;
        bodyDef.position.Set(x, y);

        // Create the body in the Box2D world
        body = world.CreateBody(&bodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(width / 2.0f, height / 2.0f);  // Assuming the width and height are the dimensions of a box

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        // Add the fixture to the body
        body->CreateFixture(&fixtureDef);

        // Now set the userData using the setBody function
        setBody(body);  // Call the setBody function to assign the userData
    }

private:
    b2Body* body = nullptr;    // Store the Box2D body directly in GameObject
    std::vector<std::unique_ptr<Component>> components; // Store components in a vector
};

