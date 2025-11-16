#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    virtual ~Transform() = default;
    virtual glm::mat4 getMatrix() const = 0;
};