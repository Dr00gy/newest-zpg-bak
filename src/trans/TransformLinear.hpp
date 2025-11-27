#pragma once
#include "Transform.hpp"
#include <algorithm>

class TransformLinear : public Transform {
public:
    TransformLinear(const glm::vec3& startPoint, const glm::vec3& endPoint, float t = 0.0f)
        : start(startPoint), end(endPoint), param(t) {}

    void setParam(float t) {
        param = glm::clamp(t, 0.0f, 1.0f);
    }

    float getParam() const {
        return param;
    }

    void setStartPoint(const glm::vec3& point) {
        start = point;
    }

    void setEndPoint(const glm::vec3& point) {
        end = point;
    }

    glm::vec3 getStartPoint() const {
        return start;
    }

    glm::vec3 getEndPoint() const {
        return end;
    }

    glm::vec3 getPositionOnPath() const {
        return glm::mix(start, end, param);
    }

    glm::mat4 getMatrix() const override {
        glm::vec3 position = getPositionOnPath();
        return glm::translate(glm::mat4(1.0f), position);
    }

private:
    glm::vec3 start;
    glm::vec3 end;
    float param;
};