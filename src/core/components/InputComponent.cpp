#include "pch.h"
#include "InputComponent.h"
#include "platform/input/Keyboard.h"
#include "platform/input/Mouse.h"

void InputComponent::bindAction(const InputAction& action) {
	m_Actions.push_back(action);
}

void InputComponent::mapKey(const std::string& name, int glfwKey, float scale) {
	m_KeyMappings[name].push_back({ glfwKey, scale });
}

void InputComponent::update(float deltaTime) {
	for (auto& action : m_Actions) {

		if (m_KeyMappings.find(action.Name) != m_KeyMappings.end()) {
			float axisValue = 0.0f;
			bool triggered = false;

			for (const auto& binding : m_KeyMappings[action.Name]) {
				bool keyActive = false;
				switch (action.Trigger) {
				case E_InputTrigger::E_Pressed:  keyActive = Keyboard::getKeyPressed(binding.Key); break;
				case E_InputTrigger::E_Released: keyActive = Keyboard::getKeyReleased(binding.Key); break;
				case E_InputTrigger::E_Held:     keyActive = Keyboard::getKeyDown(binding.Key); break;
				}
				if (keyActive) {
					triggered = true;
					axisValue += binding.Scale;
				}
			}
			if (triggered) {
				if (action.Type == E_InputType::E_Digital) {
					action.Callback(InputValue{ true });
				}
				else if (action.Type == E_InputType::E_Axis1D) {
					action.Callback(InputValue{ axisValue });
				}
			}
		}

		if (action.Type == E_InputType::E_Axis1D && action.Name == "MouseScroll") {
			float value = static_cast<float>(Mouse::getScrollY());
			if (value != 0.0f) action.Callback(InputValue{ value });
		}
		else if (action.Type == E_InputType::E_Axis2D && action.Name == "MouseDelta") {
			glm::vec2 delta(Mouse::getDX(), Mouse::getDY());
			if (delta != glm::vec2(0.0f)) action.Callback(InputValue{ delta });
		}
	}
}