#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Controls.hpp"
#include "Camera.hpp"
#include "scenes/MultiShaderForestScene.hpp"
#include "scenes/ModelScene.hpp"
#include "scenes/WhackAMoleScene.hpp"

Controls::Controls(GLFWwindow* window, Camera* camera)
    : window(window),
      camera(camera),
      firstMouse(true),
      lastX(400.0f),
      lastY(300.0f),
      tabPressed(false),
      rMousePressed(false),
      lMousePressed(false),
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
    
    if (!controls->rMousePressed) {
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
    
    controls->camera->procMouseMovement(xoffset, yoffset);
}

void Controls::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Controls* controls = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    if (controls) {
        controls->camera->procMouseScroll(static_cast<float>(yoffset));
    }
}

void Controls::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Controls* controls = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    if (!controls) return;
    
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            controls->rMousePressed = true;
            controls->firstMouse = true;
        } else if (action == GLFW_RELEASE) {
            controls->rMousePressed = false;
            controls->firstMouse = true;
        }
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            controls->lMousePressed = true;
        } else if (action == GLFW_RELEASE) {
            controls->lMousePressed = false;
        }
    }
}

void Controls::procCameraInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->procKeyboard(CameraMov::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->procKeyboard(CameraMov::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->procKeyboard(CameraMov::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->procKeyboard(CameraMov::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->procKeyboard(CameraMov::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->procKeyboard(CameraMov::DOWN, deltaTime);
}

void Controls::procInput(float deltaTime) {
    procCameraInput(deltaTime);
}

void Controls::procSceneSwitch(int& currentSceneIdx, int sceneCount) {
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!tabPressed) {
            currentSceneIdx = (currentSceneIdx + 1) % sceneCount;
            tabPressed = true;
        }
    } else {
        tabPressed = false;
    }

    for (int key = GLFW_KEY_1; key <= GLFW_KEY_9; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            int index = key - GLFW_KEY_1;
            if (index < static_cast<int>(sceneCount)) {
                currentSceneIdx = index;
            }
        }
    }
}

void Controls::procBatteryToggle(MultiShaderForestScene* forestScene) {
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

void Controls::procSkyboxToggle(ModelScene* modelScene) {
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

void Controls::procEditModeToggle(MultiShaderForestScene* forestScene) {
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

void Controls::procMouseHover(MultiShaderForestScene* forestScene) {
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

void Controls::procWhackAMoleInput(WhackAMoleScene* whackAMoleScene) {
    if (!whackAMoleScene) return;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    whackAMoleScene->updateMouseHover(xpos, ypos, width, height);
    
    static bool wasLeftPressed = false;
    bool isLeftPressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    
    if (isLeftPressed && !wasLeftPressed) {
        whackAMoleScene->handleMouseClick(xpos, ypos, width, height);
    }
    
    wasLeftPressed = isLeftPressed;
}