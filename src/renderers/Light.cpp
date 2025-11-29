#include "Light.hpp"

Light::Light(const glm::vec3& posOrDir, const glm::vec3& color, LightType type)
    : position(posOrDir), direction(glm::normalize(posOrDir)), color(color), type(type),
      ambient(0.2f), diffuse(0.8f), specular(1.0f),
      cutOff(12.5f), outerCutOff(17.5f), constant(1.0f), linear(0.09f), quadratic(0.032f) {}

void Light::setPosition(const glm::vec3& pos) {
    position = pos;
    notify();
}

void Light::setDirection(const glm::vec3& dir) {
    direction = glm::normalize(dir);
    notify();
}

void Light::setColor(const glm::vec3& col) {
    color = col;
    notify();
}

void Light::setAmbient(float amb) {
    ambient = amb;
    notify();
}

void Light::setDiffuse(float diff) {
    diffuse = diff;
    notify();
}

void Light::setSpecular(float spec) {
    specular = spec;
    notify();
}

void Light::setCutOff(float angle) {
    cutOff = angle;
    notify();
}

void Light::setOuterCutOff(float angle) {
    outerCutOff = angle;
    notify();
}

void Light::setLinear(float lin) {
    linear = lin;
    notify();
}

void Light::setQuadratic(float quad) {
    quadratic = quad;
    notify();
}

void Light::updateObservers() {
    notify();
}