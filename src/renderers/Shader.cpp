#include "Shader.hpp"
#include "../Camera.hpp"
#include "Light.hpp"

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSrc, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSrc, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}

void Shader::use() const {
    glUseProgram(programID);
}

void Shader::update(Subject* subject) {
    use();
    Camera* camera = dynamic_cast<Camera*>(subject);
    if (camera && autoUpdateCamera) {
        SetUniform("view", camera->getViewMat());
        SetUniform("viewPos", camera->getPosition());
        
        if (camera->getLastChangeType() == Subject::ChangeType::PROJECTION) {
            SetUniform("projection", camera->getProjMat());
        }
    }
    
    Light* light = dynamic_cast<Light*>(subject);
    if (light && autoUpdateLight) {
        updateAllLights();
    }
    glUseProgram(0);
}

void Shader::updateAllLights() {
    use();
    int numLights = static_cast<int>(lights.size());
    GLint numLightsLocation = glGetUniformLocation(programID, "numLights");
    
    if (numLightsLocation != -1) {
        SetUniform("numLights", numLights);
        
        for (int i = 0; i < numLights; i++) {
            std::string base = "lights[" + std::to_string(i) + "]";
            SetUniform(base + ".position", lights[i]->getPosition());
            SetUniform(base + ".direction", lights[i]->getDirection());
            SetUniform(base + ".color", lights[i]->getColor());
            SetUniform(base + ".ambient", lights[i]->getAmbient());
            SetUniform(base + ".diffuse", lights[i]->getDiffuse());
            SetUniform(base + ".specular", lights[i]->getSpecular());
            SetUniform(base + ".type", static_cast<int>(lights[i]->getType()));
            SetUniform(base + ".cutOff", glm::cos(glm::radians(lights[i]->getCutOff())));
            SetUniform(base + ".outerCutOff", glm::cos(glm::radians(lights[i]->getOuterCutOff())));
            SetUniform(base + ".constant", lights[i]->getConstant());
            SetUniform(base + ".linear", lights[i]->getLinear());
            SetUniform(base + ".quadratic", lights[i]->getQuadratic());
        }
    }
    
    if (numLights > 0) {
        SetUniform("light.position", lights[0]->getPosition());
        SetUniform("light.direction", lights[0]->getDirection());
        SetUniform("light.color", lights[0]->getColor());
        SetUniform("light.ambient", lights[0]->getAmbient());
        SetUniform("light.diffuse", lights[0]->getDiffuse());
        SetUniform("light.specular", lights[0]->getSpecular());
        SetUniform("light.type", static_cast<int>(lights[0]->getType()));
        SetUniform("light.cutOff", glm::cos(glm::radians(lights[0]->getCutOff())));
        SetUniform("light.outerCutOff", glm::cos(glm::radians(lights[0]->getOuterCutOff())));
        SetUniform("light.constant", lights[0]->getConstant());
        SetUniform("light.linear", lights[0]->getLinear());
        SetUniform("light.quadratic", lights[0]->getQuadratic());
    }
    glUseProgram(0);
}

bool Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << type << " SHADER COMPILATION ERROR:\n" << infoLog << std::endl;
            return false;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "PROGRAM LINKING ERROR:\n" << infoLog << std::endl;
            return false;
        }
    }
    return true;
}

void Shader::SetUniform(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Shader::SetUniform(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetUniform(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}