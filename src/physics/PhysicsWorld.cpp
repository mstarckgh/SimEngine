#include "physics/PhysicsWorld.h"


void PhysicsWorld::addBody(RigidBody* body) {
    bodies.push_back(body);
}

void PhysicsWorld::simulate(float dt) {
    // 1) Velocity-Integration
    for (auto& body : bodies) {
        body->integrate(dt);
    }

    // 2) Collision-Solver
    handleCollisions();
}

void PhysicsWorld::handleCollisions() {
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            RigidBody* a = bodies[i];
            RigidBody* b = bodies[j];

            if (a->getIsStatic() && b->getIsStatic()) continue;

            if (a->getAABB().intersects(b->getAABB())) {
                // Berechnung der Kollisionsachse mit der geringsten Überlappung
                glm::vec3 collisionNormal(0.0f);
                float overlap;
                a->getAABB().getMinOverlap(b->getAABB(), collisionNormal, overlap);

                // Relativgeschwiwndigkeit entlang der Stoßnormalen
                glm::vec3 relative_velocity = b->getLinearVelocity() - a->getLinearVelocity();
                float velocityAlongNormal = glm::dot(relative_velocity, collisionNormal);
                if(velocityAlongNormal > 0.0f) continue; // Objekte bewegen sich auseinander

                // Elsatizität bestimmen
                float e = std::min(a->getRestitution(), b->getRestitution());

                // Effektive Massen berehnen
                float inverseMassA = a->getIsStatic() ? 0.0f : 1.0f / a->getMass();
                float inverseMassB = b->getIsStatic() ? 0.0f : 1.0f / b->getMass();

                // Impuls skalieren
                float impulseMagnitude = -(1+e) * velocityAlongNormal;
                impulseMagnitude /= (inverseMassA + inverseMassB);

                // Impuls anwenden
                glm::vec3 impulse = impulseMagnitude * collisionNormal;
                //std::cout << impulse.x << " " << impulse.y << " " << impulse.z << std::endl;

                if(!a->getIsStatic()) a->setLinearVelocity(a->getLinearVelocity() - inverseMassA * impulse);
                if(!b->getIsStatic()) b->setLinearVelocity(b->getLinearVelocity() + inverseMassB * impulse);


                // ==== Reibung berechnen ====

                // Relativgeschwindigkeit nach dem Normalenimpuls
                glm::vec3 rv = b->getLinearVelocity() - a->getLinearVelocity();

                // Tangentialrichtung in Kontaktfläche berechnen
                glm::vec3 tangent = rv - glm::dot(rv, collisionNormal) * collisionNormal;
                if (glm::length2(tangent) > 1e-8f)
                    tangent = glm::normalize(tangent);
                else
                    tangent = glm::vec3(0.0f);

                //  maximaler Haftimpuls
                float mu_static = std::max(a->getStaticFriction(), b->getStaticFriction());
                
                float tangentImpulse = -glm::dot(rv, tangent) / (inverseMassA + inverseMassB);

                float jn = std::abs(impulseMagnitude);

                float max_static_impulse = mu_static * jn;    // Haftgrenze

                // Vegleich Haft- und Gleitreibung
                if (glm::length2(tangent) < 1e-8f) {
                    tangentImpulse = 0.0f;
                } else if (std::abs(tangentImpulse) < max_static_impulse) {
                    // tangent_impulse bleibt so
                } else {
                    // Gleitreibung
                    float mu_dynamic = std::max(a->getDynamicFriction(), b->getDynamicFriction());
                    tangentImpulse = glm::clamp(tangentImpulse, -mu_dynamic*jn, mu_dynamic*jn);
                }

                // Reibungsimpuls anwenden
                glm::vec3 frictionImpulse = tangentImpulse * tangent;
                if (!a->getIsStatic())
                    a->setLinearVelocity(a->getLinearVelocity() - inverseMassA * frictionImpulse);
                if (!b->getIsStatic())
                    b->setLinearVelocity(b->getLinearVelocity() + inverseMassB * frictionImpulse);


                // ==== Korrektur berechnen ====
                float correctedOverlap = std::max(overlap-collisionCorrectionSlop, 0.0f);
                glm::vec3 penetrationCorrection = (correctedOverlap / (inverseMassA+inverseMassB)) * collisionCorrectionPercentage * collisionNormal;

                //std::cout << penetrationCorrection.x << " " << penetrationCorrection.y << " " << penetrationCorrection.z << std::endl;


                // Korrektur anwenden
                if (!a->getIsStatic())
                    a->setPosition(a->getPosition() - inverseMassA * penetrationCorrection);
                if (!b->getIsStatic())
                    b->setPosition(b->getPosition() + inverseMassB * penetrationCorrection);
            }
        }
    }
}
