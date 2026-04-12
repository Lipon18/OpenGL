#ifndef MAINLEVEL_H
#define MAINLEVEL_H

#include "Scene.h"

class CameraComponent;
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

    CameraComponent* getActiveCamera() { return m_MainCamera; }

private:
    std::vector<Actor*> m_CubeActors;
    //Actor* m_CubeActor = nullptr;
    Actor* m_FloorActor = nullptr;
    //Actor* m_SculptureActor = nullptr;
    CameraComponent* m_MainCamera = nullptr;

    std::shared_ptr<Skybox> m_Skybox;

    void setupCamera();
    void setupCube(std::shared_ptr<Shader> shader);
    void setupFloor(std::shared_ptr<Shader> shader);
    //void setupSculpture();
};

#endif // !MAINLEVEL_H