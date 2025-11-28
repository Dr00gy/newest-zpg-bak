#pragma once
#include "BaseScene.hpp"
#include <memory>
#include <string>
#include <vector>

class SolarSystemScene : public BaseScene {
public:
    SolarSystemScene() = default;
    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;

private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Shader> shaderSun;
    std::unique_ptr<Shader> texturedShader;

    std::unique_ptr<Model> sphereModel;
    
    std::unique_ptr<Texture> mercuryTexture;
    std::unique_ptr<Texture> venusTexture;
    std::unique_ptr<Texture> earthTexture;
    std::unique_ptr<Texture> marsTexture;
    std::unique_ptr<Texture> jupiterTexture;
    std::unique_ptr<Texture> saturnTexture;
    std::unique_ptr<Texture> uranusTexture;
    std::unique_ptr<Texture> neptuneTexture;
    std::unique_ptr<Texture> moonTexture;
    std::unique_ptr<Texture> sunTexture;

    std::unique_ptr<Light> light;
    
    std::shared_ptr<TransformComposite> sunTransform;
    std::shared_ptr<TransformRotation> sunRotationTransform;
    
    struct PlanetData {
        float axisWide;
        float axisNarrow;
        float orbitalSpeed;
        float scale;
        float moonOrbitRadius;
        float moonSpeed;
        float moonScale;
        float rotationSpeed;
        
        std::shared_ptr<TransformRotation> orbitRotation;
        std::shared_ptr<TransformBezier> orbitBezier;
        std::shared_ptr<TransformRotation> selfRotation;
        
        std::shared_ptr<TransformRotation> moonOrbitRotation;
        std::shared_ptr<TransformTranslation> moonOrbitTranslation;
        std::shared_ptr<TransformRotation> moonSelfRotation;
        
        float accumulatedAngle = 0.0f;
        float lastTime = 0.0f;
    };
    
    std::vector<PlanetData> planets;
};