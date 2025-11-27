#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "renderers/Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Controls.hpp"

#include "trans/Transform.hpp"

#include "scenes/BaseScene.hpp"
#include "scenes/RotatingTriangleScene.hpp"
#include "scenes/RandomObjectsScene.hpp"
#include "scenes/SymmetricalBallsScene.hpp"
#include "scenes/ForestScene.hpp"
#include "scenes/MultiShaderForestScene.hpp"
#include "scenes/SolarSystemScene.hpp"
#include "scenes/CorrectOneBallScene.hpp"
#include "scenes/WrongOneBallScene.hpp"
#include "scenes/ModelScene.hpp"
#include "scenes/WhackAMoleScene.hpp"

class App {
public:
    App();
    ~App();
    void init();
    void run();

private:
    GLFWwindow* window;
    int currentSceneIdx;
    float deltaTime;
    float lastFrame;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Controls> controls;
    std::vector<std::unique_ptr<BaseScene>> scenes;
};