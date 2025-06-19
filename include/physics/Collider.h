#pragma once
#include <glm/glm.hpp>

class AABB {
public:
    AABB() = default;
    AABB(const glm::vec3& min, const glm::vec3& max);

    bool intersects(const AABB& other) const;
    glm::vec3 getOverlap(const AABB& other) const;
    void getMinOverlap(const AABB& other, glm::vec3& overlapNormal, float& overlap);

    glm::vec3 min;
    glm::vec3 max;
};