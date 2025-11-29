#include "App.hpp"
#include <cstdlib>
#include <ctime>

App::App()
    : window(nullptr),
      currentSceneIdx(0),
      deltaTime(0.0f),
      lastFrame(0.0f) {}

App::~App() {
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

void App::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW!!!\n";
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "ZPG", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!!!\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW!!!\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));
    camera->updateAspectRatio(static_cast<float>(width), static_cast<float>(height));
    controls = std::make_unique<Controls>(window, camera.get());
    controls->setupCallbacks();
    
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h) {
        glViewport(0, 0, w, h);
        Controls* ctrl = static_cast<Controls*>(glfwGetWindowUserPointer(win));
        if (ctrl && ctrl->getCamera()) {
            ctrl->getCamera()->updateAspectRatio(static_cast<float>(w), static_cast<float>(h));
        }
    });

    scenes.push_back(std::make_unique<SolarSystemScene>());
    scenes.push_back(std::make_unique<MultiShaderForestScene>());
    scenes.push_back(std::make_unique<SymmetricalBallsScene>());
    scenes.push_back(std::make_unique<CorrectOneBallScene>());
    scenes.push_back(std::make_unique<WrongOneBallScene>());
    scenes.push_back(std::make_unique<ModelScene>());
    scenes.push_back(std::make_unique<WhackAMoleScene>());
    for (auto& scene : scenes) {
        scene->init();
        scene->attachToCamera(camera.get());
    }
}

void App::run() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        if (controls->shouldClose()) {
            glfwSetWindowShouldClose(window, true);
        }
        controls->procInput(deltaTime);
        controls->procSceneSwitch(currentSceneIdx, scenes.size());

        // SCENE SPECIFIC INPUTS START
        MultiShaderForestScene* forestScene = dynamic_cast<MultiShaderForestScene*>(scenes[currentSceneIdx].get());
        controls->procBatteryToggle(forestScene);
        controls->procEditModeToggle(forestScene);
        //
        WhackAMoleScene* whackAMoleScene = dynamic_cast<WhackAMoleScene*>(scenes[currentSceneIdx].get());
        if (whackAMoleScene) {
            whackAMoleScene->update(deltaTime);
        }
        controls->procWhackAMoleInput(whackAMoleScene);
        controls->procMouseHover(forestScene);
        //
        ModelScene* modelScene = dynamic_cast<ModelScene*>(scenes[currentSceneIdx].get());
        controls->procSkyboxToggle(modelScene);
        // SCENE SPECIFIC INPUTS END
        
        if (!scenes.empty()) {
            scenes[currentSceneIdx]->draw();
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}