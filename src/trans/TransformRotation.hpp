#pragma once
#include "Transform.hpp"

class TransformRotation : public Transform {
public:
    TransformRotation(float angleDeg, const glm::vec3& axis)
        : angle(angleDeg), axis(axis) {}
    
    void setAngle(float angleDeg) {
        angle = angleDeg;
    }
    
    float getAngle() const {
        return angle;
    }
    
    void setAxis(const glm::vec3& newAxis) {
        axis = newAxis;
    }
    
    glm::vec3 getAxis() const {
        return axis;
    }
    
    glm::mat4 getMatrix() const override {
        return glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    }

private:
    float angle;
    glm::vec3 axis;
};