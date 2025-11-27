#include "SymmetricalBallsScene.hpp"
#include <string>

SymmetricalBallsScene::SymmetricalBallsScene() {}

void SymmetricalBallsScene::init() {
    std::string vertexSrc = loadShaderSrc("src/shaders/vertex.glsl");
    std::string fragSrc   = loadShaderSrc("src/shaders/frag_phong_correct.glsl");
    shader = std::make_unique<Shader>(vertexSrc.c_str(), fragSrc.c_str());

    sphereModel = ModelFactory::CreateSphere();

    light = std::make_unique<Light>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    light->setAmbient(0.3f);
    light->setDiffuse(0.8f);
    light->setSpecular(1.0f);
    
    shader->addLight(light.get());
    light->attach(shader.get());
    shader->updateAllLights();

    float positions[4][2] = {
        { 0.0f,  1.0f},  // top
        { 0.0f, -1.0f},  // bottom
        { 1.0f,  0.0f},  // right
        {-1.0f,  0.0f}   // left
    };

    for (int i = 0; i < 4; ++i) {
        auto t = std::make_shared<TransformComposite>();
        t->add(std::make_shared<TransformTranslation>(glm::vec3(positions[i][0], positions[i][1], 0.0f)));
        t->add(std::make_shared<TransformScale>(glm::vec3(0.5f, 0.5f, 0.5f)));

        addObject(sphereModel.get(), shader.get(), t);
    }
}

void SymmetricalBallsScene::draw() {
    shader->use();
    shader->SetUniform("objectColor", glm::vec3(0.0f, 0.8f, 0.2f));
    shader->SetUniform("shininess", 32.0f);
    
    drawImpl();
}

void SymmetricalBallsScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void SymmetricalBallsScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}