#pragma once
#include "Transform.hpp"

class TransformIdentity : public Transform {
public:
    glm::mat4 getMatrix() const override {
        return glm::mat4(1.0f);
    }
};