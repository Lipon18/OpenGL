#ifndef MOUSE_H
#define MOUSE_H

#include "Input.h"

class Mouse : public Input {
public:
	Mouse(GLFWwindow* window);
	void update(GLFWwindow* window) override;
	void postUpdate() override;

	bool isButtonDown(int button);
	bool isButtonPressed(int button);

	static double getX() { return m_x; }
	static double getY() { return m_y; }
	static double getDX() { return m_dx; }
	static double getDY() { return m_dy; }
	static double getScrollY() { return m_scrollY; }

	static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
	static void buttonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
private:
	static double m_x, m_y, m_lastX, m_lastY, m_dx, m_dy;
	static double m_scrollY;
	static bool m_firstMouse;
	static bool m_buttons[GLFW_MOUSE_BUTTON_LAST];
	static bool m_buttonsPressed[GLFW_MOUSE_BUTTON_LAST];
};

#endif // !MOUSE_H
