#ifndef ACTOR_H
#define ACTOR_H

class BaseComponent;
class Shader;

class Actor {
public:
	Actor();
	~Actor();

	template<typename T, typename... Args>
	T* addComponent(Args&&... args) {
		auto comp = std::make_unique<T>(std::forward<Args>(args)...);
		comp->setOwner(this);
		comp->init();
		T* ptr = comp.get();
		m_Components.push_back(std::move(comp));
		return ptr;
	}

	template<typename T>
	T* getComponent() {
		for (auto& comp : m_Components ) {
			if (T* ptr = dynamic_cast<T*>(comp.get()))
				if(ptr) return ptr;
		}
		return nullptr;
	}

	void updateComponents(float deltaTime);
	void renderComponents();

private:
	std::vector<std::unique_ptr<BaseComponent>> m_Components;

};

#endif // !ACTOR_H