#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


class Camera {
public:
    Camera(glm::vec3 position);
    glm::mat4 getViewMatrix() const;

    void updateMouse(double xpos, double ypos);
    void updateKeyboard(GLFWwindow* window, float deltaTime);
    void resetFirstMouse();
    void pan(double dx, double dy);
    void zoom(float yoffset);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    float yaw;
    float pitch;

    float lastX, lastY;
    bool firstMouse;

    float speed;
    float sensitivity;

    float zoomSpeed = 1.0f;

    void updateVectors();
};
