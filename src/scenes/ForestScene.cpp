#include "ForestScene.hpp"

#include <cstdlib>
#include <ctime>
#include <string>

void ForestScene::init() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::string vertexSrc = loadShaderSource("src/shaders/vertex.glsl");
    std::string fragGreenSrc = loadShaderSource("src/shaders/frag_green.glsl");
    std::string fragTriangleSrc = loadShaderSource("src/shaders/frag_triangle.glsl");
    std::string fragRectangleSrc = loadShaderSource("src/shaders/frag_rectangle.glsl");

    bushShader = std::make_unique<Shader>(vertexSrc.c_str(), fragGreenSrc.c_str());
    treeShader = std::make_unique<Shader>(vertexSrc.c_str(), fragRectangleSrc.c_str());
    plainShader = std::make_unique<Shader>(vertexSrc.c_str(), fragTriangleSrc.c_str());

    bushModel = ModelFactory::CreateBush();
    treeModel = ModelFactory::CreateTree();
    plainModel = ModelFactory::CreatePlain();

    auto plainTransform = std::make_shared<TransformComposite>();
    plainTransform->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, -1.0f, 0.0f)));
    plainTransform->add(std::make_shared<TransformScale>(glm::vec3(20.0f, 1.0f, 20.0f)));
    addObject(plainModel.get(), plainShader.get(), plainTransform);

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
        
        addObject(bushModel.get(), bushShader.get(), bushTransform);
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
        
        addObject(treeModel.get(), treeShader.get(), treeTransform);
    }
}

void ForestScene::draw() {
    drawImpl();
}

void ForestScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void ForestScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}