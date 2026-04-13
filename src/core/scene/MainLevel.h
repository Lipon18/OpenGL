#ifndef MAINLEVEL_H
#define MAINLEVEL_H

#include "Scene.h"

class CameraComponent;
class Skybox;
class Pawn;

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

    Pawn* getPawn() { return m_Pawn; }

private:
    Pawn* m_Pawn = nullptr;
    std::vector<Actor*> m_PhysicsActors;
    Actor* m_FloorActor = nullptr;
    CameraComponent* m_MainCamera = nullptr;

    std::shared_ptr<Skybox> m_Skybox;
    std::shared_ptr<Shader> m_DebugShader;
    bool m_ShowDebugCollision = false;

    const float m_GRAVITY = -9.81f;

    void handlePhysics(float deltaTime);
    void renderDebugCollision();

    void setupCamera();
};

#endif // !MAINLEVEL_H