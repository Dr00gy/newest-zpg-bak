#pragma once
#include <GL/glew.h>
#include <memory>
#include <string>

enum class ModelType {
    BASIC,
    NORMAL,
    UV,
    TAN
};

class Model {
public:
    Model(float* vertices, size_t size, int stride, ModelType type = ModelType::NORMAL);
    ~Model();

    void draw(GLenum mode = GL_TRIANGLES);
    static std::unique_ptr<Model> LoadFromHeader(float* vertices, size_t size, int stride, ModelType type = ModelType::NORMAL);
    static std::unique_ptr<Model> LoadFromFile(const std::string& path, ModelType type = ModelType::NORMAL);
    ModelType getType() const { return type; }

private:
    GLuint VAO;
    GLuint VBO;
    int vertexCount;
    ModelType type;
};
