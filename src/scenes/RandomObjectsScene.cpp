#include "RandomObjectsScene.hpp"

#include <cstdlib>
#include <ctime>
#include <string>

void RandomObjectsScene::init() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::string vertexSrc        = loadShaderSource("src/shaders/vertex.glsl");
    std::string fragTriangleSrc  = loadShaderSource("src/shaders/frag_triangle.glsl");
    std::string fragRectangleSrc = loadShaderSource("src/shaders/frag_rectangle.glsl");
    std::string fragGreenSrc     = loadShaderSource("src/shaders/frag_green.glsl");

    shader1 = std::make_unique<Shader>(vertexSrc.c_str(), fragTriangleSrc.c_str());
    shader2 = std::make_unique<Shader>(vertexSrc.c_str(), fragRectangleSrc.c_str());
    shader3 = std::make_unique<Shader>(vertexSrc.c_str(), fragGreenSrc.c_str());

    cubeModel = ModelFactory::CreateCube();
    sphereModel = ModelFactory::CreateSphere();

    for (int i = 0; i < 10; ++i) {
        auto cubeTransform = std::make_shared<TransformComposite>();
        cubeTransform->add(std::make_shared<TransformTranslation>(
            glm::vec3(
                ((rand() % 200) / 50.0f) - 2.0f,
                ((rand() % 200) / 50.0f) - 1.0f,
                ((rand() % 200) / 50.0f) - 2.0f
            )
        ));
        cubeTransform->add(std::make_shared<TransformRotation>(
            static_cast<float>(rand() % 360), glm::vec3(0.0f, 1.0f, 0.0f)
        ));
        cubeTransform->add(std::make_shared<TransformScale>(glm::vec3(0.5f, 0.5f, 0.5f)));
        Shader* chosenShader = (rand() % 2 == 0) ? shader2.get() : shader3.get();
        addObject(cubeModel.get(), chosenShader, cubeTransform);

        auto sphereTransform = std::make_shared<TransformComposite>();
        sphereTransform->add(std::make_shared<TransformTranslation>(
            glm::vec3(
                ((rand() % 200) / 50.0f) - 2.0f,
                ((rand() % 200) / 50.0f) - 1.0f,
                ((rand() % 200) / 50.0f) - 2.0f
            )
        ));
        sphereTransform->add(std::make_shared<TransformRotation>(
            static_cast<float>(rand() % 360), glm::vec3(1.0f, 0.0f, 0.0f)
        ));
        sphereTransform->add(std::make_shared<TransformScale>(glm::vec3(0.5f, 0.5f, 0.5f)));
        addObject(sphereModel.get(), shader1.get(), sphereTransform);
    }
}

void RandomObjectsScene::draw() {
    drawImpl();
}

void RandomObjectsScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void RandomObjectsScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}