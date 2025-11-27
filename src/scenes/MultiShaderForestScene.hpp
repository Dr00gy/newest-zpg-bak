#pragma once
#include "BaseScene.hpp"
#include <memory>
#include <vector>

class MultiShaderForestScene : public BaseScene {
public:
    enum class EditMode {
        CREATION,
        DELETION,
        BEZIER_EDIT
    };

    MultiShaderForestScene() = default;
    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;
    void toggleFlashlight() { flashlightEnabled = !flashlightEnabled; }
    void toggleEditMode() { 
        editMode = static_cast<EditMode>((static_cast<int>(editMode) + 1) % 3);
        switch(editMode) {
            case EditMode::CREATION:
                std::cout << "Mode switched to: SHROOM CREATION" << std::endl;
                break;
            case EditMode::DELETION:
                std::cout << "Mode switched to: SHROOM DELETION" << std::endl;
                break;
            case EditMode::BEZIER_EDIT:
                std::cout << "Mode switched to: BEZIER PATH EDIT" << std::endl;
                break;
        }
    }
    EditMode getEditMode() const { return editMode; }
    bool isEditMode() const { return editMode == EditMode::CREATION; }
    void handleMouseClick(double xpos, double ypos, int width, int height);
    void updateMouseHover(double xpos, double ypos, int width, int height);
    void resetBezierPath();

private:
    std::unique_ptr<Shader> lambertShader;
    std::unique_ptr<Shader> phongShader;
    std::unique_ptr<Shader> blinnShader;
    std::unique_ptr<Shader> phongTexturedShader;
    std::unique_ptr<Shader> triangleShader;

    std::unique_ptr<Model> bushModel;
    std::unique_ptr<Model> treeModel;
    std::unique_ptr<Model> plainModel;
    std::unique_ptr<Model> sphereModel;
    std::unique_ptr<Model> shrekModel;
    std::unique_ptr<Model> fionaModel;
    std::unique_ptr<Model> toiletModel;
    std::unique_ptr<Model> shroomModel;
    
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
    std::vector<std::shared_ptr<TransformRotation>> fireflyRotations;
    
    struct ShroomObject {
        int objectIndex;
        glm::vec3 position;
    };
    std::vector<ShroomObject> shroomObjects;
    
    bool flashlightEnabled = true;
    EditMode editMode = EditMode::CREATION;
    int hoveredShroomIndex = -1;
    
    std::shared_ptr<TransformBezier> shrekBezierTrans;
    int shrekObjectIndex;
    float bezierAnimSpeed = 0.15f;
    std::vector<glm::vec3> bezierControlPoints;
    
    glm::vec3 mouseToWorld(double xpos, double ypos, int width, int height);
    int findShroomAtPos(const glm::vec3& worldPos);
    void drawShroomsWStencil();
    void addShroomAtPos(const glm::vec3& worldPos);
    void deleteShroomAtPos(const glm::vec3& worldPos);
    void addBezierPoint(const glm::vec3& worldPos);
    void updateBezierPath();
};