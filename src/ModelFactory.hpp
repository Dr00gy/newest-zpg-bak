#pragma once
#include "Model.hpp"
#include "objects/bushes.h"
#include "objects/cube.h"
#include "objects/sphere.h"
#include "objects/tree.h"
#include "objects/plain.h"
#include <memory>

class ModelFactory {
public:
    static std::unique_ptr<Model> CreateBush();
    static std::unique_ptr<Model> CreateCube();
    static std::unique_ptr<Model> CreateSphere();
    static std::unique_ptr<Model> CreateTree();
    static std::unique_ptr<Model> CreateTriangle();
    static std::unique_ptr<Model> CreatePlain(ModelType type = ModelType::UV);
    static std::unique_ptr<Model> CreatePlainSphere();
    static std::unique_ptr<Model> CreateLogin();
    static std::unique_ptr<Model> CreateHouse();
    static std::unique_ptr<Model> CreateFormula();
    static std::unique_ptr<Model> CreateBicycle();
    static std::unique_ptr<Model> CreateCup();
    static std::unique_ptr<Model> CreateShrek();
    static std::unique_ptr<Model> CreateFiona();
    static std::unique_ptr<Model> CreateToilet();
    static std::unique_ptr<Model> CreateShroom();
    static std::unique_ptr<Model> CreateHammer();
};