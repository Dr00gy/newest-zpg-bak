#pragma once
#include "BaseScene.hpp"
#include <memory>

class ForestScene : public BaseScene {
public:
    ForestScene() = default;

    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;

private:
    std::unique_ptr<Model> bushModel;
    std::unique_ptr<Model> treeModel;
    std::unique_ptr<Model> plainModel;
    std::unique_ptr<Shader> bushShader;
    std::unique_ptr<Shader> treeShader;
    std::unique_ptr<Shader> plainShader;
};