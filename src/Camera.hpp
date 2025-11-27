#pragma once
#include "renderers/Subject.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMov {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;

class Camera : public Subject {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f);
    
    glm::mat4 getViewMat() const;
    glm::mat4 getProjMat() const;
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    glm::ivec2 getResolution() const { return resolution; }
    float getAspectRatio() const { return aspectRatio; }
    //
    void procKeyboard(CameraMov direction, float deltaTime);
    void procMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void procMouseScroll(float yoffset);
    void updateAspectRatio(float width, float height);
    //
    void setPosition(const glm::vec3& pos) {
        position = pos;
        lastChangeType = ChangeType::VIEW;
        notify();
    }
    
    void setPitch(float p) {
        pitch = p;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        updateCameraVecs();
        lastChangeType = ChangeType::VIEW;
        notify();
    }
    
    void setYaw(float y) {
        yaw = y;
        updateCameraVecs();
        lastChangeType = ChangeType::VIEW;
        notify();
    }

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::ivec2 resolution;
    
    float yaw;
    float pitch;
    float movSpeed;
    float mouseSensitivity;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    void updateCameraVecs();
};