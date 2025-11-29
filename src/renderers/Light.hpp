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
    Light(const glm::vec3& posOrDir, const glm::vec3& color = glm::vec3(1.0f), LightType type = LightType::POINT);
    
    void setPosition(const glm::vec3& pos);
    void setDirection(const glm::vec3& dir);
    void setColor(const glm::vec3& col);
    void setAmbient(float amb);
    void setDiffuse(float diff);
    void setSpecular(float spec);
    void setCutOff(float angle);
    void setOuterCutOff(float angle);
    void setLinear(float lin);
    void setQuadratic(float quad);
    void updateObservers();
    
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