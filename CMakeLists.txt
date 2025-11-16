cmake_minimum_required(VERSION 3.5.0)
project(kms VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address -Wall -Wextra -pedantic")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
find_package(assimp REQUIRED)

find_package(GLEW REQUIRED)
add_executable(kms
    src/Main.cpp
    src/App.cpp
    src/Shader.cpp
    src/Model.cpp
    src/Camera.cpp
    src/Controls.cpp
    src/Subject.cpp
    src/Texture.cpp
    src/scenes/RandomObjectsScene.cpp
    src/scenes/RotatingTriangleScene.cpp
    src/scenes/SymmetricalBallsScene.cpp
    src/scenes/ForestScene.cpp
    src/scenes/MultiShaderForestScene.cpp
    src/scenes/SolarSystemScene.cpp
    src/scenes/CorrectOneBallScene.cpp
    src/scenes/WrongOneBallScene.cpp
    src/scenes/ModelScene.cpp
    )
target_link_libraries(kms glfw GL X11 GLEW::GLEW assimp SOIL ${SDL2_LIBRARIES})