#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "BaseComponent.h"
#include "platform/input/InputAction.h"

struct KeyBinding {
	int Key;
	float Scale;
};

class Keyboard;
class Mouse;

class InputComponent : public BaseComponent {
public:
	void bindAction(const InputAction& action);
	void mapKey(const std::string& name, int glfwKey, float scale = 1.0f);

	void update(float deltaTime) override;
private:
	std::vector<InputAction> m_Actions;
	std::unordered_map<std::string, std::vector<KeyBinding>> m_KeyMappings;
};

#endif // ! INPUTCOMPONENT_H