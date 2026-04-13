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

	template<typename T = Actor, typename... Args>
	inline T* spawnActor(Args&&... args) {
		static_assert(std::is_base_of<Actor, T>::value, "T must derive from Actor");

		auto actor = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = actor.get();
		m_Actors.push_back(std::move(actor));
		return ptr;
	}

protected:
	std::vector<std::unique_ptr<Actor>> m_Actors;
};

#endif // !SCENE_H