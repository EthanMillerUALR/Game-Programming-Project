#include "HeroComponent.h"

#include "Engine.h"
#include "SpriteComponent.h"
#include "ComponentBullet.h"
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
    static bool mouseButtonPreviouslyPressed = false;

    // Spawn a bullet when the left mouse button is clicked
    if (Input::isMouseButtonDown(SDL_BUTTON_LEFT) && !mouseButtonPreviouslyPressed) {
        mouseButtonPreviouslyPressed = true; // Mark the button as pressed
        spawnBullet();
    }
    else if (!Input::isMouseButtonDown(SDL_BUTTON_LEFT)) {
        mouseButtonPreviouslyPressed = false; // Reset when the button is released
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
    // Get the hero's body
    b2Body* heroBody = parent().getBody();
    BodyComponent* heroBodyComponent = parent().getComponent<BodyComponent>();

    if (!heroBody || !heroBodyComponent) return;

    // Hero's position and angle
    b2Vec2 heroPosition = heroBody->GetPosition();
    float heroAngle = heroBody->GetAngle();

    // Get the hero's width and height from the BodyComponent
    float heroWidth = heroBodyComponent->getWidth();
    float heroHeight = heroBodyComponent->getHeight();

    // Calculate the center of the hero's body
    b2Vec2 heroCenter = heroPosition + b2Vec2(heroWidth / 2.0f, heroHeight / 2.0f);

    // Fixed distance from the hero to spawn the bullet (adjust as needed)
    float spawnDistance = 30.0f;

    // Calculate the direction vector the hero is facing
    b2Vec2 direction(cos(heroAngle), sin(heroAngle));

    // Multiply the direction vector by the spawn distance
    direction *= (heroWidth / 2.0f + spawnDistance);

    // Offset the spawn position by a fixed distance in the direction the hero is facing
    b2Vec2 bulletPosition = heroCenter + direction;

    // Create the bullet GameObject
    GameObject* bullet = new GameObject();

    // Add a BodyComponent for physics
    auto body = std::make_unique<BodyComponent>(*bullet, bulletPosition.x, bulletPosition.y, 20, 10, 0.0f, 0.0f, heroAngle);
    bullet->addComponent(std::move(body));

    // Add a SpriteComponent for visuals
    auto sprite = std::make_unique<SpriteComponent>(*bullet, "Bullet.png");
    bullet->addComponent(std::move(sprite));

    // Ensure the bullet's b2Body is initialized
    bullet->initializeBody(*Engine::getWorld(), bulletPosition.x, bulletPosition.y, 20, 10, b2_dynamicBody);

    // Set the bullet's angle to match the hero's angle (bullet will now face the same direction)
    b2Body* bulletBody = bullet->getBody();  // Retrieve the bullet's b2Body
    if (bulletBody) {
        bulletBody->SetTransform(bulletPosition, heroAngle);  // Apply the hero's angle to the bullet
    }

    // Set the bullet's velocity based on the hero's direction
    float bulletSpeed = 1000.0f; // Modify this value to adjust the bullet's speed
    b2Vec2 bulletVelocity(cos(heroAngle) * bulletSpeed, sin(heroAngle) * bulletSpeed);

    BodyComponent* bulletBodyComponent = bullet->getComponent<BodyComponent>();
    if (bulletBodyComponent) {
        bulletBodyComponent->setVelocity(bulletVelocity.x, bulletVelocity.y);  // Set velocity in the correct direction
    }

    // Add the bullet to the Engine
    Engine::scheduleAddGameObject(bullet);

    // Print the creation message
    std::cout << "The bullet object has been made." << std::endl;
}