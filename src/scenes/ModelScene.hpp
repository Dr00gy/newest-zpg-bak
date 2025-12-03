#pragma once
#include "BaseScene.hpp"
#include <vector>
#include <memory>

class ModelScene : public BaseScene {
public:
    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;

private:
    std::unique_ptr<Shader> modelShader;
    std::unique_ptr<Shader> modelShader2;
    std::unique_ptr<Shader> plainShader;
    std::unique_ptr<Shader> textureShader;
    std::unique_ptr<Shader> skyboxShader;
    std::unique_ptr<Shader> cubeShader;
    std::unique_ptr<Shader> normalMapShader;
    
    std::unique_ptr<Model> loginModel;
    std::unique_ptr<Model> houseModel;
    std::unique_ptr<Model> formulaModel;
    std::unique_ptr<Model> cupModel;
    std::unique_ptr<Model> bicycleModel;
    std::unique_ptr<Model> plainModel;
    std::unique_ptr<Model> skyboxModel;
    std::unique_ptr<Model> boxModel;
    
    std::unique_ptr<Texture> loginTexture;
    std::unique_ptr<Texture> houseTexture;
    std::unique_ptr<Texture> goldTexture;
    std::unique_ptr<Texture> grassTexture;
    std::unique_ptr<Texture> skyboxTexture;
    std::unique_ptr<Texture> boxAlbedoTexture;
    std::unique_ptr<Texture> boxNormalTexture;
    
    std::vector<std::unique_ptr<Light>> lights;

    std::shared_ptr<TransformBezier> formulaBezierTrans;
    int formulaObjIdx;
    float bezierAnimSpeed = 0.15f;
    //
    void drawSkybox();
};