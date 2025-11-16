#pragma once
#include "BaseScene.hpp"
#include "../ModelFactory.hpp"
#include "../Light.hpp"
#include "../Shader.hpp"
#include "../Model.hpp"
#include "../Texture.hpp"
#include <vector>
#include <memory>

class ModelScene : public BaseScene {
public:
    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;
    
    void toggleSkyboxMode();
    
    std::unique_ptr<Shader> modelShader;
    std::unique_ptr<Shader> modelShader2;
    std::unique_ptr<Shader> plainShader;
    std::unique_ptr<Shader> textureShader;
    std::unique_ptr<Shader> skyboxShader;
    std::unique_ptr<Shader> cubeShader;
    
    std::unique_ptr<Model> loginModel;
    std::unique_ptr<Model> houseModel;
    std::unique_ptr<Model> formulaModel;
    std::unique_ptr<Model> cupModel;
    std::unique_ptr<Model> bicycleModel;
    std::unique_ptr<Model> plainModel;
    std::unique_ptr<Model> skyboxModel;
    
    std::unique_ptr<Texture> loginTexture;
    std::unique_ptr<Texture> houseTexture;
    std::unique_ptr<Texture> goldTexture;
    std::unique_ptr<Texture> grassTexture;
    std::unique_ptr<Texture> skyboxTexture;
    
    std::vector<std::unique_ptr<Light>> lights;

private:
    void drawSkybox();
    bool isInsideSkybox = true;
};