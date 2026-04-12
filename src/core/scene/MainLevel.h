#ifndef MAINLEVEL_H
#define MAINLEVEL_H

#include "Scene.h"

class CameraComponent;
class Collision;
class Mesh;
class Skybox;

class MainLevel : public Scene {
public:
    MainLevel();
    virtual ~MainLevel();
    void init() override;
    void update(float deltaTime) override;
    void render() override;
    virtual std::shared_ptr<Skybox> getSkybox() override { return m_Skybox; }

    void toggleDebugCollision() { m_ShowDebugCollision = !m_ShowDebugCollision; }

    CameraComponent* getActiveCamera() { return m_MainCamera; }

private:
    //std::vector<Actor*> m_CubeActors;
    //Actor* m_CubeActor = nullptr;
    //Actor* m_SphereActor = nullptr;
    //Actor* m_SculptureActor = nullptr;

    std::vector<Actor*> m_PhysicsActors;
    Actor* m_FloorActor = nullptr;
    CameraComponent* m_MainCamera = nullptr;

    glm::vec3 m_CubeVelocity = glm::vec3(0.0f);
    glm::vec3 m_SphereVelocity = glm::vec3(0.5f, 0.0f, 0.0f);
    const float m_GRAVITY = -9.81f;

    std::shared_ptr<Skybox> m_Skybox;

    void handlePhysics(float deltaTime);
    void renderDebugCollision();
    bool m_ShowDebugCollision = false;
    std::shared_ptr<Shader> m_DebugShader;

    void setupCamera();
    void setupCube(std::shared_ptr<Shader> shader);
    void setupFloor(std::shared_ptr<Shader> shader);
    void setupSphere(std::shared_ptr<Shader> shader);
    //void setupSculpture();
};

#endif // !MAINLEVEL_H