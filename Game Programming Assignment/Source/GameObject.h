#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <stdexcept>
#include <iostream>
#include "Component.h"

template <typename T>
int getID() {
    return std::hash<std::type_index>()(std::type_index(typeid(T)));
}

class GameObject {
public:
    // Add a component to the GameObject by type, using variadic arguments to forward them.
    template <typename T, typename... Args>
    void add(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
        int id = getID<T>();
        components.push_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
    }

    // Add a pre-created component to the GameObject
    void addComponent(std::unique_ptr<Component> component) {
        components.push_back(std::move(component));
    }

    // Get a component by type as a raw pointer.
    template <typename T>
    T* get() {
        int id = getID<T>();
        for (auto& comp : components) {
            T* castedComp = dynamic_cast<T*>(comp.get());
            if (castedComp) {
                return castedComp;
            }
        }
        return nullptr;
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

private:
    std::vector<std::unique_ptr<Component>> components; // Changed to vector for simpler iteration
};

