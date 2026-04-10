#ifndef INPUTACTION_H
#define INPUTACTION_H

#include <functional>
#include <variant>
#include <string>
#include <glm/glm.hpp>

enum class E_InputType {
   E_Digital,   // key/button
   E_Axis1D,    // float, e.g., scroll
   E_Axis2D     // vec2, e.g., mouse movement
};

enum class E_InputTrigger {
	E_Pressed,
	E_Released,
	E_Held
};

struct InputValue {
	std::variant<bool, float, glm::vec2> Value;

	bool getBool() const { return std::get<bool>(Value); }
	float getFloat() const { return std::get<float>(Value); }
	glm::vec2 getVec2() const { return std::get<glm::vec2>(Value); }
};

struct InputAction {
	std::string Name;
	E_InputType Type;
	E_InputTrigger Trigger = E_InputTrigger::E_Pressed;
	std::function<void(const InputValue&)> Callback;

	InputAction(const std::string& name, E_InputType type, E_InputTrigger trigger,std::function<void(const InputValue&)> callback) :
		Name(name), Type(type), Trigger(trigger), Callback(callback) {}
};

#endif // !INPUTACTION_H
