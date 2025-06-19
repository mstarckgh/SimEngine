#pragma once
#include <glm/glm.hpp>
#include "graphics/Mesh.h"
#include "graphics/Material.h"

class Camera;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void setCamera(Camera* cam);
    void setLightDirection(const glm::vec3& dir);
    void setViewportSize(int width, int height);
    
    void beginFrame();
    void draw(const Mesh& mesh, const glm::mat4& modelMatrix, const Material& material);
    void endFrame();

    void initShadowMap();
    void renderShadowPass(const std::vector<std::pair<const Mesh*, glm::mat4>>& sceneMeshes);

private:
    unsigned int shaderProgram;
    glm::vec3 lightDirection;
    Camera* camera;
    int viewportWidth = 800;
    int viewportHeight = 800;

    // Shadom Map Parameter
    const unsigned int SHADOW_WIDTH = 1024*8, SHADOW_HEIGHT = 1024*8;

    unsigned int depthMapFBO;
    unsigned int depthMap;
    glm::mat4 lightSpaceMatrix;

    unsigned int shadowShader;
};

