#pragma once
#include "BaseScene.hpp"
#include <memory>
#include <string>

class SolarSystemScene : public BaseScene {
public:
    SolarSystemScene() = default;
    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;

private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Model> sphereModel;
    std::unique_ptr<Light> light;
    
    std::shared_ptr<TransformComposite> sunTransform;
    std::shared_ptr<TransformComposite> planetTransform;
    std::shared_ptr<TransformComposite> moonTransform;
    
    std::string loadShaderSource(const std::string& filePath);
};