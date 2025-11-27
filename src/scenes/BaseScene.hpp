#pragma once
#define GLM_ENABLE_EXPERIMENTAL
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
#include "../trans/TransformLinear.hpp"
#include "../trans/TransformBezier.hpp"
#include "../ModelFactory.hpp"
#include "../renderers/Light.hpp"
#include "../renderers/Shader.hpp"
#include "../Model.hpp"
#include "../renderers/Texture.hpp"
#include "../renderers/Material.hpp"
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
    void addObject(Model* model, Shader* shader, std::shared_ptr<Transform> transform, 
                   Texture* texture = nullptr, Material material = Material::Plastic()) {
        objects.push_back({model, shader, transform, texture, material});
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
            obj.shader->SetUniform("material.shininess", obj.material.getShininess());
            obj.shader->SetUniform("material.ambient", obj.material.getAmbient());
            obj.shader->SetUniform("material.diffuse", obj.material.getDiffuse());
            obj.shader->SetUniform("material.specular", obj.material.getSpecular());
            
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