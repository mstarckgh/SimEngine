#pragma once

#include <vector>

#include "physics/RigidBody.h"


class PhysicsWorld {
public:
    void addBody(RigidBody* body);
    void simulate(float deltaTime);
    const std::vector<RigidBody*>& getBodies() const { return bodies; }


    // Setter
    void setCollisionCorrectionPercentage(float percentage) { collisionCorrectionPercentage = percentage; }
    void setCollisionCorrectionSlop(float slop) { collisionCorrectionSlop = slop; }

    // Getter
    float getCollisionCorrectionPercentage() const { return collisionCorrectionPercentage; }
    float getCollisionCorrectionSlop() const { return collisionCorrectionSlop; }

private:
    std::vector<RigidBody*> bodies;
    void handleCollisions();
    float collisionCorrectionPercentage = 1.0f; // Wie stark wird Korrektur angewendet
    float collisionCorrectionSlop = 0.0f;       // Toleranz ohne Korrektur 
};
