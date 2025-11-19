#include "CorrectOneBallScene.hpp"
#include <string>

CorrectOneBallScene::CorrectOneBallScene() {}

void CorrectOneBallScene::init() {
    std::string vertexSrc = loadShaderSource("src/shaders/vertex.glsl");
    std::string fragSrc   = loadShaderSource("src/shaders/frag_phong_correct.glsl");
    shader = std::make_unique<Shader>(vertexSrc.c_str(), fragSrc.c_str());

    sphereModel = ModelFactory::CreatePlainSphere();

    light = std::make_unique<Light>(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    light->setAmbient(0.1f);
    light->setDiffuse(0.8f);
    light->setSpecular(1.0f);
    
    shader->addLight(light.get());
    light->attach(shader.get());
    shader->updateAllLights();

    auto t = std::make_shared<TransformComposite>();
    t->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, 0.0f, 0.0f)));
    t->add(std::make_shared<TransformScale>(glm::vec3(1.0f, 1.0f, 1.0f)));

    addObject(sphereModel.get(), shader.get(), t);
}

void CorrectOneBallScene::draw() {
    shader->use();
    shader->SetUniform("objectColor", glm::vec3(0.2f, 0.6f, 0.9f));
    shader->SetUniform("shininess", 16.0f);
    
    drawImpl();
}

void CorrectOneBallScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void CorrectOneBallScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}