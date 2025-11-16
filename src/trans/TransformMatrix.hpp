#pragma once
#include "Transform.hpp"

class TransformMatrix : public Transform {
public:
    TransformMatrix(const glm::mat4& m) : matrix(m) {}
    glm::mat4 getMatrix() const override {
        return matrix;
    }

private:
    glm::mat4 matrix;
};