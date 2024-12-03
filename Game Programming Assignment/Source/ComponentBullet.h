#pragma once

#include "Component.h"

class BulletComponent : public Component {
public:
    BulletComponent(GameObject& parent);
    void update(float deltaTime);
};
