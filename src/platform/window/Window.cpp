#include "pch.h"
#include "Window.h"

Window::Window(unsigned int width, unsigned int height, const char* title)
	: w_Width(width), w_Height(height), w_title(title), m_Window(nullptr) {

	if (!initGLFW()) {
		std::cout << "Failed to initailize GLFW" << std::endl;
		exit(-1);
	}

	m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!m_Window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(m_Window);
	setFramebufferCallback([](GLFWwindow* w, int width, int height) { glViewport(0, 0, width, height); });
	if (!initGLAD()) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glViewport(0, 0, width, height);
}

Window::~Window() {
	glfwTerminate();
}

bool Window::initGLFW() {
	if (!glfwInit()) return false;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return true;
}

bool Window::initGLAD() {
	return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

bool Window::isOpen() const {
	return !glfwWindowShouldClose(m_Window);
}

void Window::swapBuffers() {
	glfwSwapBuffers(m_Window);
}

void Window::pollEvent() {
	glfwPollEvents();
}

void Window::clear() {
	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setClearColor(float r, float g, float b, float a) {
	m_ClearColor[0] = r;
	m_ClearColor[1] = g;
	m_ClearColor[2] = b;
	m_ClearColor[3] = a;
}

void Window::setFramebufferCallback(GLFWframebuffersizefun callback) {
	glfwSetFramebufferSizeCallback(m_Window, callback);
}
