#pragma once
#include "Model.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "trans/Transform.hpp"
#include <memory>

struct DrawableObject {
    Model* model;
    Shader* shader;
    std::shared_ptr<Transform> transform;
    Texture* texture = nullptr;
};