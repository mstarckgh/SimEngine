#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "graphics/Mesh.h"
#include "graphics/MeshLibrary.h"
#include "graphics/Renderer.h"
#include "graphics/Camera.h"
#include "graphics/loadTexture.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsWorld.h"


bool rotating = false;
bool panning = false;
Camera camera(glm::vec3(0.0f, 2.0f, 5.0f));

void mouse_button_callback(GLFWwindow* window, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            rotating = true;
        } else if (action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            rotating = false;
            camera.resetFirstMouse();
        }
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            panning = true;
        } else if (action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            panning = false;
            camera.resetFirstMouse();
        }
    }
}

void mouse_callback(GLFWwindow*, double xpos, double ypos) {
    if (rotating)
        camera.updateMouse(xpos, ypos);
    else if (panning) {
        static double lastX = xpos, lastY = ypos;
        double dx = xpos - lastX;
        double dy = lastY - ypos; // Y invertiert wie üblich
        camera.pan(dx, -dy);
        lastX = xpos;
        lastY = ypos;
    }
}

void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
    camera.zoom(static_cast<float>(yoffset));
}

int main() {

    int windowWidth = 1920;
    int windowHeight = 1080;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "SimEngine", nullptr, nullptr);
    glfwMakeContextCurrent(window);


    // Input Config
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        glViewport(0, 0, width, height);
    });
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Renderer erstellen und mit Camera koppeln
    Renderer renderer;
    renderer.setCamera(&camera);
    renderer.setLightDirection(glm::normalize(glm::vec3(-1.0f, -4.0f, -1.0f)));
    renderer.setViewportSize(windowWidth, windowHeight);

    //unsigned int woodTex = loadTexture("assets/hazel_wood.jpg");
    
    // MeshLibrary erstellen und rectangle laden
    MeshLibrary meshLib;
    meshLib.loadDefaultMeshes();

    const Mesh* rect = meshLib.get("rectangle");
    const Mesh* cube = meshLib.get("cube");


    // ==== World Building ====
    PhysicsWorld world;
    world.setCollisionCorrectionPercentage(1.0f);
    world.setCollisionCorrectionSlop(0.0f);

    // Starrer Boden
    RigidBody floor(cube, {0, 0, 0});
    floor.setScale({40, 0.1f, 40});
    floor.getMaterial().color = glm::vec3(0.5f, 0.5f, 0.5f);
    floor.setIsStatic(true);
    floor.setRestitution(0.5f);
    floor.setDynamicFriction(0.5f);
    floor.setStaticFriction(0.5f);
    world.addBody(&floor);

    // Klotz 1
    RigidBody box_a(cube, {0, 1.0f, 0});
    box_a.getMaterial().color = glm::vec3(1.0f, 0.5f, 0.2f);    // Orange
    box_a.setIsStatic(false);
    world.addBody(&box_a);

    // Klotz 2
    RigidBody box_b(cube, {0, 5.0f, 0});
    box_b.getMaterial().color = glm::vec3(0.5f, 1.0f, 0.2f);    // Grün
    box_b.setIsStatic(false);
    box_b.setMass(1.0f);
    //world.addBody(&box_b);

    // Simulations und Zeit Prameter
    const float dt = 0.0001f;
    float timeAccumulator = 0.0f;
    float lastFrame = glfwGetTime();

    const glm::vec3 gravity(0.0f, -9.81f, 0.0f);

    // Custom forces
    const glm::vec3 force_one(50.0f, 50.0f, 0.0f);
    const glm::vec3 force_two(-50.0f, 50.0f, 0.0f);

    // Custom torques
    const glm::vec3 torque_one(10.0f, 0, 0);

    float start_time = 2.0f;
    float planned_duration = 0.2f;
    float duration = 0.0f;


    // FPS Calc
    float lastFpsUpdate = glfwGetTime();
    int frameCount = 0;



    // ==== Main Loop ====
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float frameTime = currentTime - lastFrame;
        lastFrame = currentTime;
        frameCount++;

        timeAccumulator += frameTime;

        // FPS Aktualisieren
        if(currentTime - lastFpsUpdate >= 1.0f) {
            float fps = frameCount / (currentTime - lastFpsUpdate);
            std::string title = "SimEngine - FPS: " + std::to_string(static_cast<int>(fps));
            glfwSetWindowTitle(window, title.c_str());
        }

        
        // ==== Physics Calculations ====
        while (timeAccumulator >= dt) {
            // apply Gravity to every Object
            for (RigidBody* body : world.getBodies())
                body->applyForceAtPoint(gravity * body->getMass(), glm::vec3(0.0f));
            
            if (currentTime >= start_time && duration <= planned_duration){
                duration += dt;
            }
            world.simulate(dt);
            
            timeAccumulator -= dt;
        }


        // ==== Adjust Camera ====
        camera.updateKeyboard(window, frameTime);

        // ==== SHADOW PASS ====
        std::vector<std::pair<const Mesh*, glm::mat4>> shadowMeshes;
        for (RigidBody* body : world.getBodies())
            shadowMeshes.emplace_back(body->getMesh(), body->getModelMatrix());

        renderer.renderShadowPass(shadowMeshes);

        // === RENDER PASS ===
        renderer.beginFrame();
        for (RigidBody* body : world.getBodies())
            renderer.draw(*body->getMesh(), body->getModelMatrix(), body->getMaterial());
        renderer.endFrame();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
