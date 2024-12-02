#include "ComponentLibrary.h"
#include "BodyComponent.h" 
#include "HeroComponent.h"  
#include "ZombieComponent.h"   
#include "SpriteComponent.h"   


ComponentLibrary::ComponentLibrary()
{
    componentFactory["BodyComponent"] = BodyComponent::create;
    componentFactory["HeroComponent"] = HeroComponent::create;
    componentFactory["ZombieComponent"] = ZombieComponent::create;
    componentFactory["SpriteComponent"] = SpriteComponent::create;
}

std::unique_ptr<Component> ComponentLibrary::createComponent(const std::string& type, GameObject& parent, tinyxml2::XMLElement* element) {
    // Find the component factory function based on the type
    auto it = componentFactory.find(type);

    if (it != componentFactory.end()) {
        // Pass both parent (for component binding) and element (for XML data)
        std::unique_ptr<Component> component = it->second(parent, element);
        return component;
    }

    // Return nullptr if the component type was not found
    return nullptr;
}





