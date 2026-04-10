#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Input.h"

class Keyboard : public Input {
public:
	Keyboard(GLFWwindow* window);
	void update(GLFWwindow* window) override;
	void postUpdate() override;

	/*bool isKeyDown(int key);
	bool isKeyPressed(int key);
	bool isKeyReleased(int key);*/

	static bool getKeyDown(int key) { return m_keys[key]; }
	static bool getKeyPressed(int key) { return m_keysPressed[key]; }
	static bool getKeyReleased(int key) { return m_keysReleased[key]; }

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	static bool m_keys[GLFW_KEY_LAST];
	static bool m_keysPressed[GLFW_KEY_LAST];
	static bool m_keysReleased[GLFW_KEY_LAST];
};
#endif // !KEYBOARD_H
