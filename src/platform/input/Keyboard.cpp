#include "pch.h"
#include "Keyboard.h"

bool Keyboard::m_keys[GLFW_KEY_LAST] = { false };
bool Keyboard::m_keysPressed[GLFW_KEY_LAST] = { false };
bool Keyboard::m_keysReleased[GLFW_KEY_LAST] = { false };

Keyboard::Keyboard(GLFWwindow* window) {
	glfwSetKeyCallback(window, Keyboard::keyCallback);
}

void Keyboard::update(GLFWwindow* window) {
	//if (isKeyPressed(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, true);
}

void Keyboard::postUpdate() {
	// rest the "one-frame" triggers
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		m_keysPressed[i] = false;
		m_keysReleased[i] = false;
	}
}

void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key < 0 || key >= GLFW_KEY_LAST) return;

	if (action == GLFW_PRESS) {
		m_keys[key] = true;
		m_keysPressed[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		m_keys[key] = false;
		m_keysReleased[key] = true;
	}
}