#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <set>
#include "../Utils.hpp"
#include "../trans/Transform.hpp"
#include "../trans/TransformComposite.hpp"
#include "../trans/TransformIdentity.hpp"
#include "../trans/TransformMatrix.hpp"
#include "../trans/TransformRotation.hpp"
#include "../trans/TransformScale.hpp"
#include "../trans/TransformTranslation.hpp"
#include "../ModelFactory.hpp"
#include "../Light.hpp"
#include "../Shader.hpp"
#include "../Model.hpp"
#include "../Texture.hpp"
#include "../DrawableObject.hpp"
#include "../Camera.hpp"

class BaseScene {
public:
    std::vector<DrawableObject> objects;
    Camera* attachedCamera = nullptr;

    virtual ~BaseScene() = default;
    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void attachToCamera(Camera* camera) = 0;
    virtual void detachFromCamera(Camera* camera) = 0;

    void addObject(Model* model, Shader* shader, std::shared_ptr<Transform> transform, Texture* texture = nullptr) {
        objects.push_back({model, shader, transform, texture});
    }
    
    void attachToCameraImpl(Camera* camera) {
        attachedCamera = camera;
        std::set<Shader*> uniqueShaders;
        for (auto& obj : objects) {
            if (obj.shader && uniqueShaders.find(obj.shader) == uniqueShaders.end()) {
                camera->attach(obj.shader);
                uniqueShaders.insert(obj.shader);
            }
        }
        camera->notify();
    }
    
    void detachFromCameraImpl(Camera* camera) {
        attachedCamera = nullptr;
        std::set<Shader*> uniqueShaders;
        for (auto& obj : objects) {
            if (obj.shader && uniqueShaders.find(obj.shader) == uniqueShaders.end()) {
                camera->detach(obj.shader);
                uniqueShaders.insert(obj.shader);
            }
        }
    }

    void drawImpl() {
        for (auto& obj : objects) {
            obj.shader->use();
            
            if (obj.texture) {
                obj.texture->bind(0);
                obj.shader->SetUniform("textureSampler", 0);
                obj.shader->SetUniform("useTexture", true);
            } else {
                obj.shader->SetUniform("useTexture", false);
            }
            
            glm::mat4 model = obj.transform->getMatrix();
            obj.shader->SetUniform("model", model);
            
            obj.model->draw(GL_TRIANGLES);
            
            if (obj.texture) {
                obj.texture->unbind();
            }
        }
        glUseProgram(0);
    }
};