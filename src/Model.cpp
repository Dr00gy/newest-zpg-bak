#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>

static inline void bindAttributes(ModelType type, GLuint VAO, GLuint VBO, int strideFloats) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);

    if (type == ModelType::BASIC) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
        glBindVertexArray(0);
        return;
    }

    if (type == ModelType::NORMAL) {
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(2 * sizeof(float)));

        glBindVertexArray(0);
        return;
    }

    if (type == ModelType::UV) {
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));

        glBindVertexArray(0);
        return;
    }
    glBindVertexArray(0);
}

static inline void extractMeshData(const aiMesh* mesh, std::vector<float>& data) {
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            unsigned int index = face.mIndices[j];

            data.push_back(mesh->mVertices[index].x);
            data.push_back(mesh->mVertices[index].y);
            data.push_back(mesh->mVertices[index].z);

            if (mesh->mNormals) {
                data.push_back(mesh->mNormals[index].x);
                data.push_back(mesh->mNormals[index].y);
                data.push_back(mesh->mNormals[index].z);
            } else {
                data.push_back(0.0f);
                data.push_back(0.0f);
                data.push_back(0.0f);
            }
            if (mesh->mTextureCoords[0]) {
                data.push_back(mesh->mTextureCoords[0][index].x);
                data.push_back(mesh->mTextureCoords[0][index].y);
            } else {
                data.push_back(0.0f);
                data.push_back(0.0f);
            }
        }
    }
}

Model::Model(float* vertices, size_t size, int stride, ModelType type)
    : type(type)
{
    vertexCount = size / (stride * sizeof(float));
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    bindAttributes(type, VAO, VBO, stride);
    glBindVertexArray(0);
}

Model::~Model() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Model::draw(GLenum mode) {
    glBindVertexArray(VAO);
    glDrawArrays(mode, 0, vertexCount);
    glBindVertexArray(0);
}

std::unique_ptr<Model> Model::LoadFromHeader(float* vertices, size_t size, int stride, ModelType type) {
    return std::make_unique<Model>(vertices, size, stride, type);
}

std::unique_ptr<Model> Model::LoadFromFile(const std::string& path, ModelType type) {
    Assimp::Importer importer;
    unsigned int importOptions =
          aiProcess_Triangulate
        | aiProcess_OptimizeMeshes
        | aiProcess_JoinIdenticalVertices
        | aiProcess_CalcTangentSpace
        | aiProcess_GenSmoothNormals
        | aiProcess_GenUVCoords;

    const aiScene* scene = importer.ReadFile(path, importOptions);
    if (!scene || !scene->HasMeshes()) {
        std::cerr << "Assimp load failed: " << path << std::endl;
        return nullptr;
    }

    aiMesh* mesh = scene->mMeshes[0];
    std::vector<float> data;
    data.reserve(mesh->mNumFaces * 3 * 8);
    extractMeshData(mesh, data);

    if (data.empty()) {
        std::cerr << "No vertex data loaded from: " << path << std::endl;
        return nullptr;
    }

    const int stride = 8;
    const size_t byteSize = data.size() * sizeof(float);

    std::unique_ptr<Model> m = std::make_unique<Model>(data.data(), byteSize, stride, ModelType::UV);
    std::cout << "Loaded model from " << path << ": "
              << (data.size() / stride) << " vertices, "
              << (data.size() / stride / 3) << " triangles\n";

    return m;
}
