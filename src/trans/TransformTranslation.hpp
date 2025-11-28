#pragma once
#include "Transform.hpp"

class TransformTranslation : public Transform {
public:
    TransformTranslation(const glm::vec3& offset) : offset(offset) {}
    
    glm::mat4 getMatrix() const override {
        return glm::translate(glm::mat4(1.0f), offset);
    }
    
    void setOffset(const glm::vec3& newOffset) {
        offset = newOffset;
    }
    
    glm::vec3 getOffset() const {
        return offset;
    }

private:
    glm::vec3 offset;
};