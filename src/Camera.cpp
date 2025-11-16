#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position),
      worldUp(up),
      yaw(yaw),
      pitch(pitch),
      movementSpeed(SPEED),
      mouseSensitivity(SENSITIVITY),
      fov(FOV),
      aspectRatio(800.0f / 600.0f),
      nearPlane(NEAR_PLANE),
      farPlane(FAR_PLANE) {
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ,
               float upX, float upY, float upZ,
               float yaw, float pitch)
    : position(glm::vec3(posX, posY, posZ)),
      worldUp(glm::vec3(upX, upY, upZ)),
      yaw(yaw),
      pitch(pitch),
      movementSpeed(SPEED),
      mouseSensitivity(SENSITIVITY),
      fov(FOV),
      aspectRatio(800.0f / 600.0f),
      nearPlane(NEAR_PLANE),
      farPlane(FAR_PLANE) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    
    switch (direction) {
        case CameraMovement::FORWARD:
            position += front * velocity;
            break;
        case CameraMovement::BACKWARD:
            position -= front * velocity;
            break;
        case CameraMovement::LEFT:
            position -= right * velocity;
            break;
        case CameraMovement::RIGHT:
            position += right * velocity;
            break;
        case CameraMovement::UP:
            position += up * velocity;
            break;
        case CameraMovement::DOWN:
            position -= up * velocity;
            break;
    }
    
    lastChangeType = ChangeType::VIEW;
    notify();
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
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
    
    updateCameraVectors();
    lastChangeType = ChangeType::VIEW;
    notify();
}

void Camera::processMouseScroll(float yoffset) {
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

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}