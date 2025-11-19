#pragma once
#include "BaseScene.hpp"
#include <memory>
#include <vector>
#include <random>

class WhackAMoleScene : public BaseScene {
public:
    WhackAMoleScene();

    void init() override;
    void draw() override;
    void attachToCamera(Camera* camera) override;
    void detachFromCamera(Camera* camera) override;
    void setupCamera();
    void handleMouseClick(double xpos, double ypos, int width, int height);
    void updateMouseHover(double xpos, double ypos, int width, int height);
    void update(float deltaTime);
    int getScore() const { return score; }

private:
    enum class EnemyType {
        FIONA,
        SHREK,
        MUSHROOM
    };

    struct Enemy {
        EnemyType type;
        size_t objectIndex;
        glm::vec3 position;
        float spawnTime;
        bool isAlive;
        bool isSquished;
        float squishTime;
        bool hasMoved;
        bool isMoving;
        glm::vec3 moveStartPos;
        glm::vec3 moveEndPos;
        float moveStartTime;
        float moveDuration;
    };

    struct HolePosition {
        glm::vec3 position;
        size_t cupObjectIndex;
        int activeEnemyIndex;
    };

    struct HammerEffect {
        size_t objectIndex;
        float spawnTime;
        glm::vec3 position;
    };

    std::unique_ptr<Model> cupModel;
    std::unique_ptr<Model> shrekModel;
    std::unique_ptr<Model> fionaModel;
    std::unique_ptr<Model> shroomModel;
    std::unique_ptr<Model> hammerModel;
    std::unique_ptr<Model> plainModel;
    
    std::unique_ptr<Shader> phongTexturedShader;
    std::unique_ptr<Shader> stencilShader;
    
    std::unique_ptr<Texture> shrekTexture;
    std::unique_ptr<Texture> fionaTexture;
    std::unique_ptr<Texture> shroomTexture;
    std::unique_ptr<Texture> grassTexture;
    std::unique_ptr<Texture> hammerTexture;
    
    std::vector<std::unique_ptr<Light>> lights;
    
    std::vector<HolePosition> holes;
    std::vector<Enemy> enemies;
    std::vector<HammerEffect> activeHammers;
    
    int score;
    int enemiesSpawned;
    float gameTime;
    float nextSpawnTime;
    int hoveredEnemyIndex;
    
    std::mt19937 rng;
    std::uniform_int_distribution<int> holeDistribution;
    std::uniform_int_distribution<int> enemyTypeDistribution;
    std::uniform_real_distribution<float> spawnDelayDistribution;
    std::uniform_real_distribution<float> moveChanceDistribution;
    
    const int MAX_ENEMIES = 20;
    const float ENEMY_LIFETIME = 3.0f;
    const float HAMMER_LIFETIME = 0.2f;
    const float SQUISH_DURATION = 0.3f;
    const float SELECTION_RADIUS = 1.0f;
    const float MOVE_DURATION = 0.6f;

    void initializeHoles();
    void spawnEnemy();
    void updateEnemies(float deltaTime);
    void updateHammers(float deltaTime);
    void tryMoveEnemy(size_t enemyIndex);
    void startEnemyMov(size_t enemyIndex, int holeFrom, int holeTo);
    glm::vec3 unprojectMouseToWorld(double xpos, double ypos, int fbW, int fbH);
    int findEnemyAtPos(const glm::vec3& worldPos);
    void hitEnemy(int enemyIndex, const glm::vec3& hitPosition);
    void spawnHammer(const glm::vec3& position);
};
