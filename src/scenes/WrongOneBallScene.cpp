#include "WrongOneBallScene.hpp"
#include <string>

WrongOneBallScene::WrongOneBallScene() {}

void WrongOneBallScene::init() {
    std::string vertexSrc = loadShaderSrc("src/shaders/vertex.glsl");
    std::string fragSrc   = loadShaderSrc("src/shaders/frag_phong.glsl");
    shader = std::make_unique<Shader>(vertexSrc.c_str(), fragSrc.c_str());

    sphereModel = ModelFactory::CreateSphere();

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

void WrongOneBallScene::draw() {
    shader->use();
    shader->SetUniform("objectColor", glm::vec3(0.9f, 0.2f, 0.2f));
    shader->SetUniform("shininess", 1.0f);
    
    drawImpl();
}

void WrongOneBallScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void WrongOneBallScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}