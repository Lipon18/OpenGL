#ifndef WINDOW_H
#define WINDOW_H

#include <string>

struct GLFWwindow;

class Window {
public:
	Window(unsigned int width, unsigned int height, const char* title);
	~Window();

	bool isOpen() const;
	void swapBuffers();
	void pollEvent();
	void setClearColor(float r, float g, float b, float a);
	void clear();
	void setFramebufferCallback(GLFWframebuffersizefun callback);
	GLFWwindow* getNativeWindow() const { return m_Window; }
	
	const  unsigned int w_Width;
	const unsigned int w_Height;
	//std::string w_title = "OpenGL";
	const char* w_title;
private:
	GLFWwindow* m_Window;
	bool initGLFW();
	bool initGLAD();
	float m_ClearColor[4];
};

#endif