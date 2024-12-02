#include "HeroComponent.h"

#include "Engine.h"
#include "SpriteComponent.h"
#include <tuple>
#include <utility>

std::unique_ptr<Component> HeroComponent::create(GameObject& parent, tinyxml2::XMLElement* element)
{
    double speed = element->IntAttribute("speed", 0);

    auto component = std::make_unique<HeroComponent>(parent, speed);
    return component;
}

void HeroComponent::setSpeed(float speed) {
    moveSpeed = static_cast<float>(speed);
}

void HeroComponent::update() {
    b2Body* body = parent().getBody();
    if (!body) return;

    double newSpeed = moveSpeed;

    bool upPressed = Input::isKeyDown(SDLK_w);
    bool downPressed = Input::isKeyDown(SDLK_s);
    bool leftPressed = Input::isKeyDown(SDLK_a);
    bool rightPressed = Input::isKeyDown(SDLK_d);

    b2Vec2 velocity(0, 0);
    if (upPressed) {
        velocity.y -= newSpeed;
    }
    if (downPressed) {
        velocity.y += newSpeed;
    }
    if (leftPressed) {
        velocity.x -= newSpeed;
    }
    if (rightPressed) {
        velocity.x += newSpeed;
    }

    body->SetLinearVelocity(velocity);

    mouseAngle(body);

    // Spawn a bullet when the left mouse button is clicked
    if (Input::isMouseButtonDown(SDL_BUTTON_LEFT)) {
        spawnBullet();
    }
}


void HeroComponent::mouseAngle(b2Body* body) {
    int mouseX;
    int mouseY;
    std::tie(mouseX, mouseY) = Input::getMousePosition();

    b2Vec2 position = body->GetPosition();
    double objectX = position.x;
    double objectY = position.y;

    double dx = mouseX - objectX;
    double dy = mouseY - objectY;

    double angle = atan2(dy, dx) * 180.0 / M_PI;

    body->SetTransform(body->GetPosition(), static_cast<float>(angle) * M_PI / 180.0f); // Convert to radians
}

void HeroComponent::spawnBullet() {
    //b2Body* heroBody = parent().getBody();
    //if (!heroBody) return;

    //b2Vec2 heroPosition = heroBody->GetPosition();

    //// Determine the direction towards the mouse position
    //int mouseX;
    //int mouseY;
    //std::tie(mouseX, mouseY) = Input::getMousePosition();
    //b2Vec2 mouseDirection(mouseX - heroPosition.x, mouseY - heroPosition.y);
    //mouseDirection.Normalize(); // Normalize the direction vector

    //// Set bullet's initial position and velocity
    //b2Vec2 bulletPosition = heroPosition + mouseDirection; // Spawn slightly away from the hero

    //// Create the bullet GameObject
    //auto bullet = std::make_unique<GameObject>("Bullet");

    //// Add components to the bullet (e.g., SpriteComponent, MovementComponent)
    //bullet->addComponent(std::make_unique<SpriteComponent>(*bullet, "bullet.png"));

    //// Add the bullet to the Engine
    //Engine::scheduleAddGameObject(bullet.release());

    std::cout << "This Will Eventually Make A Bullet" << std::endl;
}