#include "RotatingTriangleScene.hpp"
#include <GLFW/glfw3.h>

void RotatingTriangleScene::init() {
    std::string vertexSrc = loadShaderSrc("src/shaders/vertex.glsl");
    std::string fragTriangleSrc = loadShaderSrc("src/shaders/frag_triangle.glsl");
    
    shader = std::make_unique<Shader>(vertexSrc.c_str(), fragTriangleSrc.c_str());

    float trianglePoints[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    
    triangle = ModelFactory::CreateTriangle();
    
    transform = std::make_shared<TransformComposite>();
    transform->add(std::make_shared<TransformIdentity>());
    
    addObject(triangle.get(), shader.get(), transform);
}

void RotatingTriangleScene::draw() {
    float time = static_cast<float>(glfwGetTime());
    auto rotation = std::make_shared<TransformRotation>(time * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    
    transform = std::make_shared<TransformComposite>();
    transform->add(std::make_shared<TransformIdentity>());
    transform->add(rotation);
    
    if (!objects.empty()) {
        objects[0].transform = transform;
    }
    
    drawImpl();
}

void RotatingTriangleScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
}

void RotatingTriangleScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}