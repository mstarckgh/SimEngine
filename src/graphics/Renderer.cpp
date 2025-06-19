#include "graphics/Renderer.h"
#include "graphics/shader_utils.h"
#include "graphics/Camera.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

Renderer::Renderer()
    : shaderProgram(loadShaders("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl")),
      camera(nullptr) {
    initShadowMap();
    shadowShader = loadShaders("shaders/shadow_depth.vs", "shaders/shadow_depth.fs");
}

Renderer::~Renderer() {}

void Renderer::setCamera(Camera* cam) {
    camera = cam;
}

void Renderer::setLightDirection(const glm::vec3& dir) {
    std::cout << "SET lightDir: " << dir.x << " " << dir.y << " " << dir.z << "\n";
    lightDirection = glm::normalize(dir);
}

void Renderer::setViewportSize(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
    glViewport(0, 0, width, height);
}

void Renderer::beginFrame() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const Mesh& mesh, const glm::mat4& modelMatrix, const Material& material) {
    if (!camera) {
        std::cerr << "[Error] No camera set for Renderer!\n";
        return;
    }

    glm::vec3 tester = mesh.getCpuVertices()[0].position; 
    std::cout << modelMatrix[0][3] << " " << modelMatrix[1][3] << " " << modelMatrix[2][3] << " " << std::endl;
    
    // Aktuelles Seitenverhältnis berechnen
    float aspect = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);

    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    glm::mat4 mvp = projection * view * modelMatrix;

    glUseProgram(shaderProgram);

    // TEXTURE
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.textureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), material.useTexture);

    // Fallback-Farbe (wird nur genutzt wenn !useTexture)
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(material.color));

    // Shadow Map aktivieren (TEXTURE1 empfohlen)
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 1);

    // Licht-Raum-Matrix setzen
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_LightSpaceMatrix"),
                    1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    // Lichtparameter setzen
    glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"),  lightDirection.x, lightDirection.y, lightDirection.z);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);

    // Matritzen setzen
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_MVP"), 1, GL_FALSE, glm::value_ptr(mvp));

    mesh.bind();
    mesh.draw();
}

void Renderer::endFrame() {
    // z.B. Debug-Overlays später
}

void Renderer::initShadowMap() {
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::cout << "Shadom map init ok\n";
}

void Renderer::renderShadowPass(const std::vector<std::pair<const Mesh*, glm::mat4>>& sceneMeshes) {
    float near_plane = 1.0f, far_plane = 20.0f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(-lightDirection * 10.0f, glm::vec3(0.0f), glm::vec3(0, 1, 0));
    lightSpaceMatrix = lightProjection * lightView;

    glUseProgram(shadowShader);
    glUniformMatrix4fv(glGetUniformLocation(shadowShader, "u_LightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (auto& [mesh, model] : sceneMeshes) {
        glUniformMatrix4fv(glGetUniformLocation(shadowShader, "u_Model"), 1, GL_FALSE, glm::value_ptr(model));
        mesh->bind();
        mesh->draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Viewport zurücksetzen auf Fenstergröße
    glViewport(0, 0, viewportWidth, viewportHeight);
}