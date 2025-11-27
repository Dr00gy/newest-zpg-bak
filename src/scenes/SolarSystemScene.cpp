#include "SolarSystemScene.hpp"
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <cmath>

void SolarSystemScene::init() {
    std::string vertexSrc = loadShaderSrc("src/shaders/vertex.glsl");
    std::string vertexTexturedSrc = loadShaderSrc("src/shaders/vertex_textured.glsl");
    std::string fragSrc = loadShaderSrc("src/shaders/frag_lambert.glsl");
    std::string fragSrc2 = loadShaderSrc("src/shaders/frag_rectangle.glsl");
    std::string fragTexturedSrc = loadShaderSrc("src/shaders/frag_lambert_textured.glsl");

    shader = std::make_unique<Shader>(vertexSrc.c_str(), fragSrc.c_str());
    shaderSun = std::make_unique<Shader>(vertexSrc.c_str(), fragSrc2.c_str());
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

    sunRotationTransform = std::make_shared<TransformRotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    sunTransform = std::make_shared<TransformComposite>();
    sunTransform->add(sunRotationTransform);
    sunTransform->add(std::make_shared<TransformScale>(glm::vec3(1.5f, 1.5f, 1.5f)));
    addObject(sphereModel.get(), texturedShader.get(), sunTransform, sunTexture.get());

    // axis wide then narrow, orbital speed, scale, moonOrbitRad, moonSpeed, moonScale, rotationSpeed
    planets = {
        {4.0f, 2.0f, 2.0f, 0.25f, 0.4f, 3.0f, 0.08f, 2.0f}, // mercury
        {5.8f, 4.5f, 1.5f, 0.35f, 0.5f, 2.5f, 0.10f, 1.5f}, // venus
        {7.5f, 5.5f, 1.0f, 0.35f, 0.6f, 2.0f, 0.12f, 1.8f}, // earth
        {9.5f, 6.8f, 0.7f, 0.30f, 0.5f, 2.2f, 0.10f, 1.8f}, // mars
        {12.0f, 9.5f, 0.3f, 1.2f, 2.0f, 1.5f, 0.15f, 2.5f}, // jupiter
        {14.0f, 12.0f, 0.2f, 0.70f, 1.5f, 1.2f, 0.15f, 2.2f},// saturn
        {17.0f, 15.0f, 0.1f, 0.50f, 1.0f, 1.5f, 0.12f, 2.0f},// uranus
        {20.0f, 18.5f, 0.07f, 0.45f, 1.0f, 1.5f, 0.10f, 2.0f}// neptune
    };

    Texture* planetTextures[] = {
        mercuryTexture.get(), venusTexture.get(), earthTexture.get(), marsTexture.get(),
        jupiterTexture.get(), saturnTexture.get(), uranusTexture.get(), neptuneTexture.get()
    };

    for (int i = 0; i < planets.size(); i++) {
        auto& planet = planets[i];
        float avgRadius = (planet.axisWide + planet.axisNarrow) / 2.0f;
        
        planet.orbitRotation = std::make_shared<TransformRotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        planet.orbitTranslation = std::make_shared<TransformTranslation>(
            glm::vec3(avgRadius, 0.0f, 0.0f)
        );
        planet.selfRotation = std::make_shared<TransformRotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        
        float scaleX = planet.axisWide / avgRadius;
        float scaleZ = planet.axisNarrow / avgRadius;
        
        // elliptical orbit howto: rotate -> scale (ellipse) -> translate (distance) -> undo scale -> rotate (self) -> scale (planet size)
        auto planetTransform = std::make_shared<TransformComposite>();
        planetTransform->add(planet.orbitRotation);
        planetTransform->add(std::make_shared<TransformScale>(glm::vec3(scaleX, 1.0f, scaleZ)));
        planetTransform->add(planet.orbitTranslation);
        planetTransform->add(std::make_shared<TransformScale>(glm::vec3(1.0f/scaleX, 1.0f, 1.0f/scaleZ)));
        planetTransform->add(planet.selfRotation);
        planetTransform->add(std::make_shared<TransformScale>(glm::vec3(planet.scale)));
        
        addObject(sphereModel.get(), texturedShader.get(), planetTransform, planetTextures[i]);
        
        // MOONS
        if (i > 1) {
            planet.moonOrbitRotation = std::make_shared<TransformRotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            planet.moonOrbitTranslation = std::make_shared<TransformTranslation>(
                glm::vec3(planet.moonOrbitRadius, 0.0f, 0.0f)
            );
            planet.moonSelfRotation = std::make_shared<TransformRotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            
            auto moonTransform = std::make_shared<TransformComposite>();
            moonTransform->add(planet.orbitRotation);
            moonTransform->add(std::make_shared<TransformScale>(glm::vec3(scaleX, 1.0f, scaleZ)));
            moonTransform->add(planet.orbitTranslation);
            moonTransform->add(std::make_shared<TransformScale>(glm::vec3(1.0f/scaleX, 1.0f, 1.0f/scaleZ)));
            moonTransform->add(planet.moonOrbitRotation);
            moonTransform->add(planet.moonOrbitTranslation);
            moonTransform->add(planet.moonSelfRotation);
            moonTransform->add(std::make_shared<TransformScale>(glm::vec3(planet.moonScale))); 
            
            addObject(sphereModel.get(), texturedShader.get(), moonTransform, moonTexture.get());
        } else {
            auto dummyTransform = std::make_shared<TransformComposite>();
            dummyTransform->add(std::make_shared<TransformScale>(glm::vec3(0.0f)));
            addObject(sphereModel.get(), texturedShader.get(), dummyTransform, nullptr);
        }
    }
}

void SolarSystemScene::draw() {
    float time = static_cast<float>(glfwGetTime());
    shader->use();
    light->updateObservers();

    float sunRotation = fmod(time * 20.0f, 360.0f);
    sunRotationTransform->setAngle(sunRotation);

    for (int i = 0; i < planets.size(); i++) {
        auto& planet = planets[i];
        if (planet.lastTime == 0.0f) {
            planet.lastTime = time;
        }
        
        float deltaTime = time - planet.lastTime;
        planet.lastTime = time;
        
        float currentAngleRad = fmod(planet.accumulatedAngle, 2.0f * M_PI);
        float avgRadius = (planet.axisWide + planet.axisNarrow) / 2.0f;
        
        float currentRadius = sqrt(
            planet.axisWide * planet.axisWide * cos(currentAngleRad) * cos(currentAngleRad) +
            planet.axisNarrow * planet.axisNarrow * sin(currentAngleRad) * sin(currentAngleRad)
        );
        
        float speedMultiplier = avgRadius / currentRadius;
        
        speedMultiplier = glm::clamp(speedMultiplier, 0.4f, 2.5f);
        planet.accumulatedAngle += planet.orbitalSpeed * speedMultiplier * deltaTime;
        
        // orbit
        planet.orbitRotation->setAngle(glm::degrees(planet.accumulatedAngle));
        // self
        float planetRotation = fmod(time * planet.rotationSpeed * 50.0f, 360.0f);
        planet.selfRotation->setAngle(planetRotation);
        
        if (i > 1) {
            float moonAngle = time * planet.moonSpeed;
            planet.moonOrbitRotation->setAngle(glm::degrees(moonAngle));
            
            float moonRotation = fmod(time * planet.moonSpeed * 20.0f, 360.0f);
            planet.moonSelfRotation->setAngle(moonRotation);
        }
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

    if (attachedCamera) objects[0].shader->SetUniform("viewPos", attachedCamera->getPosition());
    glm::mat4 model = objects[0].transform->getMatrix();
    objects[0].shader->SetUniform("model", model);
    objects[0].model->draw(GL_TRIANGLES);
    if (objects[0].texture) {
        objects[0].texture->unbind();
    }
    glUseProgram(0);

    for (int i = 0; i < planets.size(); i++) {
        int planetIdx = 1 + i * 2;
        objects[planetIdx].shader->use();
        
        if (objects[planetIdx].texture) {
            objects[planetIdx].texture->bind(0);
            objects[planetIdx].shader->SetUniform("textureSampler", 0);
            objects[planetIdx].shader->SetUniform("useTexture", true);
        }
        
        objects[planetIdx].shader->SetUniform("shininess", 32.0f);
        if (attachedCamera) objects[planetIdx].shader->SetUniform("viewPos", attachedCamera->getPosition());
        model = objects[planetIdx].transform->getMatrix();
        objects[planetIdx].shader->SetUniform("model", model);
        objects[planetIdx].shader->SetUniform("isSun", false);
        objects[planetIdx].model->draw(GL_TRIANGLES);
        
        if (objects[planetIdx].texture) {
            objects[planetIdx].texture->unbind();
        }
        glUseProgram(0);
        
        int moonIdx = 2 + i * 2;
        if (i > 1) {
            objects[moonIdx].shader->use();
            
            if (objects[moonIdx].texture) {
                objects[moonIdx].texture->bind(0);
                objects[moonIdx].shader->SetUniform("textureSampler", 0);
                objects[moonIdx].shader->SetUniform("useTexture", true);
                objects[moonIdx].shader->SetUniform("isSun", false);
            }
            
            objects[moonIdx].shader->SetUniform("shininess", 32.0f);
            if (attachedCamera) objects[moonIdx].shader->SetUniform("viewPos", attachedCamera->getPosition());
            model = objects[moonIdx].transform->getMatrix();
            objects[moonIdx].shader->SetUniform("model", model);
            objects[moonIdx].model->draw(GL_TRIANGLES);
            
            if (objects[moonIdx].texture) {
                objects[moonIdx].texture->unbind();
            }
            glUseProgram(0);
        }
    }
}

void SolarSystemScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void SolarSystemScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}