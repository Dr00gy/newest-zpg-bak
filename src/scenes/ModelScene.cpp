#include "ModelScene.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

void ModelScene::init() {
    // VERTS
    std::string vertexSrc = loadShaderSrc("src/shaders/vertex.glsl");
    std::string vertexTexturedSrc = loadShaderSrc("src/shaders/vertex_textured.glsl");
    // FRAGS
    std::string fragSrc = loadShaderSrc("src/shaders/frag_rectangle.glsl");
    std::string fragPhongTexturedSrc = loadShaderSrc("src/shaders/mult_phong_textured.glsl");
    std::string fragPhongMaterialSrc = loadShaderSrc("src/shaders/mult_phong_material.glsl");
    std::string fragPhongTexturedMaterialSrc = loadShaderSrc("src/shaders/mult_phong_textured_material.glsl");
    // SKYBOX shaders
    std::string skyboxVertexSrc = loadShaderSrc("src/shaders/skybox_vertex.glsl");
    std::string skyboxFragmentSrc = loadShaderSrc("src/shaders/skybox_fragment.glsl");
    std::string cubeVertexSrc = loadShaderSrc("src/shaders/cube_vertex.glsl");
    std::string cubeFragmentSrc = loadShaderSrc("src/shaders/cube_fragment.glsl");
    // NORMAL MAP STUFF
    std::string normalMapVertexSrc = loadShaderSrc("src/shaders/vertex_tan.glsl");
    std::string normalMapFragmentSrc = loadShaderSrc("src/shaders/mult_phong_t_m_normal.glsl");
    
    modelShader = std::make_unique<Shader>(vertexSrc.c_str(), fragSrc.c_str());
    plainShader = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragPhongTexturedSrc.c_str());
    textureShader = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragPhongTexturedSrc.c_str());
    skyboxShader = std::make_unique<Shader>(skyboxVertexSrc.c_str(), skyboxFragmentSrc.c_str());
    cubeShader = std::make_unique<Shader>(cubeVertexSrc.c_str(), cubeFragmentSrc.c_str());
    //
    modelShader2 = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragPhongMaterialSrc.c_str());
    textureShader = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragPhongTexturedMaterialSrc.c_str());
    //
    normalMapShader = std::make_unique<Shader>(normalMapVertexSrc.c_str(), normalMapFragmentSrc.c_str());

    loginModel = ModelFactory::CreateLogin();
    houseModel = ModelFactory::CreateHouse();
    formulaModel = ModelFactory::CreateFormula();
    cupModel = ModelFactory::CreateCup();
    bicycleModel = ModelFactory::CreateBicycle();
    plainModel = ModelFactory::CreateNPlain();
    skyboxModel = ModelFactory::CreateCube();
    boxModel = ModelFactory::CreateBox();

    loginTexture = std::make_unique<Texture>("src/images/grunge.jpg");
    houseTexture = std::make_unique<Texture>("src/images/model.png");
    goldTexture = std::make_unique<Texture>("src/images/gold.jpg");
    grassTexture = std::make_unique<Texture>("src/images/grass.png");
    //
    boxAlbedoTexture = std::make_unique<Texture>("src/images/albedo2.png");
    boxNormalTexture = std::make_unique<Texture>("src/images/normalmap.png");
    
    std::vector<std::string> skyboxFaces = {
        "src/images/skybox/right.png",
        "src/images/skybox/left.png",
        "src/images/skybox/top.png",
        "src/images/skybox/bottom.png",
        "src/images/skybox/back.png",
        "src/images/skybox/front.png"
    };
    skyboxTexture = std::make_unique<Texture>(skyboxFaces);

    auto light1 = std::make_unique<Light>(
        glm::vec3(0.0f, 0.5f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        LightType::POINT
    );
    light1->setLinear(0.5f);
    light1->setQuadratic(0.4f);

    textureShader->addLight(light1.get());
    modelShader2->addLight(light1.get());
    normalMapShader->addLight(light1.get());

    light1->attach(modelShader2.get());
    light1->attach(textureShader.get());
    light1->attach(normalMapShader.get());
    lights.push_back(std::move(light1));

    modelShader2->updateAllLights();
    textureShader->updateAllLights();
    normalMapShader->updateAllLights();

    // ALL TRANSFORMS START HERE
    glm::mat4 customIdentity = glm::mat4(1.0f);
    customIdentity[3][3] = 20.0f;
    auto customWTransform = std::make_shared<TransformMatrix>(customIdentity);

    auto plainTransform = std::make_shared<TransformComposite>();
    plainTransform->add(customWTransform);
    plainTransform->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, -1.0f, 0.0f)));
    plainTransform->add(std::make_shared<TransformScale>(glm::vec3(1.0f, 1.0f, 1.0f)));
    addObject(plainModel.get(), textureShader.get(), plainTransform, grassTexture.get(), Material::Stone());
    
    auto objTransform = std::make_shared<TransformComposite>();
    objTransform->add(customWTransform);
    objTransform->add(std::make_shared<TransformTranslation>(glm::vec3(1.0f, 4.0f, -1.0f)));
    objTransform->add(std::make_shared<TransformRotation>(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    objTransform->add(std::make_shared<TransformScale>(glm::vec3(3.0f, 3.0f, 3.0f)));
    addObject(loginModel.get(), textureShader.get(), objTransform, loginTexture.get(), Material::Stone());
    
    auto objTransform2 = std::make_shared<TransformComposite>();
    objTransform2->add(customWTransform);
    objTransform2->add(std::make_shared<TransformTranslation>(glm::vec3(8.0f, -1.0f, 4.0f)));
    objTransform2->add(std::make_shared<TransformScale>(glm::vec3(1.0f, 1.0f, 1.0f)));
    addObject(houseModel.get(), textureShader.get(), objTransform2, houseTexture.get(), Material::Plastic());
    
    auto objTransform4 = std::make_shared<TransformComposite>();
    objTransform4->add(customWTransform);
    objTransform4->add(std::make_shared<TransformTranslation>(glm::vec3(4.0f, -1.0f, 4.0f)));
    objTransform4->add(std::make_shared<TransformRotation>(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
    objTransform4->add(std::make_shared<TransformScale>(glm::vec3(0.1f, 0.1f, 0.1f)));
    addObject(cupModel.get(), textureShader.get(), objTransform4, goldTexture.get(), Material::Gold());
    
    auto objTransform5 = std::make_shared<TransformComposite>();
    objTransform5->add(customWTransform);
    objTransform5->add(std::make_shared<TransformTranslation>(glm::vec3(-4.0f, -1.0f, -4.0f)));
    objTransform5->add(std::make_shared<TransformRotation>(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
    objTransform5->add(std::make_shared<TransformScale>(glm::vec3(0.1f, 0.1f, 0.1f)));
    addObject(bicycleModel.get(), textureShader.get(), objTransform5, goldTexture.get(), Material::Rubber());

    // BOX
    auto objTransform6 = std::make_shared<TransformComposite>();
    objTransform6->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, 0.0f, -1.0f)));
    objTransform6->add(std::make_shared<TransformScale>(glm::vec3(0.1f, 0.1f, 0.1f)));
    addObjectWithNormalMap(boxModel.get(), normalMapShader.get(), objTransform6, 
                          boxAlbedoTexture.get(), boxNormalTexture.get(), 
                          Material::Stone(), 2);

    // MOVING FORMULA SECTION HERE
    float a = 1.0f;
    float b = 1.0f;
    std::vector<glm::vec3> bezierPoints = {
        {0,0,-b}, {a,0,-b}, {a,0,b}, {0,0,b},
        {-a,0,b}, {-a,0,-b}, {0,0,-b}
    };

    formulaBezierTrans = std::make_shared<TransformBezier>(bezierPoints, 0.0f);
    auto formulaTransform = std::make_shared<TransformComposite>();
    formulaTransform->add(formulaBezierTrans);
    formulaTransform->add(customWTransform);
    formulaTransform->add(std::make_shared<TransformRotation>(-270.0f, glm::vec3(0,1,0)));
    formulaTransform->add(std::make_shared<TransformTranslation>(glm::vec3(-2.0f, -1.0f, -2.0f)));
    formulaTransform->add(std::make_shared<TransformScale>(glm::vec3(0.1f, 0.1f, 0.1f)));
    
    formulaObjIdx = objects.size();
    addObject(formulaModel.get(), modelShader2.get(), formulaTransform, nullptr, Material::Metal());
}

void ModelScene::drawSkybox() {
    if (!attachedCamera) return;
    glDepthFunc(GL_LEQUAL);
    skyboxShader->use();
    
    glm::mat4 view = glm::mat4(glm::mat3(attachedCamera->getViewMat()));
    skyboxShader->SetUniform("view", view);
    skyboxShader->SetUniform("projection", attachedCamera->getProjMat());
    
    skyboxTexture->bind(0);
    skyboxShader->SetUniform("skybox", 0);
    skyboxModel->draw(GL_TRIANGLES);

    glDepthFunc(GL_LESS);
    glUseProgram(0);
}

void ModelScene::draw() {
    float time = static_cast<float>(glfwGetTime());
    
    if (formulaBezierTrans) {
        float t = fmod(time * bezierAnimSpeed, 1.0f);
        formulaBezierTrans->setParam(t);
    }
    
    textureShader->use();
    textureShader->SetUniform("useTexture", true);
    textureShader->updateAllLights();

    float r = 0.8f * (sin(time) + 1.0f);
    float g = 0.8f * (sin(time + 2.0f));
    float b = 0.8f * (sin(time + 4.0f));

    modelShader2->use();
    modelShader2->SetUniform("objectColor", glm::vec3(r, g, b));
    modelShader2->updateAllLights();
    
    normalMapShader->use();
    normalMapShader->updateAllLights();
    
    drawImpl();
    drawSkybox();
}


void ModelScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
    if (camera && skyboxShader) {
        camera->attach(skyboxShader.get());
    }
}

void ModelScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
    if (camera && skyboxShader) {
        camera->detach(skyboxShader.get());
    }
}