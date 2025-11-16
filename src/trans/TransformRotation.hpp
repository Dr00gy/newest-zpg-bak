#pragma once
#include "Transform.hpp"

class TransformRotation : public Transform {
public:
    TransformRotation(float angleDeg, const glm::vec3& axis)
        : angle(angleDeg), axis(axis) {}
    glm::mat4 getMatrix() const override {
        return glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    }

private:
    float angle;
    glm::vec3 axis;
};