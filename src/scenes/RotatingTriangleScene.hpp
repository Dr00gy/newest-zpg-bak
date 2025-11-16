#pragma once
#include "BaseScene.hpp"
#include <memory>

class RotatingTriangleScene : public BaseScene {
public:
    RotatingTriangleScene() = default;

    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;

private:
    std::unique_ptr<Model> triangle;
    std::unique_ptr<Shader> shader;
    std::shared_ptr<TransformComposite> transform;
};