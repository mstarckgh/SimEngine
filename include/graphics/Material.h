#pragma once
#include <glm/glm.hpp>

struct Material {
    glm::vec3 color = glm::vec3(1.0f);
    unsigned int textureID = 0;
    bool useTexture = false;
};