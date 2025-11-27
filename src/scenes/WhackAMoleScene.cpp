#include "WhackAMoleScene.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

WhackAMoleScene::WhackAMoleScene()
    : score(0),
      enemiesSpawned(0),
      gameTime(0.0f),
      nextSpawnTime(1.0f),
      hoveredEnemyIdx(-1),
      rng(std::random_device{}()),
      holeDistrib(0, 5),
      enemyTypeDistrib(0, 2),
      spawnDelayDistrib(0.5f, 2.0f),
      moveChanceDistrib(0.0f, 1.0f) {}

void WhackAMoleScene::init() {
    std::string vertexTexturedSrc = loadShaderSrc("src/shaders/vertex_textured.glsl");
    std::string fragPhongTexturedSrc = loadShaderSrc("src/shaders/mult_phong_textured.glsl");
    std::string fragStencilSrc = loadShaderSrc("src/shaders/stencil.glsl");

    phongTexturedShader = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragPhongTexturedSrc.c_str());
    stencilShader = std::make_unique<Shader>(vertexTexturedSrc.c_str(), fragStencilSrc.c_str());

    cupModel = ModelFactory::CreateCup();
    shrekModel = ModelFactory::CreateShrek();
    fionaModel = ModelFactory::CreateFiona();
    shroomModel = ModelFactory::CreateShroom();
    hammerModel = ModelFactory::CreateHammer();
    plainModel = ModelFactory::CreatePlain();
    
    shrekTexture = std::make_unique<Texture>("src/images/shrek.png");
    fionaTexture = std::make_unique<Texture>("src/images/fiona.png");
    shroomTexture = std::make_unique<Texture>("src/images/hrib.jpg");
    grassTexture = std::make_unique<Texture>("src/images/swamp.jpg");
    hammerTexture = std::make_unique<Texture>("src/images/hammer.jpg");

    auto light1 = std::make_unique<Light>(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f), LightType::POINT);
    light1->setAmbient(0.3f);
    light1->setDiffuse(1.0f);
    light1->setSpecular(0.5f);
    
    auto light2 = std::make_unique<Light>(glm::vec3(5.0f, 3.0f, 5.0f), glm::vec3(1.0f), LightType::POINT);
    light2->setAmbient(0.2f);
    light2->setDiffuse(0.8f);
    light2->setSpecular(0.3f);
    
    phongTexturedShader->addLight(light1.get());
    phongTexturedShader->addLight(light2.get());
    light1->attach(phongTexturedShader.get());
    light2->attach(phongTexturedShader.get());
    lights.push_back(std::move(light1));
    lights.push_back(std::move(light2));
    phongTexturedShader->updateAllLights();

    auto plainTransform = std::make_shared<TransformComposite>();
    plainTransform->add(std::make_shared<TransformTranslation>(glm::vec3(0.0f, -2.0f, 0.0f)));
    plainTransform->add(std::make_shared<TransformScale>(glm::vec3(20.0f, 1.0f, 20.0f)));
    addObject(plainModel.get(), phongTexturedShader.get(), plainTransform, grassTexture.get());

    initializeHoles();
}

void WhackAMoleScene::initializeHoles() {
    const float startX = -6.0f;
    const float startZ = -3.0f;
    const float spacingX = 4.0f;
    const float spacingZ = 4.0f;
    const float yPos = -1.0f;

    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 3; ++col) {
            glm::vec3 pos(startX + col * spacingX, yPos, startZ + row * spacingZ);
            
            auto cupTransform = std::make_shared<TransformComposite>();
            cupTransform->add(std::make_shared<TransformTranslation>(pos));
            cupTransform->add(std::make_shared<TransformTranslation>(glm::vec3(3,0,1)));
            cupTransform->add(std::make_shared<TransformRotation>(-90.0f, glm::vec3(1,0,0)));
            cupTransform->add(std::make_shared<TransformRotation>(-180.0f, glm::vec3(0,1,0)));
            cupTransform->add(std::make_shared<TransformScale>(glm::vec3(0.5f)));

            int cupIndex = objects.size();
            addObject(cupModel.get(), phongTexturedShader.get(), cupTransform, nullptr);

            holes.push_back({pos, cupIndex, -1});
        }
    }
}

void WhackAMoleScene::spawnEnemy() {
    if (enemiesSpawned >= MAX_ENEMIES) return;

    std::vector<int> freeHoles;
    for (int i = 0; i < holes.size(); i++)
        if (holes[i].activeEnemyIdx == -1)
            freeHoles.push_back(i);

    if (freeHoles.empty()) return;
    int holeIdx = freeHoles[holeDistrib(rng) % freeHoles.size()];

    EnemyType type = static_cast<EnemyType>(enemyTypeDistrib(rng));
    Model* enemyModel = nullptr;
    Texture* enemyTexture = nullptr;
    float scaleValue = 1.0f;

    if (type == EnemyType::FIONA) {
        enemyModel = fionaModel.get();
        enemyTexture = fionaTexture.get();
        scaleValue = 2.0f;
    } else if (type == EnemyType::SHREK) {
        enemyModel = shrekModel.get();
        enemyTexture = shrekTexture.get();
        scaleValue = 2.0f;
    } else {
        enemyModel = shroomModel.get();
        enemyTexture = shroomTexture.get();
        scaleValue = 0.05f;
    }

    glm::vec3 pos = holes[holeIdx].position;
    pos.y += 0.5f;
    pos.x += 3.0f;
    pos.z += 1.0f;

    auto t = std::make_shared<TransformComposite>();
    if (type == EnemyType::MUSHROOM) t->add(std::make_shared<TransformTranslation>(glm::vec3(3,0,1)));
    t->add(std::make_shared<TransformTranslation>(pos));
    t->add(std::make_shared<TransformScale>(glm::vec3(scaleValue)));

    int objIndex = objects.size();
    addObject(enemyModel, phongTexturedShader.get(), t, enemyTexture);

    int idx = enemies.size();
    auto moveTransform = std::make_shared<TransformLinear>(pos, pos, 0.0f);
    
    enemies.push_back({type, objIndex, pos, gameTime, true, false, 0, false, false, moveTransform, 0, MOVE_DURATION});
    holes[holeIdx].activeEnemyIdx = idx;
    enemiesSpawned++;
}

void WhackAMoleScene::tryMoveEnemy(int i) {
    Enemy& e = enemies[i];
    if (!e.isAlive || e.isSquished || e.hasMoved || e.isMoving) return;
    if (moveChanceDistrib(rng) > 0.1f) return;

    int fromHole = -1;
    for (int h = 0; h < holes.size(); h++)
        if (holes[h].activeEnemyIdx == i)
            fromHole = h;
    if (fromHole == -1) return;

    std::vector<int> adjacentHoles;
    int row = fromHole / 3;
    int col = fromHole % 3;
    
    if (col > 0) adjacentHoles.push_back(fromHole - 1);
    if (col < 2) adjacentHoles.push_back(fromHole + 1);
    if (row > 0) adjacentHoles.push_back(fromHole - 3);
    if (row < 1) adjacentHoles.push_back(fromHole + 3);

    std::vector<int> freeAdjacent;
    for (int adj : adjacentHoles)
        if (holes[adj].activeEnemyIdx == -1)
            freeAdjacent.push_back(adj);
    if (freeAdjacent.empty()) return;

    int toHole = freeAdjacent[std::uniform_int_distribution<>(0, freeAdjacent.size() - 1)(rng)];
    startEnemyMov(i, fromHole, toHole);
}

void WhackAMoleScene::startEnemyMov(int idx, int fromHole, int toHole) {
    Enemy& e = enemies[idx];
    e.hasMoved = true;
    e.isMoving = true;

    holes[fromHole].activeEnemyIdx = -1;
    holes[toHole].activeEnemyIdx = idx;

    glm::vec3 startPos = e.position;
    glm::vec3 dest = holes[toHole].position;
    dest.y += 0.5f;
    dest.x += 3.0f;
    dest.z += 1.0f;
    
    e.moveTransform->setStartPoint(startPos);
    e.moveTransform->setEndPoint(dest);
    e.moveTransform->setParam(0.0f);

    e.moveStartTime = gameTime;
}

void WhackAMoleScene::updateEnemies(float dt) {
    for (int i = 0; i < enemies.size(); i++) {
        Enemy& e = enemies[i];
        if (!e.isAlive) continue;
        if (!e.isSquished && !e.hasMoved)
            tryMoveEnemy(i);

        if (e.isMoving) {
            float t = (gameTime - e.moveStartTime) / e.moveDuration;
            if (t >= 1.0f) {
                t = 1.0f;
                e.isMoving = false;
            }
            e.moveTransform->setParam(t);
            e.position = e.moveTransform->getPositionOnPath();

            auto tr = std::make_shared<TransformComposite>();
            if (e.type == EnemyType::MUSHROOM)
                tr->add(std::make_shared<TransformTranslation>(glm::vec3(3,0,1)));
            
            tr->add(e.moveTransform);

            float scale = (e.type == EnemyType::MUSHROOM ? 0.05f : 2.0f);
            tr->add(std::make_shared<TransformScale>(glm::vec3(scale)));

            objects[e.objectIndex].transform = tr;
            continue;
        }

        float alive = gameTime - e.spawnTime;
        if (e.isSquished) {
            float p = (gameTime - e.squishTime) / SQUISH_DURATION;
            if (p >= 1.0f) {
                e.isAlive = false;
                for (auto& h : holes)
                    if (h.activeEnemyIdx == i)
                        h.activeEnemyIdx = -1;
            } else {
                float scale = (e.type == EnemyType::MUSHROOM ? 0.05f : 2.0f);
                float ys = 1.0f - 0.8f * p;
                auto tr = std::make_shared<TransformComposite>();
                if (e.type == EnemyType::MUSHROOM)
                    tr->add(std::make_shared<TransformTranslation>(glm::vec3(3,0,1)));
                tr->add(std::make_shared<TransformTranslation>(e.position));
                tr->add(std::make_shared<TransformScale>(glm::vec3(scale, scale * ys, scale)));
                objects[e.objectIndex].transform = tr;
            }
        } else if (alive >= ENEMY_LIFETIME) {
            e.isAlive = false;
            for (auto& h : holes)
                if (h.activeEnemyIdx == i)
                    h.activeEnemyIdx = -1;
        }
    }
}

void WhackAMoleScene::updateHammers(float dt) {
    for (auto it = activeHammers.begin(); it != activeHammers.end();) {
        float age = gameTime - it->spawnTime;
        if (age >= HAMMER_LIFETIME) {
            int idx = it->objectIndex;
            if (idx < objects.size()) {
                objects.erase(objects.begin() + idx);
                for (auto& e : enemies)
                    if (e.objectIndex > idx) e.objectIndex--;
                for (auto& h : holes)
                    if (h.cupObjIdx > idx) h.cupObjIdx--;
                for (auto& he : activeHammers)
                    if (he.objectIndex > idx) he.objectIndex--;
            }
            it = activeHammers.erase(it);
        } else ++it;
    }
}

void WhackAMoleScene::update(float dt) {
    gameTime += dt;
    updateEnemies(dt);
    updateHammers(dt);

    if (gameTime >= nextSpawnTime && enemiesSpawned < MAX_ENEMIES) {
        spawnEnemy();
        nextSpawnTime = gameTime + spawnDelayDistrib(rng);
    }

    if (enemiesSpawned >= MAX_ENEMIES) {
        bool any = false;
        for (auto& e : enemies)
            if (e.isAlive)
                any = true;
        if (!any) {
            std::cout << "Final Score: " << score << std::endl;
        }
    }
}

glm::vec3 WhackAMoleScene::mouseToWorld(double xpos, double ypos, int W, int H) {
    if (!attachedCamera) return glm::vec3(0);
    float x = static_cast<float>(xpos);
    float y = static_cast<float>(H - ypos);
    glm::vec4 viewport(0, 0, W, H);
    glm::mat4 view = attachedCamera->getViewMat();
    glm::mat4 proj = attachedCamera->getProjMat();
    glm::vec3 nearP = glm::unProject({x, y, 0}, view, proj, viewport);
    glm::vec3 farP = glm::unProject({x, y, 1}, view, proj, viewport);
    
    glm::vec3 dir = glm::normalize(farP - nearP);
    float t = (0.5f - nearP.y) / dir.y;
    
    return nearP + dir * t;
}

int WhackAMoleScene::findEnemyAtPos(const glm::vec3& pos) {
    for (int i = 0; i < enemies.size(); i++) {
        auto& e = enemies[i];
        if (!e.isAlive || e.isSquished) continue;
        glm::vec3 d = pos - e.position;
        float dist = glm::length(glm::vec2(d.x, d.z));
        if (dist < SELECTION_RADIUS)
            return i;
    }
    return -1;
}

void WhackAMoleScene::hitEnemy(int idx, const glm::vec3& hitPos) {
    if (idx < 0 || idx >= enemies.size()) return;
    Enemy& e = enemies[idx];
    if (!e.isAlive || e.isSquished) return;

    e.isSquished = true;
    e.squishTime = gameTime;

    int pts = 0;
    if (e.type == EnemyType::FIONA) pts = 50;
    else if (e.type == EnemyType::SHREK) pts = 100;
    else pts = -50;

    score += pts;
    spawnHammer(hitPos);
}

void WhackAMoleScene::spawnHammer(const glm::vec3& pos) {
    auto t = std::make_shared<TransformComposite>();
    t->add(std::make_shared<TransformTranslation>(pos));
    t->add(std::make_shared<TransformTranslation>(glm::vec3(1,3.5f,-0.7f)));
    t->add(std::make_shared<TransformRotation>(-45, glm::vec3(0,0,1)));
    t->add(std::make_shared<TransformScale>(glm::vec3(0.2f)));
    int idx = objects.size();
    addObject(hammerModel.get(), phongTexturedShader.get(), t, hammerTexture.get());
    activeHammers.push_back({idx, gameTime, pos});
}

void WhackAMoleScene::handleMouseClick(double xpos, double ypos, int W, int H) {
    glm::vec3 wp = mouseToWorld(xpos, ypos, W, H);
    int idx = findEnemyAtPos(wp);
    if (idx >= 0) hitEnemy(idx, wp);
}

void WhackAMoleScene::updateMouseHover(double xpos, double ypos, int W, int H) {
    glm::vec3 wp = mouseToWorld(xpos, ypos, W, H);
    hoveredEnemyIdx = findEnemyAtPos(wp);
}

void WhackAMoleScene::draw() {
    phongTexturedShader->use();
    phongTexturedShader->SetUniform("objectColor", glm::vec3(1));
    phongTexturedShader->SetUniform("shininess", 32.0f);
    phongTexturedShader->updateAllLights();
    glUseProgram(0);

    for (int i = 0; i < objects.size(); i++) {
        bool dead = false;
        for (auto& e : enemies)
            if (i == e.objectIndex && !e.isAlive)
                dead = true;
        if (dead) continue;

        auto& obj = objects[i];
        obj.shader->use();

        if (obj.texture) {
            obj.texture->bind(0);
            obj.shader->SetUniform("textureSampler", 0);
            obj.shader->SetUniform("useTexture", true);
        } else obj.shader->SetUniform("useTexture", false);

        glm::mat4 model = obj.transform->getMatrix();
        obj.shader->SetUniform("model", model);
        obj.model->draw(GL_TRIANGLES);

        if (obj.texture) obj.texture->unbind();
    }
    glUseProgram(0);
}

void WhackAMoleScene::attachToCamera(Camera* camera) {
    attachToCameraImpl(camera);
    setupCamera();
}

void WhackAMoleScene::detachFromCamera(Camera* camera) {
    detachFromCameraImpl(camera);
}

void WhackAMoleScene::setupCamera() {
    if (!attachedCamera) return;
    attachedCamera->setPosition(glm::vec3(0,17,8));
    attachedCamera->setPitch(-70);
    attachedCamera->setYaw(-90);
}