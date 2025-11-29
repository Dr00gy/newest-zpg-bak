#include "Material.hpp"

Material::Material(float shininess, float ambient, float diffuse, float specular)
    : shininess(shininess), ambient(ambient), diffuse(diffuse), specular(specular) {}

Material Material::Metal() {
    return Material(128.0f, 0.1f, 0.6f, 1.0f);
}

Material Material::Rubber() {
    return Material(8.0f, 0.3f, 0.9f, 0.1f);
}

Material Material::Plastic() {
    return Material(32.0f, 0.2f, 0.8f, 0.5f);
}

Material Material::Stone() {
    return Material(4.0f, 0.3f, 0.7f, 0.1f);
}

Material Material::Gold() {
    return Material(256.0f, 0.25f, 0.4f, 1.0f);
}

Material Material::Shrek() {
    return LoadFromMTL("src/objects/shrek.mtl");
}

Material Material::Fiona() {
    return LoadFromMTL("src/objects/fiona.mtl");
}

Material Material::LoadFromMTL(const std::string& mtlPath, const std::string& materialName) {
    std::ifstream file(mtlPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open MTL file!!! " << mtlPath << std::endl;
        return Material::Plastic();
    }

    float shininess = 32.0f;
    float ambient = 0.2f;
    float diffuse = 0.8f;
    float specular = 1.0f;
    
    bool foundMaterial = materialName.empty();
    std::string line;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "newmtl") {
            if (!materialName.empty()) {
                std::string matName;
                iss >> matName;
                foundMaterial = (matName == materialName);
            }
        }
        else if (foundMaterial) {
            if (prefix == "Ns") {
                iss >> shininess;
            }
            else if (prefix == "Ka") {
                float r, g, b;
                iss >> r >> g >> b;
                ambient = (r + g + b) / 3.0f;
            }
            else if (prefix == "Kd") {
                float r, g, b;
                iss >> r >> g >> b;
                diffuse = (r + g + b) / 3.0f;
            }
            else if (prefix == "Ks") {
                float r, g, b;
                iss >> r >> g >> b;
                specular = (r + g + b) / 3.0f;
            }
        }
    }

    file.close();
    
    return Material(shininess, ambient, diffuse, specular);
}