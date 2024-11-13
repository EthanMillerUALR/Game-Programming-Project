#include "ComponentLibrary.h"
#include "BodyComponent.h" 
#include "ControllerComponent.h"  
#include "SlideComponent.h"   
#include "SpriteComponent.h"   


ComponentLibrary::ComponentLibrary()
{
    componentFactory["BodyComponent"] = BodyComponent::create;
    componentFactory["ControllerComponent"] = ControllerComponent::create;
    componentFactory["SlideComponent"] = SlideComponent::create;
    componentFactory["SpriteComponent"] = SpriteComponent::create;
}

std::unique_ptr<Component> ComponentLibrary::createComponent(const std::string& type, GameObject& parent, tinyxml2::XMLElement* element) {
    auto it = componentFactory.find(type);
    if (it != componentFactory.end()) {
        std::unique_ptr<Component> component = it->second(parent, element);  // Pass only element, not parent
        return component;
    }
    return nullptr;
}




