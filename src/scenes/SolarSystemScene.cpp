#include "SolarSystemScene.hpp"
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <cmath>

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

glm::vec3 calculateEllipticalOrbit(float time, float speed, float radiusWide, float radiusNarrow) {
    float angle = fmod(time * speed, 2.0f * M_PI);
    
    float distanceFromSun = sqrt(radiusWide * radiusWide * cos(angle) * cos(angle) + 
                                  radiusNarrow * radiusNarrow * sin(angle) * sin(angle));
    
    float speedMultiplier = 1.0f / (distanceFromSun);
    angle += speedMultiplier * 2.0f;
    
    glm::vec3 position(
        radiusWide * cos(angle),
        0.0f,
        radiusNarrow * sin(angle)
    );
    
    return position;
}

void SolarSystemScene::init() {
    std::string vertexSrc = loadShaderSource("src/shaders/vertex.glsl");
    std::string fragSrc = loadShaderSource("src/shaders/frag_lambert.glsl");
    shader = std::make_unique<Shader>(vertexSrc.c_str(), fragSrc.c_str());

    std::string vertexSrc2 = loadShaderSource("src/shaders/vertex.glsl");
    std::string fragSrc2 = loadShaderSource("src/shaders/frag_rectangle.glsl");
    shaderSun = std::make_unique<Shader>(vertexSrc2.c_str(), fragSrc2.c_str());

    std::string vertexTexturedSrc = loadShaderSource("src/shaders/vertex_textured.glsl");
    std::string fragTexturedSrc = loadShaderSource("src/shaders/frag_lambert_textured.glsl");
    texturedShader = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragTexturedSrc.c_str());

    sphereModel = ModelFactory::CreatePlainSphere();

    mercuryTexture = std::make_unique<Texture>("src/images/mercury.jpg");
    venusTexture = std::make_unique<Texture>("src/images/venus.jpg");
    earthTexture = std::make_unique<Texture>("src/images/earth.jpg");
    marsTexture = std::make_unique<Texture>("src/images/mars.jpg");
    jupiterTexture = std::make_unique<Texture>("src/images/jupiter.jpg");
    saturnTexture = std::make_unique<Texture>("src/images/saturn.jpg");
    uranusTexture = std::make_unique<Texture>("src/images/uranus.jpg");
    neptuneTexture = std::make_unique<Texture>("src/images/neptune.jpg");
    moonTexture = std::make_unique<Texture>("src/images/moon.jpg");
    sunTexture = std::make_unique<Texture>("src/images/sun.jpg");

    light = std::make_unique<Light>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    light->setAmbient(0.3f);
    light->setDiffuse(0.8f);
    light->setSpecular(1.0f);
    light->attach(shader.get());
    light->attach(texturedShader.get());
    light->notify();

    shader->addLight(light.get());
    texturedShader->addLight(light.get());
    light->attach(shader.get());
    light->attach(texturedShader.get());
    shader->updateAllLights();
    texturedShader->updateAllLights();

    sunTransform = std::make_shared<TransformComposite>();
    sunTransform->add(std::make_shared<TransformScale>(glm::vec3(1.5f, 1.5f, 1.5f)));
    addObject(sphereModel.get(), texturedShader.get(), sunTransform, sunTexture.get());

    for (int i = 0; i < 8; i++) {
        auto planetTransform = std::make_shared<TransformComposite>();
        planetTransform->add(std::make_shared<TransformIdentity>());
        planetTransforms.push_back(planetTransform);
        
        Texture* planetTextures[] = {
            mercuryTexture.get(), venusTexture.get(), earthTexture.get(), marsTexture.get(),
            jupiterTexture.get(), saturnTexture.get(), uranusTexture.get(), neptuneTexture.get()
        };
        addObject(sphereModel.get(), texturedShader.get(), planetTransform, planetTextures[i]);
        
        auto moonTransform = std::make_shared<TransformComposite>();
        moonTransform->add(std::make_shared<TransformIdentity>());
        moonTransforms.push_back(moonTransform);
        addObject(sphereModel.get(), texturedShader.get(), moonTransform, moonTexture.get());
    }
}

void SolarSystemScene::draw() {
    float time = static_cast<float>(glfwGetTime());

    shader->use();
    light->updateObservers();

    struct PlanetData {
        float radiusWide;
        float radiusNarrow;
        float orbitalSpeed;
        float scale;
        float moonOrbitRadius;
        float moonSpeed;
        float moonScale;
        float rotationSpeed;
    };
    
    std::vector<PlanetData> planets = {
        {3.0f, 2.0f, 0.8f, 0.25f, 0.4f, 3.0f, 0.08f, 2.0f},
        {4.5f, 4.5f, 0.6f, 0.35f, 0.5f, 2.5f, 0.10f, 1.5f},
        {6.0f, 5.5f, 0.5f, 0.35f, 0.6f, 2.0f, 0.12f, 1.8f},
        {7.5f, 6.8f, 0.4f, 0.30f, 0.5f, 2.2f, 0.10f, 1.7f},
        {10.0f, 9.5f, 0.25f, 1.2f, 1.2f, 1.5f, 0.15f, 2.5f},
        {13.0f, 12.0f, 0.20f, 0.70f, 1.5f, 1.2f, 0.15f, 2.3f},
        {16.0f, 15.0f, 0.15f, 0.50f, 1.0f, 1.5f, 0.12f, 2.0f},
        {19.0f, 18.5f, 0.12f, 0.45f, 1.0f, 1.5f, 0.10f, 1.9f}
    };

    float sunRotation = fmod(time * 20.0f, 360.0f); 
    auto newSunTransform = std::make_shared<TransformComposite>();
    newSunTransform->add(std::make_shared<TransformRotation>(sunRotation, glm::vec3(0.0f, 1.0f, 0.0f)));
    newSunTransform->add(std::make_shared<TransformScale>(glm::vec3(1.5f, 1.5f, 1.5f)));
    sunTransform = newSunTransform;
    objects[0].transform = newSunTransform;

    for (size_t i = 0; i < planets.size(); i++) {
        const auto& planet = planets[i];
        
        glm::vec3 orbitPosition = calculateEllipticalOrbit(time, planet.orbitalSpeed, 
                                                           planet.radiusWide, planet.radiusNarrow);
        
        float planetRotation = fmod(time * planet.rotationSpeed * 50.0f, 360.0f);
        auto newPlanetTransform = std::make_shared<TransformComposite>();
        newPlanetTransform->add(std::make_shared<TransformTranslation>(orbitPosition));
        newPlanetTransform->add(std::make_shared<TransformRotation>(planetRotation, glm::vec3(0.0f, 1.0f, 0.0f)));
        newPlanetTransform->add(std::make_shared<TransformScale>(glm::vec3(planet.scale)));
        planetTransforms[i] = newPlanetTransform;
        objects[1 + i * 2].transform = newPlanetTransform;
        
        float moonAngle = time * planet.moonSpeed;
        glm::vec3 moonOffset(
            planet.moonOrbitRadius * cos(moonAngle),
            0.0f,
            planet.moonOrbitRadius * sin(moonAngle)
        );
        
        float moonRotation = fmod(time * planet.moonSpeed * 20.0f, 360.0f);
        auto newMoonTransform = std::make_shared<TransformComposite>();
        newMoonTransform->add(std::make_shared<TransformTranslation>(orbitPosition + moonOffset));
        newMoonTransform->add(std::make_shared<TransformRotation>(moonRotation, glm::vec3(0.0f, 1.0f, 0.0f)));
        newMoonTransform->add(std::make_shared<TransformScale>(glm::vec3(planet.moonScale)));
        moonTransforms[i] = newMoonTransform;
        objects[2 + i * 2].transform = newMoonTransform;
    }

    glUseProgram(0);

    objects[0].shader->use();

    if (objects[0].texture) {
        objects[0].texture->bind(0);
        objects[0].shader->SetUniform("textureSampler", 0);
        objects[0].shader->SetUniform("useTexture", true);
        objects[0].shader->SetUniform("isSun", true);
        objects[0].shader->SetUniform("sunRadius", 3.0f);
        objects[0].shader->SetUniform("sunGlow", 2.0f);
    }

    if (attachedCamera)
        objects[0].shader->SetUniform("viewPos", attachedCamera->getPosition());

    glm::mat4 model = objects[0].transform->getMatrix();
    objects[0].shader->SetUniform("model", model);
    objects[0].model->draw(GL_TRIANGLES);

    if (objects[0].texture) {
        objects[0].texture->unbind();
    }

    glUseProgram(0);

    for (size_t i = 0; i < planets.size(); i++) {
        size_t planetIdx = 1 + i * 2;
        objects[planetIdx].shader->use();
        
        if (objects[planetIdx].texture) {
            objects[planetIdx].texture->bind(0);
            objects[planetIdx].shader->SetUniform("textureSampler", 0);
            objects[planetIdx].shader->SetUniform("useTexture", true);
        }
        
        objects[planetIdx].shader->SetUniform("shininess", 32.0f);
        if (attachedCamera)
            objects[planetIdx].shader->SetUniform("viewPos", attachedCamera->getPosition());
        model = objects[planetIdx].transform->getMatrix();
        objects[planetIdx].shader->SetUniform("model", model);
        objects[planetIdx].shader->SetUniform("isSun", false);
        objects[planetIdx].model->draw(GL_TRIANGLES);
        
        if (objects[planetIdx].texture) {
            objects[planetIdx].texture->unbind();
        }
        glUseProgram(0);
        
        size_t moonIdx = 2 + i * 2;
        objects[moonIdx].shader->use();
        
        if (objects[moonIdx].texture) {
            objects[moonIdx].texture->bind(0);
            objects[moonIdx].shader->SetUniform("textureSampler", 0);
            objects[moonIdx].shader->SetUniform("useTexture", true);
            objects[moonIdx].shader->SetUniform("isSun", false);
        }
        
        objects[moonIdx].shader->SetUniform("shininess", 32.0f);
        if (attachedCamera)
            objects[moonIdx].shader->SetUniform("viewPos", attachedCamera->getPosition());
        model = objects[moonIdx].transform->getMatrix();
        objects[moonIdx].shader->SetUniform("model", model);
        objects[moonIdx].model->draw(GL_TRIANGLES);
        
        if (objects[moonIdx].texture) {
            objects[moonIdx].texture->unbind();
        }
        glUseProgram(0);
    }
    
    if (attachedCamera)
        objects.back().shader->SetUniform("viewPos", attachedCamera->getPosition());
    model = objects.back().transform->getMatrix();
    objects.back().shader->SetUniform("model", model);

    glUseProgram(0);
}

void SolarSystemScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void SolarSystemScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}