#pragma once
#include "Transform.hpp"

class TransformScale : public Transform {
public:
    TransformScale(const glm::vec3& scale) : scaleVec(scale) {}
    TransformScale(float uniformScale) : scaleVec(uniformScale, uniformScale, uniformScale) {}
    glm::mat4 getMatrix() const override {
        return glm::scale(glm::mat4(1.0f), scaleVec);
    }

private:
    glm::vec3 scaleVec;
};