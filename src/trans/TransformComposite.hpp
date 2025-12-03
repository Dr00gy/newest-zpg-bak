#pragma once
#include <vector>
#include <memory>
#include "Transform.hpp"

class TransformComposite : public Transform {
public:
    void add(std::shared_ptr<Transform> t) {
        transforms.push_back(t);
    }

    glm::mat4 getMatrix() const override {
        glm::mat4 result(1.0f);
        for (auto& t : transforms) {
            result = result * t->getMatrix();
        }
        return result;
    }

private:
    std::vector<std::shared_ptr<Transform>> transforms;
};