#include "ModelFactory.hpp"

std::unique_ptr<Model> ModelFactory::CreateBush() {
    return Model::LoadFromHeader(bushes, sizeof(bushes), 6, ModelType::NORMAL);
}

std::unique_ptr<Model> ModelFactory::CreateCube() {
    return Model::LoadFromHeader(cube, sizeof(cube), 3, ModelType::BASIC);
}

std::unique_ptr<Model> ModelFactory::CreateSphere() {
    return Model::LoadFromHeader(sphere, sizeof(sphere), 6, ModelType::NORMAL);
}

std::unique_ptr<Model> ModelFactory::CreateTree() {
    return Model::LoadFromHeader(tree, sizeof(tree), 6, ModelType::NORMAL);
}

std::unique_ptr<Model> ModelFactory::CreateTriangle() {
    float trianglePoints[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    return std::make_unique<Model>(trianglePoints, sizeof(trianglePoints), 3, ModelType::BASIC);
}

std::unique_ptr<Model> ModelFactory::CreatePlain() {
    return Model::LoadFromHeader(plain, sizeof(plain), 8, ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateNPlain() {
    return Model::LoadFromFile("src/objects/teren.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreatePlainSphere() {
    return Model::LoadFromFile("src/objects/planet.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateLogin() {
    return Model::LoadFromFile("src/objects/pytel.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateHouse() {
    return Model::LoadFromFile("src/objects/model.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateFormula() {
    return Model::LoadFromFile("src/objects/formula1.obj", ModelType::NORMAL);
}

std::unique_ptr<Model> ModelFactory::CreateBicycle() {
    return Model::LoadFromFile("src/objects/bicycle.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateCup() {
    return Model::LoadFromFile("src/objects/cup.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateShrek() {
    return Model::LoadFromFile("src/objects/shrek.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateFiona() {
    return Model::LoadFromFile("src/objects/fiona.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateToilet() {
    return Model::LoadFromFile("src/objects/toiled.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateShroom() {
    return Model::LoadFromFile("src/objects/mushromms.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateHammer() {
    return Model::LoadFromFile("src/objects/hammer.obj", ModelType::UV);
}

std::unique_ptr<Model> ModelFactory::CreateBox() {
    return Model::LoadFromFile("src/objects/Nmodel.obj", ModelType::TAN);
}