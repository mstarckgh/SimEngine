#include "physics/RigidBody.h"

RigidBody::RigidBody(const Mesh* mesh, const glm::vec3& position, const glm::quat& orientation)
    : mesh(mesh)
    , position(position)
    , orientation(glm::normalize(orientation))
    , scale(1.0f)
    , isStatic(false)
    , mass(1.0f)
    , inertia(computeInertiaFromMesh())
    , restitution(1.0f)
    , linearVelocity(0.0f)
    , linearAcceleration(0.0f)
    , staticFriction(0.0f)
    , dynamicFriction(0.0f)
    , netForce(0.0f)
    , angularVelocity(0.0f)
    , angularAcceleration(0.0f)
    , netTorque(0.0f)
{}


// ==== SETTER ====
void RigidBody::setPosition(const glm::vec3& pos) {
    position = pos;
}

void RigidBody::setOrientation(const glm::quat& q) {
    orientation = glm::normalize(q);
}

void RigidBody::setEulerRotation(const glm::vec3& eulerAngles) {
    orientation = glm::normalize(glm::quat(eulerAngles));
}

void RigidBody::setScale(const glm::vec3& s) {
    scale = s;
    inertia = computeInertiaFromMesh();
}

void RigidBody::setIsStatic(bool s) {
    isStatic = s;
}

void RigidBody::setLinearVelocity(const glm::vec3& v) {
    linearVelocity = v;
}

void RigidBody::setLinearAcceleration(const glm::vec3& a) {
    linearAcceleration = a;
}

void RigidBody::setAngularVelocity(const glm::vec3& w) {
    angularVelocity = w;
}

void RigidBody::setAngularAcceleration(const glm::vec3& dw) {
    angularAcceleration = dw;
}

void RigidBody::setMass(float m) {
    mass = m;
    inertia = computeInertiaFromMesh();
}

void RigidBody::setInertia(glm::vec3 J) {
    inertia = glm::mat3(
        J.x,  0.0f, 0.0f,
        0.0f, J.y,  0.0f,
        0.0f, 0.0f, J.z
    );
}

void RigidBody::setRestitution(float r) {
    restitution = r;
}

void RigidBody::setDynamicFriction(float my) {
    dynamicFriction = my;
}

void RigidBody::setStaticFriction(float my) {
    staticFriction = my;
}


// ==== GETTER ====

AABB RigidBody::getAABB() const {
    return AABB(position-(0.5f*scale), position+(0.5f*scale));
}

float RigidBody::getMass() const {
    return mass;
}

glm::vec3 RigidBody::getLinearVelocity() const {
    return linearVelocity;
}

glm::vec3 RigidBody::getLinearAcceleration() const {
    return linearAcceleration;
}

glm::vec3 RigidBody::getAngularVelocity() const {
    return angularVelocity;
}

glm::vec3 RigidBody::getAngularAcceleration() const {
    return angularAcceleration;
}

glm::vec3 RigidBody::getNetForce() const {
    return netForce;
}

glm::vec3 RigidBody::getPosition() const {
    return position;
}

glm::quat RigidBody::getOrientation() const {
    return orientation;
}
    
glm::quat RigidBody::getOrientationInverse() const {
    return glm::conjugate(orientation);
}

glm::vec3 RigidBody::getScale() const {
    return scale;
}

const Mesh* RigidBody::getMesh() const {
    return mesh;
}

bool RigidBody::getIsStatic() const {
    return isStatic;
}


// ==== Functions =====

glm::mat4 RigidBody::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model *= glm::toMat4(orientation);
    model = glm::scale(model, scale);
    return model;
}

void RigidBody::applyForceAtPoint(const glm::vec3& force, const glm::vec3& point) {
    if (isStatic) return;
    netForce  += force;
    netTorque += glm::cross(point-position, force); 
}

void RigidBody::applyTorque(const glm::vec3& torque){
    if(!isStatic) {
        netTorque += torque;
    }
}

void RigidBody::integrate(float dt) {
    if(isStatic) return;

    integrateLinearVelocity(dt);
    integrateAngularVelocity(dt);
    integratePosition(dt);
    integrateOrientation(dt);

    resetForces();
    resetTorque();
}

void RigidBody::integrateLinearVelocity(float dt) {
    linearAcceleration = netForce / mass;
    linearVelocity += linearAcceleration * dt;
}

void RigidBody::integrateAngularVelocity(float dt) {
    // Body-Space Inverse:
    glm::mat3 inverseInertia = glm::inverse(inertia);

    // World-Space Inverse:
    glm::mat3 R         = glm::toMat3(orientation);
    glm::mat3 invI_world= R * inverseInertia * glm::transpose(R);

    angularAcceleration = invI_world * netTorque;
    angularVelocity += angularAcceleration * dt;
}

void RigidBody::integratePosition(float dt) {
    position += linearVelocity * dt;
}

void RigidBody::integrateOrientation(float dt) {
    glm::quat w{0.0f, angularVelocity};
    orientation += 0.5f * w * orientation * dt;
    orientation = glm::normalize(orientation);
}

void RigidBody::resetForces() {
    netForce = glm::vec3(0.0f);
}

void RigidBody::resetTorque() {
    netTorque = glm::vec3(0.0f);
}

const glm::mat3 RigidBody::computeInertiaFromMesh() const {
    
    // Masseneigenschaften initialisieren
    glm::mat3 I(0.0f);
    float V = 0.0f;

    // Lambda für das Volumen eines Tetraeders (v0, v1, v2, Ursprung)
    auto signedTetraVolume = [](const glm::vec3& v0,
                                 const glm::vec3& v1,
                                 const glm::vec3& v2){
        return glm::dot(v0, glm::cross(v1, v2)) * (1.0f / 6.0f);
    };

    // Mesh CPU-Daten Abgreifen
    const auto& verts = mesh->getCpuVertices();
    const auto& idx   = mesh->getCpuIndices();

    for (size_t i=0; i+2<idx.size(); i+=3) {
        const glm::vec3& v0 = verts[idx[i+0]].position;
        const glm::vec3& v1 = verts[idx[i+1]].position;
        const glm::vec3& v2 = verts[idx[i+2]].position;

        float dV = signedTetraVolume(v0, v1, v2);
        V += dV;

        // ∫x² dV = dV/10 * (Σ xi² + Σ xi xj)
        auto F2 = [&](float a0, float a1, float a2) {
            return (a0*a0 + a1*a1 + a2*a2
                    + a0*a1 + a1*a2 + a2*a0) * (dV / 10.0f);
        };
        I[0][0] += F2(v0.y, v1.y, v2.y) + F2(v0.z, v1.z, v2.z);
        I[1][1] += F2(v0.z, v1.z, v2.z) + F2(v0.x, v1.x, v2.x);
        I[2][2] += F2(v0.x, v1.x, v2.x) + F2(v0.y, v1.y, v2.y);

        // ∫xy dV = dV/20 * Σ(2 xi yi + xi yj + xj yi)
        auto F3 = [&](float a0,float a1,float a2, float b0,float b1,float b2) {
            float s = 2*a0*b0 + a0*b1 + a0*b2
                + 2*a1*b1 + a1*b0 + a1*b2
                + 2*a2*b2 + a2*b0 + a2*b1;
            return s * (dV / 20.0f);
        };
        float xy = F3(v0.x,v1.x,v2.x, v0.y,v1.y,v2.y);
        float yz = F3(v0.y,v1.y,v2.y, v0.z,v1.z,v2.z);
        float zx = F3(v0.z,v1.z,v2.z, v0.x,v1.x,v2.x);

        I[0][1] -= xy; I[1][0] = I[0][1];
        I[1][2] -= yz; I[2][1] = I[1][2];
        I[2][0] -= zx; I[0][2] = I[2][0];
    }

    // Auf die gewünschte Gesamtmasse skalieren
    I *= (mass/V);

    // Gewünschte Skalierung anpassen
    glm::mat3 S(
        scale.x, 0.0f,      0.0f,
        0.0f,     scale.y,   0.0f,
        0.0f,    0.0f,      scale.z
    );

    std::cout << "Computed Inertia" << std::endl;
    // Rückgabe der Hauptträgheitsmomente
    return S * I * S;
}