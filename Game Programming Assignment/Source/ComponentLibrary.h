#pragma once
#include "Component.h"
#include <unordered_map>
#include <functional>
#include "tinyxml2.h"
#include "Engine.h"

class ComponentLibrary {
public:

    ComponentLibrary();
    ~ComponentLibrary() = default;

    std::unique_ptr<Component> createComponent(const std::string& type, GameObject& parent, tinyxml2::XMLElement* element);


private:

    std::unordered_map<std::string, std::function<std::unique_ptr<Component>(GameObject&, tinyxml2::XMLElement*)>> componentFactory;

};