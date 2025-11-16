#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <vector>
#include "Observer.hpp"

class Light;

class Shader : public Observer {
public:
    Shader(const char* vertexSrc, const char* fragmentSrc);
    ~Shader();

    void use() const;
    void update(Subject* subject) override;

    void SetUniform(const std::string& name, bool value) const;
    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, float value) const;
    void SetUniform(const std::string& name, const glm::vec2& value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;
    void SetUniform(const std::string& name, const glm::vec4& value) const;
    void SetUniform(const std::string& name, const glm::mat4& mat) const;

    void addLight(Light* light) { 
        lights.push_back(light); 
    }
    
    void updateAllLights();
    
    void setAutoUpdateCamera(bool value) { autoUpdateCamera = value; }
    void setAutoUpdateLight(bool value) { autoUpdateLight = value; }

private:
    GLuint programID;
    bool checkCompileErrors(GLuint shader, std::string type);
    bool autoUpdateCamera = true;
    bool autoUpdateLight = true;
    std::vector<Light*> lights;
};