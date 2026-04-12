#ifndef SCENE_H
#define SCENE_H

class Actor;
class Shader;
class Skybox;

class Scene {
public:
	Scene() = default;
	virtual ~Scene();

	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
	virtual std::shared_ptr<Skybox> getSkybox() { return nullptr; }
	virtual void toggleDebugCollision() {}

	Actor* spawnActor();

protected:
	std::vector<std::unique_ptr<Actor>> m_Actors;
};

#endif // !SCENE_H