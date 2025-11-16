#pragma once
#include "BaseScene.hpp"
#include <memory>
#include <vector>

class MultiShaderForestScene : public BaseScene {
public:
    MultiShaderForestScene() = default;

    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;
    void toggleFlashlight() { flashlightEnabled = !flashlightEnabled; }
    void toggleEditMode() { 
        editMode = !editMode; 
        std::cout << "Edit mode switched to: " << (editMode ? "CREATION" : "DELETION") << std::endl;
    }
    bool isEditMode() const { return editMode; }
    void handleMouseClick(double xpos, double ypos, int width, int height);
    void updateMouseHover(double xpos, double ypos, int width, int height);

private:
    std::unique_ptr<Model> bushModel;
    std::unique_ptr<Model> treeModel;
    std::unique_ptr<Model> plainModel;
    std::unique_ptr<Model> sphereModel;
    std::unique_ptr<Model> shrekModel;
    std::unique_ptr<Model> fionaModel;
    std::unique_ptr<Model> toiletModel;
    std::unique_ptr<Model> shroomModel;
    
    std::unique_ptr<Shader> lambertShader;
    std::unique_ptr<Shader> phongShader;
    std::unique_ptr<Shader> blinnShader;
    std::unique_ptr<Shader> phongTexturedShader;
    std::unique_ptr<Shader> triangleShader;
    std::unique_ptr<Shader> stencilShader;
    
    std::unique_ptr<Texture> grassTexture;
    std::unique_ptr<Texture> shrekTexture;
    std::unique_ptr<Texture> fionaTexture;
    std::unique_ptr<Texture> toiletTexture;
    std::unique_ptr<Texture> shroomTexture;
    
    std::vector<std::unique_ptr<Light>> lights;
    std::unique_ptr<Light> directionalLight;
    std::unique_ptr<Light> flashlight;
    
    struct LightSphere {
        Light* light;
        size_t objectIndex;
    };
    std::vector<LightSphere> lightSpheres;
    
    struct ShroomObject {
        size_t objectIndex;
        glm::vec3 position;
    };
    std::vector<ShroomObject> shroomObjects;
    
    bool flashlightEnabled = true;
    bool editMode = true;
    int hoveredShroomIndex = -1;
    
    glm::vec3 unprojectMouseToWorld(double xpos, double ypos, int width, int height);
    int findShroomAtPosition(const glm::vec3& worldPos);
    void drawShroomsWithStencil();
    void addShroomAtPosition(const glm::vec3& worldPos);
    void deleteShroomAtPosition(const glm::vec3& worldPos);
};