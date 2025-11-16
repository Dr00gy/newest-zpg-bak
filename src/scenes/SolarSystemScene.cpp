#include "SolarSystemScene.hpp"
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>

std::string SolarSystemScene::loadShaderSource(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void SolarSystemScene::init() {
    std::string vertexSrc = loadShaderSource("src/shaders/vertex.glsl");
    std::string fragSrc = loadShaderSource("src/shaders/frag_lambert.glsl");
    shader = std::make_unique<Shader>(vertexSrc.c_str(), fragSrc.c_str());

    sphereModel = ModelFactory::CreateSphere();

    light = std::make_unique<Light>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    light->setAmbient(0.3f);
    light->setDiffuse(0.8f);
    light->setSpecular(1.0f);
    light->attach(shader.get());
    light->notify();

    shader->addLight(light.get());
    light->attach(shader.get());
    shader->updateAllLights();

    sunTransform = std::make_shared<TransformComposite>();
    sunTransform->add(std::make_shared<TransformIdentity>());
    
    planetTransform = std::make_shared<TransformComposite>();
    planetTransform->add(std::make_shared<TransformIdentity>());
    
    moonTransform = std::make_shared<TransformComposite>();
    moonTransform->add(std::make_shared<TransformIdentity>());

    addObject(sphereModel.get(), shader.get(), sunTransform);
    addObject(sphereModel.get(), shader.get(), planetTransform);
    addObject(sphereModel.get(), shader.get(), moonTransform);
}

void SolarSystemScene::draw() {
    float time = static_cast<float>(glfwGetTime());

    shader->use();
    light->updateObservers();

    sunTransform = std::make_shared<TransformComposite>();
    sunTransform->add(std::make_shared<TransformIdentity>());
    sunTransform->add(std::make_shared<TransformScale>(glm::vec3(1.0f, 1.0f, 1.0f)));

    float planetOrbitRadius = 2.5f;
    float planetOrbitSpeed  = 50.0f;
    float planetScale       = 0.4f;

    planetTransform = std::make_shared<TransformComposite>();
    planetTransform->add(std::make_shared<TransformRotation>(time * planetOrbitSpeed, glm::vec3(0.0f, 1.0f, 0.0f)));
    planetTransform->add(std::make_shared<TransformTranslation>(glm::vec3(planetOrbitRadius, 0.0f, 0.0f)));
    planetTransform->add(std::make_shared<TransformScale>(glm::vec3(planetScale, planetScale, planetScale)));

    float moonOrbitRadius = 1.0f;
    float moonOrbitSpeed  = 150.0f;
    float moonScale       = 0.2f;

    moonTransform = std::make_shared<TransformComposite>();
    moonTransform->add(std::make_shared<TransformRotation>(time * planetOrbitSpeed, glm::vec3(0.0f, 1.0f, 0.0f)));
    moonTransform->add(std::make_shared<TransformTranslation>(glm::vec3(planetOrbitRadius, 0.0f, 0.0f)));
    moonTransform->add(std::make_shared<TransformRotation>(time * moonOrbitSpeed, glm::vec3(0.0f, 1.0f, 0.0f)));
    moonTransform->add(std::make_shared<TransformTranslation>(glm::vec3(moonOrbitRadius, 0.0f, 0.0f)));
    moonTransform->add(std::make_shared<TransformScale>(glm::vec3(moonScale, moonScale, moonScale)));

    if (objects.size() > 0)
        objects[0].transform = sunTransform;
    if (objects.size() > 1)
        objects[1].transform = planetTransform;
    if (objects.size() > 2)
        objects[2].transform = moonTransform;

    glUseProgram(0);

    for (auto& obj : objects) {
        obj.shader->use();

        if (obj.transform == sunTransform) {
            obj.shader->SetUniform("objectColor", glm::vec3(1.0f, 1.0f, 0.0f));
        } else if (obj.transform == planetTransform) {
            obj.shader->SetUniform("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
        } else if (obj.transform == moonTransform) {
            obj.shader->SetUniform("objectColor", glm::vec3(0.5f, 0.5f, 0.5f));
        }

        obj.shader->SetUniform("shininess", 32.0f);

        if (attachedCamera)
            obj.shader->SetUniform("viewPos", attachedCamera->getPosition());

        glm::mat4 model = obj.transform->getMatrix();
        obj.shader->SetUniform("model", model);

        obj.model->draw(GL_TRIANGLES);
        glUseProgram(0);
    }
}

void SolarSystemScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void SolarSystemScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}