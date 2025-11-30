#pragma once
#include "Model.hpp"
#include "renderers/Shader.hpp"
#include "renderers/Texture.hpp"
#include "trans/Transform.hpp"
#include <memory>

struct DrawableObject {
    Model* model;
    Shader* shader;
    std::shared_ptr<Transform> transform;
    Texture* texture = nullptr;
    Texture* normalMap = nullptr;
    Material material = Material::Plastic();
    int normalIntensity = 1;
};