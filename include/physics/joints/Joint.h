#pragma once

#include <glm/glm.hpp>
#include "physics/RigidBody.h"


class Joint {
    public:
        Joint(RigidBody* parent, RigidBody* child)
            : parentBody(parent), childBody(child) {}
        
        // Virtuelle Destruktor für korrektes Aufräumen
        virtual ~Joint() = default;

        // Vor der Lösung: berechne Massen-inverse, Jacobian, Bias, etc.
        virtual void preSolve(float dt) = 0;
        
        // Löse den Impuls constraint-basiert
        virtual void solveImpulse() = 0;
        
        // Korrektur nach dem Schritt (Baumgarantie)
        virtual void postSolve() {}

    protected:
        RigidBody* parentBody;
        RigidBody* childBody; 
};