#include "pch.h"
#include "Mouse.h"

double Mouse::m_x = 0, Mouse::m_y = 0, Mouse::m_lastX = 0, Mouse::m_lastY = 0, Mouse::m_dx = 0, Mouse::m_dy = 0, Mouse::m_scrollY = 0;
bool Mouse::m_firstMouse = true;
bool Mouse::m_buttons[GLFW_MOUSE_BUTTON_LAST] = { false };
bool Mouse::m_buttonsPressed[GLFW_MOUSE_BUTTON_LAST] = { false };

Mouse::Mouse(GLFWwindow* window) {
	glfwSetCursorPosCallback(window, Mouse::cursorCallback);
	glfwSetMouseButtonCallback(window, Mouse::buttonCallback);
	glfwSetScrollCallback(window, Mouse::scrollCallback);
}

void Mouse::update(GLFWwindow* window) {}

void Mouse::postUpdate() {
	m_dx = 0;
	m_dy = 0;
	m_scrollY = 0;
	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) m_buttonsPressed[i] = false;
}

void Mouse::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
	m_x = xpos;
	m_y = ypos;

	if (m_firstMouse) {
		m_lastX = m_x;
		m_lastY = m_y;
		m_firstMouse = false;
	}
	m_dx = m_x - m_lastX;
	m_dy = m_lastY - m_y; // inverted screen condition
	m_lastX = m_x;
	m_lastY = m_y;
}

void Mouse::buttonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		m_buttons[button] = true;
		m_buttonsPressed[button] = true;
	}
	else if(action == GLFW_RELEASE) {
		m_buttons[button] = false;
	}
}

void Mouse::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	m_scrollY = yoffset;
}

bool Mouse::isButtonDown(int button) { return m_buttons[button]; }
bool Mouse::isButtonPressed(int button) { return m_buttonsPressed[button]; }