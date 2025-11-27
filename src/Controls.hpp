#pragma once
#include <GLFW/glfw3.h>

class Camera;
class MultiShaderForestScene;
class ModelScene;
class WhackAMoleScene;

class Controls {
public:
    Controls(GLFWwindow* window, Camera* camera);
    
    void setupCallbacks();
    void procInput(float deltaTime);
    void procSceneSwitch(int& currentSceneIdx, int sceneCount);
    void procBatteryToggle(MultiShaderForestScene* forestScene);
    void procSkyboxToggle(ModelScene* modelScene);
    void procEditModeToggle(MultiShaderForestScene* forestScene);
    void procWhackAMoleInput(WhackAMoleScene* whackAMoleScene);
    void procMouseHover(MultiShaderForestScene* forestScene);
    bool shouldClose() const;
    
    Camera* getCamera() const { return camera; }

private:
    GLFWwindow* window;
    Camera* camera;
    float lastX;
    float lastY;
    bool firstMouse;
    bool tabPressed;
    bool rMousePressed;
    bool lMousePressed;
    bool fPressed;
    bool tPressed;
    bool mPressed;
    
    void procCameraInput(float deltaTime);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};