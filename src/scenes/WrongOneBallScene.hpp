#pragma once
#include "BaseScene.hpp"
#include <memory>

class WrongOneBallScene : public BaseScene {
public:
    WrongOneBallScene();
    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;

private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Model> sphereModel;
    std::unique_ptr<Light> light;
};