#ifndef GAME_CONTACT_LISTENER_H
#define GAME_CONTACT_LISTENER_H

#include <box2d/box2d.h>

class GameContactListener : public b2ContactListener {
public:
    // Called when two fixtures begin to touch
    void BeginContact(b2Contact* contact) override;

    // Called when two fixtures cease to touch
    void EndContact(b2Contact* contact) override;

    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};

#endif // GAME_CONTACT_LISTENER_H