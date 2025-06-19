#include "graphics/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

Camera::Camera(glm::vec3 startPos) 
    : position(startPos), yaw(-90.0f), pitch(0.0f),
      speed(3.0f), sensitivity(0.1f), lastX(0.0), lastY(0.0), firstMouse(true)
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::updateKeyboard(GLFWwindow* window, float dt) {
    float velocity = speed * dt;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += right * velocity;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) position += up * velocity;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) position -= up * velocity;
}

void Camera::updateMouse(double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateVectors();
}

void Camera::resetFirstMouse() {
    firstMouse = true;
}

void Camera::pan(double dx, double dy){
    position -= right * static_cast<float>(dx) * 0.01f;
    position += up    * static_cast<float>(dy) * 0.01f;
}

void Camera::zoom(float yoffset){
    position += front * yoffset * zoomSpeed;
}

void Camera::updateVectors() {
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(dir);
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up    = glm::normalize(glm::cross(right, front));
}
