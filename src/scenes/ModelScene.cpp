#include "ModelScene.hpp"

#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

void ModelScene::init() {
    std::string vertexSrc = loadShaderSource("src/shaders/vertex.glsl");
    std::string vertexTexturedSrc = loadShaderSource("src/shaders/vertex_textured.glsl");
    std::string fragSrc = loadShaderSource("src/shaders/frag_rectangle.glsl");
    std::string fragSrc2 = loadShaderSource("src/shaders/frag_green.glsl");
    std::string fragPhongTexturedSrc = loadShaderSource("src/shaders/mult_phong_textured.glsl");
    std::string skyboxVertexSrc = loadShaderSource("src/shaders/skybox_vertex.glsl");
    std::string skyboxFragmentSrc = loadShaderSource("src/shaders/skybox_fragment.glsl");
    std::string cubeVertexSrc = loadShaderSource("src/shaders/cube_vertex.glsl");
    std::string cubeFragmentSrc = loadShaderSource("src/shaders/cube_fragment.glsl");

    modelShader = std::make_unique<Shader>(vertexSrc.c_str(), fragSrc.c_str());
    modelShader2 = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragPhongTexturedSrc.c_str());
    plainShader = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragPhongTexturedSrc.c_str());
    textureShader = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragPhongTexturedSrc.c_str());
    skyboxShader = std::make_unique<Shader>(skyboxVertexSrc.c_str(), skyboxFragmentSrc.c_str());
    cubeShader = std::make_unique<Shader>(cubeVertexSrc.c_str(), cubeFragmentSrc.c_str());

    loginModel = ModelFactory::CreateLogin();
    houseModel = ModelFactory::CreateHouse();
    formulaModel = ModelFactory::CreateFormula();
    cupModel = ModelFactory::CreateCup();
    bicycleModel = ModelFactory::CreateBicycle();
    plainModel = ModelFactory::CreatePlain();
    skyboxModel = ModelFactory::CreateCube();

    loginTexture = std::make_unique<Texture>("src/images/grunge.jpg");
    houseTexture = std::make_unique<Texture>("src/images/model.png");
    goldTexture = std::make_unique<Texture>("src/images/gold.jpg");
    grassTexture = std::make_unique<Texture>("src/images/grass.png");
    
    std::vector<std::string> skyboxFaces = {
        "src/images/skybox/right.png",
        "src/images/skybox/left.png",
        "src/images/skybox/top.png",
        "src/images/skybox/bottom.png",
        "src/images/skybox/front.png",
        "src/images/skybox/back.png"
    };
    skyboxTexture = std::make_unique<Texture>(skyboxFaces);

    auto light1 = std::make_unique<Light>(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        LightType::POINT
    );
    light1->setAmbient(0.05f);
    light1->setDiffuse(0.8f);
    light1->setSpecular(0.05f);

    textureShader->addLight(light1.get());
    modelShader2->addLight(light1.get());

    light1->attach(modelShader2.get());
    light1->attach(textureShader.get());

    lights.push_back(std::move(light1));

    modelShader2->updateAllLights();
    textureShader->updateAllLights();


    glm::mat4 customIdentity = glm::mat4(1.0f);
    customIdentity[3][3] = 20.0f;
    auto customWTransform = std::make_shared<TransformMatrix>(customIdentity);

    auto plainTransform = std::make_shared<TransformComposite>();
    plainTransform->add(customWTransform);
    plainTransform->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, -1.0f, 0.0f)));
    plainTransform->add(std::make_shared<TransformScale>(glm::vec3(20.0f, 1.0f, 20.0f)));
    addObject(plainModel.get(), plainShader.get(), plainTransform, grassTexture.get());


    auto objTransform = std::make_shared<TransformComposite>();
    objTransform->add(customWTransform);
    objTransform->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, -1.0f, 0.0f)));
    objTransform->add(std::make_shared<TransformScale>(glm::vec3(1.0f, 1.0f, 1.0f)));
    addObject(loginModel.get(), textureShader.get(), objTransform, loginTexture.get());


    auto objTransform2 = std::make_shared<TransformComposite>();
    objTransform2->add(customWTransform);
    objTransform2->add(std::make_shared<TransformTranslation>(glm::vec3(8.0f, -1.0f, 8.0f)));
    objTransform2->add(std::make_shared<TransformScale>(glm::vec3(1.0f, 1.0f, 1.0f)));
    addObject(houseModel.get(), textureShader.get(), objTransform2, houseTexture.get());


    auto objTransform3 = std::make_shared<TransformComposite>();
    objTransform3->add(customWTransform);
    objTransform3->add(std::make_shared<TransformTranslation>(glm::vec3(-2.0f, -1.0f, -2.0f)));
    objTransform3->add(std::make_shared<TransformScale>(glm::vec3(0.1f, 0.1f, 0.1f)));
    addObject(formulaModel.get(), modelShader2.get(), objTransform3);


    auto objTransform4 = std::make_shared<TransformComposite>();
    objTransform4->add(customWTransform);
    objTransform4->add(std::make_shared<TransformTranslation>(glm::vec3(4.0f, -1.0f, 4.0f)));
    objTransform4->add(std::make_shared<TransformRotation>(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
    objTransform4->add(std::make_shared<TransformScale>(glm::vec3(0.1f, 0.1f, 0.1f)));
    addObject(cupModel.get(), textureShader.get(), objTransform4, goldTexture.get());


    auto objTransform5 = std::make_shared<TransformComposite>();
    objTransform5->add(customWTransform);
    objTransform5->add(std::make_shared<TransformTranslation>(glm::vec3(-4.0f, -1.0f, -4.0f)));
    objTransform5->add(std::make_shared<TransformRotation>(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
    objTransform5->add(std::make_shared<TransformScale>(glm::vec3(0.1f, 0.1f, 0.1f)));
    addObject(bicycleModel.get(), textureShader.get(), objTransform5, goldTexture.get());
}

void ModelScene::drawSkybox() {
    if (!attachedCamera) return;
    
    if (isInsideSkybox) {
        glDepthFunc(GL_LEQUAL);
        skyboxShader->use();
        
        glm::mat4 view = glm::mat4(glm::mat3(attachedCamera->getViewMatrix()));
        skyboxShader->SetUniform("view", view);
        skyboxShader->SetUniform("projection", attachedCamera->getProjectionMatrix());
        
        skyboxTexture->bind(0);
        skyboxShader->SetUniform("skybox", 0);
        
        skyboxModel->draw(GL_TRIANGLES);
        glDepthFunc(GL_LESS);
        glUseProgram(0);
    } else {
        cubeShader->use();
        
        auto cubeTransform = std::make_shared<TransformComposite>();
        cubeTransform->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, 0.0f, 0.0f)));
        cubeTransform->add(std::make_shared<TransformScale>(glm::vec3(1.0f, 1.0f, 1.0f)));
        
        cubeShader->SetUniform("useTexture", true);
        
        glm::mat4 model = cubeTransform->getMatrix();
        cubeShader->SetUniform("model", model);
        cubeShader->SetUniform("view", attachedCamera->getViewMatrix());
        cubeShader->SetUniform("projection", attachedCamera->getProjectionMatrix());
        
        goldTexture->bind(0);
        cubeShader->SetUniform("textureSampler", 0);
        
        skyboxModel->draw(GL_TRIANGLES);
        
        goldTexture->unbind();
        glUseProgram(0);
    }
}



void ModelScene::draw() {
    drawSkybox();
    
    textureShader->use();
    textureShader->SetUniform("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    textureShader->SetUniform("shininess", 32.0f);
    textureShader->SetUniform("useTexture", true);
    textureShader->updateAllLights();

    modelShader2->use();
    modelShader2->SetUniform("objectColor", glm::vec3(0.400f, 0.137f, 0.936f));
    modelShader2->SetUniform("shininess", 32.0f);
    modelShader2->updateAllLights();
    
    drawImpl();
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

void ModelScene::toggleSkyboxMode() {
    isInsideSkybox = !isInsideSkybox;
    std::cout << "Skybox mode: " << (isInsideSkybox ? "Inside" : "Outside") << std::endl;
}
