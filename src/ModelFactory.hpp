#pragma once
#include "Model.hpp"
#include "objects/bushes.h"
#include "objects/cube.h"
#include "objects/sphere.h"
#include "objects/tree.h"
#include "objects/plain.h"

class ModelFactory {
public:
    static std::unique_ptr<Model> CreateBush() {
        return Model::LoadFromHeader(bushes, sizeof(bushes), 6, ModelType::NORMAL);
    }
    
    static std::unique_ptr<Model> CreateCube() {
        return Model::LoadFromHeader(cube, sizeof(cube), 3, ModelType::BASIC);
    }
    
    static std::unique_ptr<Model> CreateSphere() {
        return Model::LoadFromHeader(sphere, sizeof(sphere), 6, ModelType::NORMAL);
    }
    
    static std::unique_ptr<Model> CreateTree() {
        return Model::LoadFromHeader(tree, sizeof(tree), 6, ModelType::NORMAL);
    }
    
    static std::unique_ptr<Model> CreateTriangle() {
        float trianglePoints[] = {
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        };
        return std::make_unique<Model>(trianglePoints, sizeof(trianglePoints), 3, ModelType::BASIC);
    }
    
    static std::unique_ptr<Model> CreatePlain(ModelType type = ModelType::UV) {
        return Model::LoadFromHeader(plain, sizeof(plain), 8, type);
    }

    static std::unique_ptr<Model> CreatePlainSphere() {
        return Model::LoadFromFile("src/objects/planet.obj", ModelType::UV);
    }

    static std::unique_ptr<Model> CreateLogin() {
        return Model::LoadFromFile("src/objects/dalin.obj", ModelType::UV);
    }

    static std::unique_ptr<Model> CreateHouse() {
        return Model::LoadFromFile("src/objects/model.obj", ModelType::UV);
    }

    static std::unique_ptr<Model> CreateFormula() {
        return Model::LoadFromFile("src/objects/formula1.obj", ModelType::NORMAL);
    }

    static std::unique_ptr<Model> CreateBicycle() {
        return Model::LoadFromFile("src/objects/bicycle.obj", ModelType::UV);
    }

    static std::unique_ptr<Model> CreateCup() {
        return Model::LoadFromFile("src/objects/cup.obj", ModelType::UV);
    }

    static std::unique_ptr<Model> CreateShrek() {
        return Model::LoadFromFile("src/objects/shrek.obj", ModelType::UV);
    }

    static std::unique_ptr<Model> CreateFiona() {
        return Model::LoadFromFile("src/objects/fiona.obj", ModelType::UV);
    }

    static std::unique_ptr<Model> CreateToilet() {
        return Model::LoadFromFile("src/objects/toiled.obj", ModelType::UV);
    }

    static std::unique_ptr<Model> CreateShroom() {
        return Model::LoadFromFile("src/objects/mushromms.obj", ModelType::UV);
    }

    static std::unique_ptr<Model> CreateHammer() {
        return Model::LoadFromFile("src/objects/hammer.obj", ModelType::UV);
    }
};