#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/quaternion.hpp>

#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "physics/Collider.h"

class RigidBody {
public:
    RigidBody(const Mesh* mesh, const glm::vec3& position, const glm::quat& orientation = glm::quat());


    // Setter
    void setPosition(const glm::vec3& pos);
    void setOrientation(const glm::quat& q);
    void setEulerRotation(const glm::vec3& eulerAngles);
    void setScale(const glm::vec3& scale);
    void setIsStatic(bool isStatic);
    void setLinearVelocity(const glm::vec3& v);
    void setLinearAcceleration(const glm::vec3& a);
    void setAngularVelocity(const glm::vec3& v);
    void setAngularAcceleration(const glm::vec3& a);
    void setMass(float m);
    void setInertia (glm::vec3 J);
    void setRestitution(float r);
    void setDynamicFriction(float my);
    void setStaticFriction(float my);


    // Getter
    glm::vec3 getPosition() const;
    glm::quat getOrientation() const;
    glm::quat getOrientationInverse() const;
    glm::vec3 getScale() const;
    const Mesh* getMesh() const;
    bool getIsStatic() const;
    Material& getMaterial() { return material; }
    const Material& getMaterial() const { return material; }
    glm::mat4 getModelMatrix() const;
    AABB getAABB() const;
    float getMass() const;
    glm::mat3 getInertia() const { return inertia; }
    glm::vec3 getLinearVelocity() const;
    glm::vec3 getLinearAcceleration() const;
    glm::vec3 getAngularVelocity() const;
    glm::vec3 getAngularAcceleration() const;
    glm::vec3 getNetForce() const;
    float getRestitution() const { return restitution; }
    float getStaticFriction() const { return staticFriction; }
    float getDynamicFriction() const { return dynamicFriction; }


    void applyForceAtPoint(const glm::vec3& force, const glm::vec3& point);
    void applyTorque(const glm::vec3& torque);
    void integrate(float dt);
    void integrateLinearVelocity(float dt);
    void integratePosition(float dt);
    void integrateAngularVelocity(float dt);
    void integrateOrientation(float dt);
    void resetForces();
    void resetTorque();
    const glm::mat3 computeInertiaFromMesh() const;

private:
    bool isStatic;
    Material material;
    const Mesh* mesh;
    glm::vec3 scale;
    float mass = 1.0f;
    glm::mat3 inertia = glm::mat3(1.0f);

    
    
    float restitution = 1.0f;                   // Elastizität des Objektes ∈[0;1]
    float staticFriction = 0.0f;                // Haftreibungs µ
    float dynamicFriction = 0.0f;               // Gleitreibungs µ

    glm::vec3 position;
    glm::vec3 linearVelocity = glm::vec3(0.0f);
    glm::vec3 linearAcceleration = glm::vec3(0.0f);
    glm::vec3 netForce = glm::vec3(0.0f);

    glm::quat orientation;
    glm::vec3 angularVelocity = glm::vec3(0.0f);
    glm::vec3 angularAcceleration = glm::vec3(0.0f);
    glm::vec3 netTorque = glm::vec3(0.0f);
};
