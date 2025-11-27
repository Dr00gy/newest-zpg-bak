#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position),
      worldUp(up),
      yaw(yaw),
      pitch(pitch),
      movSpeed(SPEED),
      mouseSensitivity(SENSITIVITY),
      fov(FOV),
      aspectRatio(800.0f / 600.0f),
      nearPlane(NEAR_PLANE),
      farPlane(FAR_PLANE) {
    updateCameraVecs();
}

glm::mat4 Camera::getViewMat() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjMat() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::procKeyboard(CameraMov direction, float deltaTime) {
    float velocity = movSpeed * deltaTime;
    
    switch (direction) {
        case CameraMov::FORWARD:
            position += front * velocity;
            break;
        case CameraMov::BACKWARD:
            position -= front * velocity;
            break;
        case CameraMov::LEFT:
            position -= right * velocity;
            break;
        case CameraMov::RIGHT:
            position += right * velocity;
            break;
        case CameraMov::UP:
            position += up * velocity;
            break;
        case CameraMov::DOWN:
            position -= up * velocity;
            break;
    }
    
    lastChangeType = ChangeType::VIEW;
    notify();
}

void Camera::procMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    
    updateCameraVecs();
    lastChangeType = ChangeType::VIEW;
    notify();
}

void Camera::procMouseScroll(float yoffset) {
    fov -= yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
    
    lastChangeType = ChangeType::PROJECTION;
    notify();
}

void Camera::updateAspectRatio(float width, float height) {
    aspectRatio = width / height;
    lastChangeType = ChangeType::PROJECTION;
    notify();
}

void Camera::updateCameraVecs() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}