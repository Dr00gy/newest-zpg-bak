#pragma once
#include <glm/glm.hpp>

class Material {
public:
    Material(float shininess = 32.0f, float ambient = 0.2f, float diffuse = 0.8f, float specular = 1.0f)
        : shininess(shininess), ambient(ambient), diffuse(diffuse), specular(specular) {}
    
    static Material Metal() {
        return Material(128.0f, 0.1f, 0.6f, 1.0f);
    }
    
    static Material Rubber() {
        return Material(8.0f, 0.3f, 0.9f, 0.1f);
    }
    
    static Material Plastic() {
        return Material(32.0f, 0.2f, 0.8f, 0.5f);
    }
    
    static Material Stone() {
        return Material(4.0f, 0.3f, 0.7f, 0.1f);
    }
    
    static Material Gold() {
        return Material(256.0f, 0.25f, 0.4f, 1.0f);
    }
    
    float getShininess() const { return shininess; }
    float getAmbient() const { return ambient; }
    float getDiffuse() const { return diffuse; }
    float getSpecular() const { return specular; }
    
    void setShininess(float s) { shininess = s; }
    void setAmbient(float a) { ambient = a; }
    void setDiffuse(float d) { diffuse = d; }
    void setSpecular(float s) { specular = s; }
    
private:
    float shininess;
    float ambient;
    float diffuse;
    float specular;
};