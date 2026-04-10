#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include "core/actor/Actor.h"

class BaseComponent {
public:
	BaseComponent() : owner(nullptr) {}
	virtual ~BaseComponent() = default;

	virtual void init() {};
	virtual void update(float deltaTime) {};
	virtual void render() {};

	const std::string getComponentName() const { return name; }
	void setOwner(Actor* newOwner) { owner = newOwner; }

protected:
	Actor* owner;
	std::string name = "";
};

#endif // !BASECOMPONENT_H