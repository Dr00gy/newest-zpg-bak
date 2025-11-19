#include "MultiShaderForestScene.hpp"

#include <cstdlib>
#include <ctime>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

void MultiShaderForestScene::init() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::string vertexSrc = loadShaderSource("src/shaders/vertex.glsl");
    std::string vertexTexturedSrc = loadShaderSource("src/shaders/vertex_textured.glsl");
    std::string fragLambertSrc = loadShaderSource("src/shaders/mult_lambert.glsl");
    std::string fragPhongSrc = loadShaderSource("src/shaders/mult_phong.glsl");
    std::string fragBlinnSrc = loadShaderSource("src/shaders/mult_blinn.glsl");
    std::string fragPhongTexturedSrc = loadShaderSource("src/shaders/mult_phong_textured.glsl");
    std::string fragTriangleSrc = loadShaderSource("src/shaders/frag_triangle.glsl");
    std::string fragStencilSrc = loadShaderSource("src/shaders/stencil.glsl");

    lambertShader = std::make_unique<Shader>(vertexSrc.c_str(), fragLambertSrc.c_str());
    phongShader = std::make_unique<Shader>(vertexSrc.c_str(), fragPhongSrc.c_str());
    blinnShader = std::make_unique<Shader>(vertexSrc.c_str(), fragBlinnSrc.c_str());
    phongTexturedShader = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragPhongTexturedSrc.c_str());
    triangleShader = std::make_unique<Shader>(vertexSrc.c_str(), fragTriangleSrc.c_str());
    stencilShader = std::make_unique<Shader>(vertexSrc.c_str(), fragStencilSrc.c_str());

    bushModel = ModelFactory::CreateBush();
    treeModel = ModelFactory::CreateTree();
    plainModel = ModelFactory::CreatePlain();
    sphereModel = ModelFactory::CreatePlainSphere();
    shrekModel = ModelFactory::CreateShrek();
    fionaModel = ModelFactory::CreateFiona();
    toiletModel = ModelFactory::CreateToilet();
    shroomModel = ModelFactory::CreateShroom();
    
    grassTexture = std::make_unique<Texture>("src/images/swamp.jpg");
    shrekTexture = std::make_unique<Texture>("src/images/shrek.png");
    fionaTexture = std::make_unique<Texture>("src/images/fiona.png");
    toiletTexture = std::make_unique<Texture>("src/images/toiled.jpg");
    shroomTexture = std::make_unique<Texture>("src/images/hrib.jpg");

    auto light1 = std::make_unique<Light>(
        glm::vec3(5.0f, 3.0f, 5.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        LightType::POINT
    );
    light1->setAmbient(0.05f);
    light1->setDiffuse(0.8f);
    light1->setSpecular(0.05f);
    
    auto light2 = std::make_unique<Light>(
        glm::vec3(-5.0f, 3.0f, -5.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        LightType::POINT
    );
    light2->setAmbient(0.05f);
    light2->setDiffuse(0.6f);
    light2->setSpecular(0.05f);
    
    auto light3 = std::make_unique<Light>(
        glm::vec3(0.0f, 5.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        LightType::POINT
    );
    light3->setAmbient(0.07f);
    light3->setDiffuse(1.0f);
    light3->setSpecular(0.05f);
    
    auto light4 = std::make_unique<Light>(
        glm::vec3(-8.0f, 2.0f, 8.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        LightType::POINT
    );
    light4->setAmbient(0.03f);
    light4->setDiffuse(0.5f);
    light4->setSpecular(0.05f);
    
    directionalLight = std::make_unique<Light>(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        LightType::DIRECTIONAL
    );
    directionalLight->setAmbient(0.15f);
    directionalLight->setDiffuse(0.4f);
    directionalLight->setSpecular(0.3f);
    
    flashlight = std::make_unique<Light>(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        LightType::REFLECTOR
    );
    flashlight->setAmbient(0.0f);
    flashlight->setDiffuse(1.0f);
    flashlight->setSpecular(1.0f);
    flashlight->setCutOff(12.5f);
    flashlight->setOuterCutOff(17.5f);
    
    lambertShader->addLight(light1.get());
    lambertShader->addLight(light2.get());
    lambertShader->addLight(light3.get());
    lambertShader->addLight(light4.get());
    lambertShader->addLight(directionalLight.get());
    lambertShader->addLight(flashlight.get());
    
    phongShader->addLight(light1.get());
    phongShader->addLight(light2.get());
    phongShader->addLight(light3.get());
    phongShader->addLight(light4.get());
    phongShader->addLight(directionalLight.get());
    phongShader->addLight(flashlight.get());
    
    blinnShader->addLight(light1.get());
    blinnShader->addLight(light2.get());
    blinnShader->addLight(light3.get());
    blinnShader->addLight(light4.get());
    blinnShader->addLight(directionalLight.get());
    blinnShader->addLight(flashlight.get());
    
    phongTexturedShader->addLight(light1.get());
    phongTexturedShader->addLight(light2.get());
    phongTexturedShader->addLight(light3.get());
    phongTexturedShader->addLight(light4.get());
    phongTexturedShader->addLight(directionalLight.get());
    phongTexturedShader->addLight(flashlight.get());
    
    light1->attach(lambertShader.get());
    light1->attach(phongShader.get());
    light1->attach(blinnShader.get());
    light1->attach(phongTexturedShader.get());
    
    light2->attach(lambertShader.get());
    light2->attach(phongShader.get());
    light2->attach(blinnShader.get());
    light2->attach(phongTexturedShader.get());
    
    light3->attach(lambertShader.get());
    light3->attach(phongShader.get());
    light3->attach(blinnShader.get());
    light3->attach(phongTexturedShader.get());
    
    light4->attach(lambertShader.get());
    light4->attach(phongShader.get());
    light4->attach(blinnShader.get());
    light4->attach(phongTexturedShader.get());
    
    directionalLight->attach(lambertShader.get());
    directionalLight->attach(phongShader.get());
    directionalLight->attach(blinnShader.get());
    directionalLight->attach(phongTexturedShader.get());
    
    flashlight->attach(lambertShader.get());
    flashlight->attach(phongShader.get());
    flashlight->attach(blinnShader.get());
    flashlight->attach(phongTexturedShader.get());
    
    lights.push_back(std::move(light1));
    lights.push_back(std::move(light2));
    lights.push_back(std::move(light3));
    lights.push_back(std::move(light4));
    
    lambertShader->updateAllLights();
    phongShader->updateAllLights();
    blinnShader->updateAllLights();
    phongTexturedShader->updateAllLights();

    auto plainTransform = std::make_shared<TransformComposite>();
    plainTransform->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, -1.0f, 0.0f)));
    plainTransform->add(std::make_shared<TransformScale>(glm::vec3(20.0f, 1.0f, 20.0f)));
    addObject(plainModel.get(), phongTexturedShader.get(), plainTransform, grassTexture.get());

    for (size_t i = 0; i < lights.size(); ++i) {
        auto sphereTransform = std::make_shared<TransformComposite>();
        sphereTransform->add(std::make_shared<TransformTranslation>(lights[i]->getPosition()));
        sphereTransform->add(std::make_shared<TransformScale>(glm::vec3(0.1f)));
        
        lightSpheres.push_back({lights[i].get(), objects.size()});
        addObject(sphereModel.get(), phongShader.get(), sphereTransform);
    }

    Shader* lightingShaders[] = {
        lambertShader.get(),
        phongShader.get(),
        blinnShader.get()
    };
    int numShaders = 3;

    for (int i = 0; i < 50; ++i) {
        auto bushTransform = std::make_shared<TransformComposite>();
        float x = ((rand() % 400) / 10.0f) - 20.0f;
        float z = ((rand() % 400) / 10.0f) - 20.0f;
        bushTransform->add(std::make_shared<TransformTranslation>(glm::vec3(x, -1.0f, z)));
        bushTransform->add(std::make_shared<TransformRotation>(
            static_cast<float>(rand() % 360), glm::vec3(0.0f, 1.0f, 0.0f)
        ));
        
        float scale = 0.3f + ((rand() % 50) / 100.0f);
        bushTransform->add(std::make_shared<TransformScale>(glm::vec3(scale, scale, scale)));
        
        Shader* selectedShader = lightingShaders[rand() % numShaders];
        addObject(bushModel.get(), selectedShader, bushTransform);
    }

    for (int i = 0; i < 50; ++i) {
        auto treeTransform = std::make_shared<TransformComposite>();
        
        float x = ((rand() % 400) / 10.0f) - 20.0f;
        float z = ((rand() % 400) / 10.0f) - 20.0f;
        treeTransform->add(std::make_shared<TransformTranslation>(glm::vec3(x, -1.0f, z)));
        
        treeTransform->add(std::make_shared<TransformRotation>(
            static_cast<float>(rand() % 360), glm::vec3(0.0f, 1.0f, 0.0f)
        ));
        
        float scale = 0.5f + ((rand() % 70) / 100.0f);
        treeTransform->add(std::make_shared<TransformScale>(glm::vec3(scale, scale, scale)));
        
        Shader* selectedShader = lightingShaders[rand() % numShaders];
        
        addObject(treeModel.get(), selectedShader, treeTransform);
    }

    auto shrekTransform = std::make_shared<TransformComposite>();
    shrekTransform->add(std::make_shared<TransformTranslation>(glm::vec3(1.0f, -1.0f, 1.0f)));
    auto fionaTransform = std::make_shared<TransformComposite>();
    fionaTransform->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, -1.0f, 0.0f)));
    auto toiletTransform = std::make_shared<TransformComposite>();
    toiletTransform->add(std::make_shared<TransformTranslation>(glm::vec3(-3.0f, -1.0f, -3.0f)));

    addObject(shrekModel.get(), phongTexturedShader.get(), shrekTransform, shrekTexture.get());
    addObject(fionaModel.get(), phongTexturedShader.get(), fionaTransform, fionaTexture.get());
    addObject(toiletModel.get(), phongTexturedShader.get(), toiletTransform, toiletTexture.get());
    
    std::cout << "Edit mode initialized: CREATION (Press M to switch to deletion mode)" << std::endl;
}

glm::vec3 MultiShaderForestScene::unprojectMouseToWorld(double xpos, double ypos, int fbW, int fbH) {
    if (!attachedCamera) return glm::vec3(0.0f);

    float winX = static_cast<float>(xpos);
    float winY = static_cast<float>(fbH - ypos);

    glm::vec4 viewport(0.0f, 0.0f, static_cast<float>(fbW), static_cast<float>(fbH));
    glm::mat4 view = attachedCamera->getViewMatrix();
    glm::mat4 proj = attachedCamera->getProjectionMatrix();

    glm::vec3 nearP = glm::unProject({winX, winY, 0.0f}, view, proj, viewport);
    glm::vec3 farP  = glm::unProject({winX, winY, 1.0f}, view, proj, viewport);

    glm::vec3 dir = glm::normalize(farP - nearP);
    float t = (-1.0f - nearP.y) / dir.y;
    //std::cout << "near " << nearP.x << " " << nearP.y << " " << nearP.z << std::endl;

    return nearP + dir * t;
}


int MultiShaderForestScene::findShroomAtPosition(const glm::vec3& worldPos) {
    const float SELECTION_RADIUS = 1.0f;
    
    for (size_t i = 0; i < shroomObjects.size(); ++i) {
        glm::vec3 diff = worldPos - shroomObjects[i].position;
        float dist = glm::length(glm::vec2(diff.x, diff.z));
        
        if (dist < SELECTION_RADIUS) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

void MultiShaderForestScene::addShroomAtPosition(const glm::vec3& worldPos) {
    auto shroomTransform = std::make_shared<TransformComposite>();
    shroomTransform->add(std::make_shared<TransformTranslation>(glm::vec3(2.0f, 0.0f, 0.0f)));
    shroomTransform->add(std::make_shared<TransformTranslation>(worldPos));
    shroomTransform->add(std::make_shared<TransformScale>(glm::vec3(0.05f)));
    
    size_t objIndex = objects.size();
    addObject(shroomModel.get(), phongTexturedShader.get(), shroomTransform, shroomTexture.get());
    shroomObjects.push_back({objIndex, worldPos});
    
    std::cout << "Added shroom at (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
}

void MultiShaderForestScene::deleteShroomAtPosition(const glm::vec3& worldPos) {
    int shroomIndex = findShroomAtPosition(worldPos);
    if (shroomIndex >= 0) {
        size_t objIndex = shroomObjects[shroomIndex].objectIndex;
        
        if (objIndex < objects.size()) {
            objects.erase(objects.begin() + objIndex);
            
            for (size_t i = 0; i < shroomObjects.size(); ++i) {
                if (shroomObjects[i].objectIndex > objIndex) {
                    shroomObjects[i].objectIndex--;
                }
            }
            
            for (auto& ls : lightSpheres) {
                if (ls.objectIndex > objIndex) {
                    ls.objectIndex--;
                }
            }
        }
        
        shroomObjects.erase(shroomObjects.begin() + shroomIndex);
        std::cout << "Deleted shroom" << std::endl;
    }
}

void MultiShaderForestScene::handleMouseClick(double xpos, double ypos, int width, int height) {
    glm::vec3 worldPos = unprojectMouseToWorld(xpos, ypos, width, height);
    
    if (editMode) {
        addShroomAtPosition(worldPos);
    } else {
        deleteShroomAtPosition(worldPos);
    }
}

void MultiShaderForestScene::updateMouseHover(double xpos, double ypos, int width, int height) {
    glm::vec3 worldPos = unprojectMouseToWorld(xpos, ypos, width, height);
    hoveredShroomIndex = findShroomAtPosition(worldPos);
}

void MultiShaderForestScene::drawShroomsWithStencil() {
    if (hoveredShroomIndex < 0 || hoveredShroomIndex >= static_cast<int>(shroomObjects.size())) {
        return;
    }
    
    glEnable(GL_STENCIL_TEST);
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);
    
    size_t objIndex = shroomObjects[hoveredShroomIndex].objectIndex;
    auto& obj = objects[objIndex];
    obj.shader->use();
    
    if (obj.texture) {
        obj.texture->bind(0);
        obj.shader->SetUniform("textureSampler", 0);
    }
    
    glm::mat4 model = obj.transform->getMatrix();
    obj.shader->SetUniform("model", model);
    obj.model->draw(GL_TRIANGLES);
    
    if (obj.texture) {
        obj.texture->unbind();
    }
    
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    
    stencilShader->use();
    auto outlineTransform = std::make_shared<TransformComposite>();
    outlineTransform->add(std::make_shared<TransformTranslation>(shroomObjects[hoveredShroomIndex].position));
    outlineTransform->add(std::make_shared<TransformScale>(glm::vec3(0.13f)));
    
    glm::mat4 outlineModel = outlineTransform->getMatrix();
    stencilShader->SetUniform("model", outlineModel);
    
    if (editMode) {
        stencilShader->SetUniform("outlineColor", glm::vec3(0.0f, 1.0f, 0.0f));
    } else {
        stencilShader->SetUniform("outlineColor", glm::vec3(1.0f, 0.0f, 0.0f));
    }
    
    obj.model->draw(GL_TRIANGLES);
    
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    
    glUseProgram(0);
}

void MultiShaderForestScene::draw() {
    float time = static_cast<float>(glfwGetTime());
    
    lights[0]->setPosition(glm::vec3(
        5.0f * cos(time * 0.5f),
        3.0f + 1.0f * sin(time * 0.7f),
        5.0f * sin(time * 0.5f)
    ));
    
    lights[1]->setPosition(glm::vec3(
        -5.0f * cos(time * 0.3f),
        3.0f + 1.5f * sin(time * 0.5f),
        -5.0f * sin(time * 0.3f)
    ));
    
    lights[2]->setPosition(glm::vec3(
        8.0f * sin(time * 0.4f),
        5.0f + 2.0f * sin(time * 0.6f),
        8.0f * cos(time * 0.4f)
    ));
    
    lights[3]->setPosition(glm::vec3(
        -8.0f * sin(time * 0.25f),
        2.0f + 1.0f * cos(time * 0.8f),
        8.0f * cos(time * 0.25f)
    ));
    
    for (auto& ls : lightSpheres) {
        auto sphereTransform = std::make_shared<TransformComposite>();
        sphereTransform->add(std::make_shared<TransformTranslation>(ls.light->getPosition()));
        sphereTransform->add(std::make_shared<TransformScale>(glm::vec3(0.1f)));
        objects[ls.objectIndex].transform = sphereTransform;
    }
    
    if (attachedCamera) {
        flashlight->setPosition(attachedCamera->getPosition());
        flashlight->setDirection(attachedCamera->getFront());
    }
    
    if (flashlightEnabled) {
        flashlight->setDiffuse(1.0f);
        flashlight->setSpecular(1.0f);
    } else {
        flashlight->setDiffuse(0.0f);
        flashlight->setSpecular(0.0f);
    }
    
    lambertShader->use();
    lambertShader->SetUniform("objectColor", glm::vec3(0.05f, 0.25f, 0.05));
    lambertShader->updateAllLights();
    
    phongShader->use();
    phongShader->SetUniform("objectColor", glm::vec3(0.15f, 0.1f, 0.05f));
    phongShader->SetUniform("shininess", 32.0f);
    phongShader->updateAllLights();
    
    blinnShader->use();
    blinnShader->SetUniform("objectColor", glm::vec3(0.02f, 0.2f, 0.35f));
    blinnShader->SetUniform("shininess", 32.0f);
    blinnShader->updateAllLights();
    
    phongTexturedShader->use();
    phongTexturedShader->SetUniform("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    phongTexturedShader->SetUniform("shininess", 32.0f);
    phongTexturedShader->SetUniform("useTexture", true);
    phongTexturedShader->updateAllLights();
    
    glUseProgram(0);
    
    for (size_t i = 0; i < objects.size(); ++i) {
        bool isLightSphere = false;
        for (size_t j = 0; j < lightSpheres.size(); ++j) {
            if (i == lightSpheres[j].objectIndex) {
                isLightSphere = true;
                break;
            }
        }
        
        bool isHoveredShroom = false;
        if (hoveredShroomIndex >= 0 && hoveredShroomIndex < static_cast<int>(shroomObjects.size())) {
            if (i == shroomObjects[hoveredShroomIndex].objectIndex) {
                isHoveredShroom = true;
            }
        }
        
        if (!isLightSphere && !isHoveredShroom) {
            auto& obj = objects[i];
            obj.shader->use();
            
            if (obj.texture) {
                obj.texture->bind(0);
                obj.shader->SetUniform("textureSampler", 0);
            }
            
            glm::mat4 model = obj.transform->getMatrix();
            obj.shader->SetUniform("model", model);
            obj.model->draw(GL_TRIANGLES);
            
            if (obj.texture) {
                obj.texture->unbind();
            }
        }
    }
    
    drawShroomsWithStencil();
    
    phongShader->use();
    phongShader->SetUniform("objectColor", glm::vec3(2.0f, 2.0f, 0.0f));
    phongShader->SetUniform("shininess", 128.0f);
    
    for (size_t i = 0; i < lightSpheres.size(); ++i) {
        auto& obj = objects[lightSpheres[i].objectIndex];
        glm::mat4 model = obj.transform->getMatrix();
        phongShader->SetUniform("model", model);
        obj.model->draw(GL_TRIANGLES);
    }
    glUseProgram(0);
}

void MultiShaderForestScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void MultiShaderForestScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}