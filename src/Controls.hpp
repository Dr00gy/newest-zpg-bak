#pragma once
#include <GLFW/glfw3.h>

class Camera;
class MultiShaderForestScene;
class ModelScene;

class Controls {
public:
    Controls(GLFWwindow* window, Camera* camera);
    
    void setupCallbacks();
    void processInput(float deltaTime);
    void processSceneSwitching(size_t& currentSceneIndex, size_t sceneCount);
    void processFlashlightToggle(MultiShaderForestScene* forestScene);
    void processSkyboxToggle(ModelScene* modelScene);
    void processEditModeToggle(MultiShaderForestScene* forestScene);
    void processMouseHover(MultiShaderForestScene* forestScene);
    bool shouldClose() const;

    Camera* getCamera() const { return camera; }

private:
    GLFWwindow* window;
    Camera* camera;
    float lastX;
    float lastY;
    bool firstMouse;
    bool tabPressed;
    bool rightMousePressed;
    bool leftMousePressed;
    bool fPressed;
    bool tPressed;
    bool mPressed;
    
    void processCameraInput(float deltaTime);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};