//#include "BulletComponent.h"
//#include <cmath>
//
//BulletComponent::BulletComponent(GameObject& parent, double lifetime, double speed, double angle)
//    : Component(parent), lifetime(lifetime), speed(speed), angle(angle) {}
//
//void BulletComponent::update() {
//    lifetime -= Engine::getDeltaTime();
//    if (lifetime <= 0) {
//        //parent().destroy(); // Mark object for deletion
//        return;
//    }
//
//    auto body = parent().get<BodyComponent>();
//    if (body) {
//        double vx = speed * cos(angle);
//        double vy = speed * sin(angle);
//        body->setVelocity(vx, vy);
//        body->x() += vx;
//        body->y() += vy;
//    }
//}
//
//std::unique_ptr<Component> BulletComponent::create(GameObject& parent, tinyxml2::XMLElement* element) {
//    return std::make_unique<BulletComponent>(parent);
//}
