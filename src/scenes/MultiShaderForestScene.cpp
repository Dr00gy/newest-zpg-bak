#include "MultiShaderForestScene.hpp"
#include <cstdlib>
#include <ctime>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

void MultiShaderForestScene::init() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::string vertexSrc = loadShaderSrc("src/shaders/vertex.glsl");
    std::string vertexTexturedSrc = loadShaderSrc("src/shaders/vertex_textured.glsl");
    std::string fragLambertSrc = loadShaderSrc("src/shaders/mult_lambert.glsl");
    std::string fragPhongSrc = loadShaderSrc("src/shaders/mult_phong.glsl");
    std::string fragBlinnSrc = loadShaderSrc("src/shaders/mult_blinn.glsl");
    std::string fragPhongTexturedSrc = loadShaderSrc("src/shaders/mult_phong_textured.glsl");
    std::string fragTriangleSrc = loadShaderSrc("src/shaders/frag_triangle.glsl");

    lambertShader = std::make_unique<Shader>(vertexSrc.c_str(), fragLambertSrc.c_str());
    phongShader = std::make_unique<Shader>(vertexSrc.c_str(), fragPhongSrc.c_str());
    blinnShader = std::make_unique<Shader>(vertexSrc.c_str(), fragBlinnSrc.c_str());
    phongTexturedShader = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragPhongTexturedSrc.c_str());
    triangleShader = std::make_unique<Shader>(vertexSrc.c_str(), fragTriangleSrc.c_str());

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

    // FIREFLIES INIT HERE
    for (int i = 0; i < lights.size(); ++i) {
        float radius, height;
        switch(i) {
            case 0:
                radius = 5.0f;
                height = 3.0f;
                break;
            case 1:
                radius = 5.0f;
                height = 3.0f;
                break;
            case 2:
                radius = 8.0f;
                height = 5.0f;
                break;
            case 3:
                radius = 8.0f;
                height = 2.0f;
                break;
        }
        
        auto rotation = std::make_shared<TransformRotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        fireflyRotations.push_back(rotation);
        
        auto sphereTransform = std::make_shared<TransformComposite>();
        sphereTransform->add(rotation);
        sphereTransform->add(std::make_shared<TransformTranslation>(glm::vec3(radius, height, 0.0f)));
        sphereTransform->add(std::make_shared<TransformScale>(glm::vec3(0.1f)));
        
        lightSpheres.push_back({lights[i].get(), objects.size()});
        addObject(sphereModel.get(), phongShader.get(), sphereTransform);
    }
    // NO LONGER FIREFLIES INIT

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

    std::vector<glm::vec3> bezierPoints = {
        glm::vec3(-3.0f, -1.0f, -2.0f),
        glm::vec3(-5.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -8.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
    };

    shrekBezierTrans = std::make_shared<TransformBezier>(bezierPoints, 0.0f);
    auto shrekTransform = std::make_shared<TransformComposite>();
    shrekTransform->add(shrekBezierTrans);
    shrekObjectIndex = objects.size();
    addObject(shrekModel.get(), phongTexturedShader.get(), shrekTransform, shrekTexture.get(), Material::Shrek());

    auto fionaTransform = std::make_shared<TransformComposite>();
    fionaTransform->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, -1.0f, 0.0f)));
    auto toiletTransform = std::make_shared<TransformComposite>();
    toiletTransform->add(std::make_shared<TransformTranslation>(glm::vec3(-3.0f, -1.0f, -3.0f)));
    addObject(fionaModel.get(), phongTexturedShader.get(), fionaTransform, fionaTexture.get(), Material::Fiona());
    addObject(toiletModel.get(), phongTexturedShader.get(), toiletTransform, toiletTexture.get());
    
    std::cout << "Initial mode is CREATION (Press M to switch modes)!" << std::endl;
}

int MultiShaderForestScene::getShroomAtCursor(double xpos, double ypos, int W, int H) {
    int x = static_cast<int>(xpos);
    int y = H - static_cast<int>(ypos);
    
    if (x < 0 || x >= W || y < 0 || y >= H) return -1;
    
    GLubyte stencilValue;
    glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &stencilValue);
    
    if (stencilValue == 0) return -1;
    
    int shroomIdx = static_cast<int>(stencilValue) - 1;
    if (shroomIdx >= 0 && shroomIdx < shroomObjects.size()) {
        return shroomIdx;
    }
    
    return -1;
}

void MultiShaderForestScene::addShroomAtPos(const glm::vec3& worldPos) {
    auto shroomTransform = std::make_shared<TransformComposite>();
    shroomTransform->add(std::make_shared<TransformTranslation>(glm::vec3(2.0f, 0.0f, 0.0f)));
    shroomTransform->add(std::make_shared<TransformTranslation>(worldPos));
    shroomTransform->add(std::make_shared<TransformScale>(glm::vec3(0.05f)));
    
    int objIndex = objects.size();
    addObject(shroomModel.get(), phongTexturedShader.get(), shroomTransform, shroomTexture.get());
    shroomObjects.push_back({objIndex, worldPos});
    
    std::cout << "Added shroom at (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
}

void MultiShaderForestScene::deleteShroomAtCursor(double xpos, double ypos, int W, int H) {
    int shroomIndex = getShroomAtCursor(xpos, ypos, W, H);
    if (shroomIndex < 0) return;
    int objIndex = shroomObjects[shroomIndex].objectIndex;

    if (objIndex < objects.size()) {
        objects.erase(objects.begin() + objIndex);
        for (int i = 0; i < shroomObjects.size(); ++i) {
            if (shroomObjects[i].objectIndex > objIndex) {
                shroomObjects[i].objectIndex--;
            }
        }
        
        for (auto& ls : lightSpheres) {
            if (ls.objectIndex > objIndex) {
                ls.objectIndex--;
            }
        }
        
        if (shrekObjectIndex > objIndex) {
            shrekObjectIndex--;
        }
    }
    
    shroomObjects.erase(shroomObjects.begin() + shroomIndex);
    std::cout << "Deleted shroom" << std::endl;
}

glm::vec3 MultiShaderForestScene::mouseToWorld(double xpos, double ypos, int W, int H) {
    if (!attachedCamera) return glm::vec3(0.0f);
    float winX = static_cast<float>(xpos);
    float winY = static_cast<float>(H - ypos);
    glm::vec4 viewport(0, 0, W, H);
    glm::mat4 view = attachedCamera->getViewMat();
    glm::mat4 proj = attachedCamera->getProjMat();
    glm::vec3 nearP = glm::unProject({winX, winY, 0.0f}, view, proj, viewport);
    glm::vec3 farP  = glm::unProject({winX, winY, 1.0f}, view, proj, viewport);
    
    glm::vec3 dir = glm::normalize(farP - nearP);
    float t = (-1.0f - nearP.y) / dir.y;

    return nearP + dir * t;
}

void MultiShaderForestScene::handleMouseClick(double xpos, double ypos, int width, int height) {
    switch(editMode) {
        case EditMode::CREATION: {
            glm::vec3 worldPos = mouseToWorld(xpos, ypos, width, height);
            addShroomAtPos(worldPos);
            break;
        }
        case EditMode::DELETION:
            deleteShroomAtCursor(xpos, ypos, width, height);
            break;
        case EditMode::BEZIER_EDIT: {
            glm::vec3 worldPos = mouseToWorld(xpos, ypos, width, height);
            addBezierPoint(worldPos);
            break;
        }
    }
}

void MultiShaderForestScene::updateMouseHover(double xpos, double ypos, int width, int height) {
    hoveredShroomIndex = getShroomAtCursor(xpos, ypos, width, height);
}

void MultiShaderForestScene::drawShroomsWStencil() {
    if (hoveredShroomIndex < 0 || hoveredShroomIndex >= static_cast<int>(shroomObjects.size())) {
        return;
    }
    
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);
    
    int objIndex = shroomObjects[hoveredShroomIndex].objectIndex;
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
    
    obj.model->draw(GL_TRIANGLES);
    
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    
    glUseProgram(0);
}

void MultiShaderForestScene::draw() {
    float time = static_cast<float>(glfwGetTime());
    if (shrekBezierTrans) {
        float t = fmod(time * bezierAnimSpeed, 1.0f);
        shrekBezierTrans->setParam(t);
    }
    
    for (int i = 0; i < fireflyRotations.size(); ++i) {
        float speed = 0.1f + i; 
        float angle = time * speed * 50.0f;
        
        fireflyRotations[i]->setAngle(angle);
        
        glm::mat4 transform = objects[lightSpheres[i].objectIndex].transform->getMatrix();
        glm::vec3 pos = glm::vec3(transform[3]);
        lights[i]->setPosition(pos);
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
    
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilMask(0xFF);
    
    // non-shroom objects stencil value 0
    for (int i = 0; i < objects.size(); ++i) {
        bool isLightSphere = false;
        for (int j = 0; j < lightSpheres.size(); ++j) {
            if (i == lightSpheres[j].objectIndex) {
                isLightSphere = true;
                break;
            }
        }
        
        bool isShroom = false;
        for (int j = 0; j < shroomObjects.size(); ++j) {
            if (i == shroomObjects[j].objectIndex) {
                isShroom = true;
                break;
            }
        }
        
        bool isHoveredShroom = false;
        if (hoveredShroomIndex >= 0 && hoveredShroomIndex < static_cast<int>(shroomObjects.size())) {
            if (i == shroomObjects[hoveredShroomIndex].objectIndex) {
                isHoveredShroom = true;
            }
        }
        
        if (!isLightSphere && !isShroom && !isHoveredShroom) {
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
    
    for (int i = 0; i < shroomObjects.size(); i++) {
        if (hoveredShroomIndex >= 0 && i == hoveredShroomIndex) continue;
        
        glStencilFunc(GL_ALWAYS, i + 1, 0xFF);
        
        auto& obj = objects[shroomObjects[i].objectIndex];
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
    
    glStencilMask(0x00);
    glDisable(GL_STENCIL_TEST);
    
    drawShroomsWStencil();
    
    // FIREFLIES HERE
    phongShader->use();
    phongShader->SetUniform("objectColor", glm::vec3(2.0f, 2.0f, 0.0f));
    phongShader->SetUniform("shininess", 128.0f);
    for (int i = 0; i < lightSpheres.size(); ++i) {
        auto& obj = objects[lightSpheres[i].objectIndex];
        glm::mat4 model = obj.transform->getMatrix();
        phongShader->SetUniform("isFirefly", true);
        phongShader->SetUniform("model", model);
        obj.model->draw(GL_TRIANGLES);
        phongShader->SetUniform("isFirefly", false);
    }
    glUseProgram(0);
}

void MultiShaderForestScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void MultiShaderForestScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}

void MultiShaderForestScene::addBezierPoint(const glm::vec3& worldPos) {
    bezierControlPoints.push_back(worldPos);
    std::cout << "Added Bezier point #" << bezierControlPoints.size() 
    << " at (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
    
    if (bezierControlPoints.size() >= 4) {
        updateBezierPath();
    } else {
        std::cout << "Add atleast 4 pts (currently: " << bezierControlPoints.size() << ")" << std::endl;
    }
}

void MultiShaderForestScene::updateBezierPath() {
    if (bezierControlPoints.size() < 4) {
        return;
    }

    shrekBezierTrans = std::make_shared<TransformBezier>(bezierControlPoints, 0.0f);
    auto shrekTransform = std::make_shared<TransformComposite>();
    shrekTransform->add(shrekBezierTrans);
    objects[shrekObjectIndex].transform = shrekTransform;
    
    std::cout << "Bezier path updated with pts..." << std::endl;
}