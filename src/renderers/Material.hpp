#pragma once
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Material {
public:
    Material(float shininess = 32.0f, float ambient = 0.2f, float diffuse = 0.8f, float specular = 1.0f);
    
    static Material Metal();
    static Material Rubber();
    static Material Plastic();
    static Material Stone();
    static Material Gold();
    static Material Shrek();
    static Material Fiona();
    static Material LoadFromMTL(const std::string& mtlPath, const std::string& materialName = "");
    
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