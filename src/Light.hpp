#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Subject.hpp"

class Shader;

enum class LightType {
    POINT,
    DIRECTIONAL,
    REFLECTOR
};

class Light : public Subject {
public:
    Light(const glm::vec3& positionOrDirection, const glm::vec3& color = glm::vec3(1.0f), LightType type = LightType::POINT)
        : position(positionOrDirection), direction(glm::normalize(positionOrDirection)), color(color), type(type),
          ambient(0.2f), diffuse(0.8f), specular(1.0f),
          cutOff(12.5f), outerCutOff(17.5f), constant(1.0f), linear(0.09f), quadratic(0.032f) {}
    
    void setPosition(const glm::vec3& pos) {
        position = pos;
        notify();
    }
    
    void setDirection(const glm::vec3& dir) {
        direction = glm::normalize(dir);
        notify();
    }
    
    void setColor(const glm::vec3& col) {
        color = col;
        notify();
    }
    
    void setAmbient(float amb) {
        ambient = amb;
        notify();
    }
    
    void setDiffuse(float diff) {
        diffuse = diff;
        notify();
    }
    
    void setSpecular(float spec) {
        specular = spec;
        notify();
    }
    
    void setCutOff(float angle) {
        cutOff = angle;
        notify();
    }
    
    void setOuterCutOff(float angle) {
        outerCutOff = angle;
        notify();
    }
    
    void updateObservers() {
        notify();
    }
    
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getDirection() const { return direction; }
    glm::vec3 getColor() const { return color; }
    LightType getType() const { return type; }
    float getAmbient() const { return ambient; }
    float getDiffuse() const { return diffuse; }
    float getSpecular() const { return specular; }
    float getCutOff() const { return cutOff; }
    float getOuterCutOff() const { return outerCutOff; }
    float getConstant() const { return constant; }
    float getLinear() const { return linear; }
    float getQuadratic() const { return quadratic; }
    
private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    LightType type;
    float ambient;
    float diffuse;
    float specular;
    
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
};