#include "physics/Collider.h"


AABB::AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint)
    : min(minPoint), max(maxPoint) {}

bool AABB::intersects(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}

glm::vec3 AABB::getOverlap(const AABB& other) const {
    glm::vec3 overlap;
    
    overlap.x = std::min(max.x, other.max.x) - std::max(min.x, other.min.x);
    overlap.y = std::min(max.y, other.max.y) - std::max(min.y, other.min.y);
    overlap.z = std::min(max.z, other.max.z) - std::max(min.z, other.min.z);

    // Schutz vor negativer überlappung
    return glm::max(overlap, glm::vec3(0.0f));
}

void AABB::getMinOverlap(const AABB& other, glm::vec3& overlapNormal, float& returnOverlap) {   
    // Overlap der zwei Objekte berechnen
    glm::vec3 overlap = getOverlap(other);

    // Richtung mit geringsten overlap bestimmen
    overlapNormal = glm::vec3(0.0f);
    returnOverlap = std::numeric_limits<float>::max();

    // X
    if (overlap.x > 0.0f && overlap.x < returnOverlap) {
        returnOverlap = overlap.x;
        overlapNormal = glm::vec3(1, 0, 0);
    }
    // Y
    if (overlap.y > 0.0f && overlap.y < returnOverlap) {
        returnOverlap = overlap.y;
        overlapNormal = glm::vec3(0, 1, 0);
    }
    // Z
    if (overlap.z > 0.0f && overlap.z < returnOverlap) {
        returnOverlap = overlap.z;
        overlapNormal = glm::vec3(0, 0, 1);
    }
}