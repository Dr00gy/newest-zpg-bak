#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Controls.hpp"
#include "Camera.hpp"
#include "scenes/MultiShaderForestScene.hpp"
#include "scenes/ModelScene.hpp"

Controls::Controls(GLFWwindow* window, Camera* camera)
    : window(window),
      camera(camera),
      firstMouse(true),
      lastX(400.0f),
      lastY(300.0f),
      tabPressed(false),
      rightMousePressed(false),
      leftMousePressed(false),
      fPressed(false),
      tPressed(false),
      mPressed(false) {}

void Controls::setupCallbacks() {
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Controls::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Controls* controls = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    if (!controls) return;
    
    if (!controls->rightMousePressed) {
        controls->firstMouse = true;
        return;
    }
    
    if (controls->firstMouse) {
        controls->lastX = static_cast<float>(xpos);
        controls->lastY = static_cast<float>(ypos);
        controls->firstMouse = false;
    }
    
    float xoffset = static_cast<float>(xpos) - controls->lastX;
    float yoffset = controls->lastY - static_cast<float>(ypos);
    
    controls->lastX = static_cast<float>(xpos);
    controls->lastY = static_cast<float>(ypos);
    
    controls->camera->processMouseMovement(xoffset, yoffset);
}

void Controls::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Controls* controls = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    if (controls) {
        controls->camera->processMouseScroll(static_cast<float>(yoffset));
    }
}

void Controls::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Controls* controls = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    if (!controls) return;
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            controls->rightMousePressed = true;
            controls->firstMouse = true;
        } else if (action == GLFW_RELEASE) {
            controls->rightMousePressed = false;
            controls->firstMouse = true;
        }
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            controls->leftMousePressed = true;
        } else if (action == GLFW_RELEASE) {
            controls->leftMousePressed = false;
        }
    }
}

void Controls::processCameraInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(CameraMovement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboard(CameraMovement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->processKeyboard(CameraMovement::DOWN, deltaTime);
}

void Controls::processInput(float deltaTime) {
    processCameraInput(deltaTime);
}

void Controls::processSceneSwitching(size_t& currentSceneIndex, size_t sceneCount) {
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!tabPressed) {
            currentSceneIndex = (currentSceneIndex + 1) % sceneCount;
            tabPressed = true;
        }
    } else {
        tabPressed = false;
    }

    for (int key = GLFW_KEY_1; key <= GLFW_KEY_9; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            int index = key - GLFW_KEY_1;
            if (index < static_cast<int>(sceneCount)) {
                currentSceneIndex = index;
            }
        }
    }
}

void Controls::processFlashlightToggle(MultiShaderForestScene* forestScene) {
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (!fPressed) {
            if (forestScene) {
                forestScene->toggleFlashlight();
            }
            fPressed = true;
        }
    } else {
        fPressed = false;
    }
}

void Controls::processSkyboxToggle(ModelScene* modelScene) {
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        if (!tPressed) {
            if (modelScene) {
                modelScene->toggleSkyboxMode();
            }
            tPressed = true;
        }
    } else {
        tPressed = false;
    }
}

void Controls::processEditModeToggle(MultiShaderForestScene* forestScene) {
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!mPressed) {
            if (forestScene) {
                forestScene->toggleEditMode();
                std::cout << "Edit mode: " << (forestScene->isEditMode() ? "CREATION" : "DELETION") << std::endl;
            }
            mPressed = true;
        }
    } else {
        mPressed = false;
    }
    
    if (forestScene) {
        static bool wasLeftPressed = false;
        bool isLeftPressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        
        if (isLeftPressed && !wasLeftPressed) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            
            forestScene->handleMouseClick(xpos, ypos, width, height);
        }
        
        wasLeftPressed = isLeftPressed;
    }
}

void Controls::processMouseHover(MultiShaderForestScene* forestScene) {
    if (!forestScene) return;
    
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    forestScene->updateMouseHover(xpos, ypos, width, height);
}

bool Controls::shouldClose() const {
    return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}