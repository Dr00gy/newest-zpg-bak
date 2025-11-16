#pragma once
#include "BaseScene.hpp"
#include <memory>

class RandomObjectsScene : public BaseScene {
public:
    RandomObjectsScene() = default;

    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;

private:
    std::unique_ptr<Model> cubeModel;
    std::unique_ptr<Model> sphereModel;
    std::unique_ptr<Shader> shader1, shader2, shader3;
};
