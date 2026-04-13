#include "pch.h"
#include "App.h"
#include "renderer/shaders/Shader.h"
#include "platform/input/Keyboard.h"
#include "platform/input/Mouse.h"
#include "core/camera/Camera.h"
#include "renderer/lighting/DirLight.h"
#include "renderer/lighting/SpotLight.h"
#include "core/components/InputComponent.h"
#include "core/components/CameraComponent.h"
#include "core/scene/MainLevel.h"
#include "renderer/Renderer.h"
#include "renderer/RenderGlobals.h"
#include "core/actor/Pawn.h"

glm::mat4 gl_View;
glm::mat4 gl_Projection;

App::App() : m_Window(800, 600, "OpenGL") {
	init();
}

App::~App() {}

void App::init() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	m_ActiveScene = std::make_unique<MainLevel>();
	m_ActiveScene->init();

	//m_MainCamera = static_cast<MainLevel*>(m_ActiveScene.get())->getActiveCamera();
	MainLevel* mainLvl = static_cast<MainLevel*>(m_ActiveScene.get());
	if(mainLvl) {
		m_MainCamera = mainLvl->getActiveCamera();
		m_Pawn = mainLvl->getPawn();
	}

	initLighting();
	setupInput();

	m_Window.setClearColor(0.15f, 0.15f, 0.18f, 1.0f);
	glfwSetInputMode(m_Window.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void App::initLighting() {
	m_DirLight = std::make_unique<DirLight>(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	glm::vec3 pointLightPositions[] = { {0.7f, 0.2f, 2.0f}, {2.3f, -3.3f, -4.0f}, {-4.0f, 2.0f, -12.0f}, {0.0f, 0.0f, -3.0f} };
	for (int i = 0; i < 4; i++) {
		m_PointLights.emplace_back(
			pointLightPositions[i],
			glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f),
			1.0f, 0.09f, 0.032f
		);
	}
	if (m_MainCamera) {
		m_SpotLight = std::make_unique<SpotLight>(m_MainCamera->getPosition(), m_MainCamera->getFront(), 12.5f, 17.5f, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f));
	}
}

void App::setupInput() {
	auto* native = m_Window.getNativeWindow();
	if (!native) return;

	m_Keyboard = std::make_unique<Keyboard>(native);
	m_Mouse = std::make_unique<Mouse>(native);
	m_InputComponent = std::make_unique<InputComponent>();

	setMappingContext();

	m_InputComponent->bindAction({ "MoveForward", E_InputType::E_Axis1D, E_InputTrigger::E_Held, BIND_ACTION(App::moveForward) });
	m_InputComponent->bindAction({ "MoveRight", E_InputType::E_Axis1D, E_InputTrigger::E_Held, BIND_ACTION(App::moveRight) });
	m_InputComponent->bindAction({ "MouseDelta", E_InputType::E_Axis2D, E_InputTrigger::E_Held, BIND_ACTION(App::mouseLook) });
	m_InputComponent->bindAction({ "MouseScroll", E_InputType::E_Axis1D, E_InputTrigger::E_Held, BIND_ACTION(App::mouseScroll) });

	m_InputComponent->bindAction({ "Pause", E_InputType::E_Digital, E_InputTrigger::E_Pressed, BIND_ACTION(App::TogglePause) });
	m_InputComponent->bindAction({ "ToggleDepth", E_InputType::E_Digital, E_InputTrigger::E_Pressed, BIND_ACTION(App::ToggleDepthTest) });
	m_InputComponent->bindAction({ "ToggleCollision", E_InputType::E_Digital, E_InputTrigger::E_Pressed, BIND_ACTION(App::ToggleCollisionDebug) });
	m_InputComponent->bindAction({ "Quit", E_InputType::E_Digital, E_InputTrigger::E_Pressed, BIND_ACTION(App::quit) });

	m_InputComponent->bindAction({ "Eject", E_InputType::E_Digital, E_InputTrigger::E_Pressed, [this](const InputValue& v) { TogglePossession(); }});
}

void App::run() {
	while (m_Window.isOpen()) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		m_Window.pollEvent();

		m_Keyboard->update(m_Window.getNativeWindow());
		m_Mouse->update(m_Window.getNativeWindow());

		if (m_InputComponent)
			m_InputComponent->update(deltaTime);

		//processInput(deltaTime);
		update(deltaTime);
		render();

		m_Keyboard->postUpdate();
		m_Mouse->postUpdate();
		m_Window.swapBuffers();
	}
}

void App::update(float deltaTime) {
	if (!m_ActiveScene) return;

	m_ActiveScene->update(deltaTime);

	if(m_MainCamera && m_SpotLight) {
		m_SpotLight->setPosition(m_MainCamera->getPosition());
		m_SpotLight->setDirection(m_MainCamera->getFront());
	}
}

void App::render() {
	m_Window.clear();
	if (!m_ActiveScene) return;

	Camera* activeCamera = nullptr;
	if (m_IsPawnPossessed && m_Pawn) {
		activeCamera = m_Pawn->getCameraComponent()->getCamera();
	}
	else if (m_MainCamera) {
		activeCamera = m_MainCamera->getCamera();
	}

	if (!activeCamera) return;

	auto skybox = m_ActiveScene->getSkybox();
	float aspect = 800.0f / 600.0f;

	gl_View = activeCamera->GetViewMatrix();
	float fov = static_cast<float>(activeCamera->getCameraZoom());
	gl_Projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);

	Renderer::DrawSkybox(skybox, m_MainCamera, aspect);
	m_ActiveScene->render();
}

void App::setMappingContext() {
	// Maps keys
	m_InputComponent->mapKey("MoveForward", GLFW_KEY_W, 1.0f);
	m_InputComponent->mapKey("MoveForward", GLFW_KEY_S, -1.0f);
	m_InputComponent->mapKey("MoveRight", GLFW_KEY_A, -1.0f);
	m_InputComponent->mapKey("MoveRight", GLFW_KEY_D, 1.0f);

	m_InputComponent->mapKey("ToggleDepth", GLFW_KEY_F1);
	m_InputComponent->mapKey("ToggleCollision", GLFW_KEY_F2);
	m_InputComponent->mapKey("Pause", GLFW_KEY_P);
	m_InputComponent->mapKey("Quit", GLFW_KEY_ESCAPE);
	m_InputComponent->mapKey("Eject", GLFW_KEY_F8);
}

void App::moveForward(const InputValue& val) {
	if (m_IsPaused || !m_MainCamera) return;

	float scale = val.getFloat();
	if (scale > 0.0f) m_MainCamera->processKeyboard(E_Forward, deltaTime * scale);
	else if (scale < 0.0f) m_MainCamera->processKeyboard(E_Backward, deltaTime * -scale);
}

void App::moveRight(const  InputValue& val) {
	if (m_IsPaused || !m_MainCamera) return;

	float scale = val.getFloat();
	if (scale > 0.0f) m_MainCamera->processKeyboard(E_Right, deltaTime * scale);
	else if (scale < 0.0f) m_MainCamera->processKeyboard(E_Left, deltaTime * -scale);
}

void App::TogglePause(const InputValue& val) {
	m_IsPaused = !m_IsPaused;

	std::cout << (m_IsPaused ? "[SYSTEM] OpenGL application paused - Mouse released" : "[SYSTEM] OpenGL application resume - Mouse Captured") << std::endl;
	glfwSetInputMode(m_Window.getNativeWindow(), GLFW_CURSOR, m_IsPaused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void App::ToggleDepthTest(const InputValue& val) {
	Renderer::setShowDepth(!Renderer::getShowDepth());
}

void App::ToggleCollisionDebug(const InputValue& val) {
	m_ActiveScene->toggleDebugCollision();
}

void App::TogglePossession() {
	if (!m_Pawn) return;

	m_IsPawnPossessed = !m_IsPawnPossessed;

	m_Pawn->setPossessed(m_IsPawnPossessed);
	std::cout << (m_IsPawnPossessed ? "Possessing Pawn" : "Ejected Free Camera Active") << std::endl;
}

void App::mouseLook(const InputValue& val) {
	if (m_IsPaused || !m_MainCamera) return;

	glm::vec2 delta = val.getVec2();
	if (delta.x != 0.0f || delta.y != 0.0f) {
		m_MainCamera->processMouseMovement(delta.x, delta.y);
	}
}

void App::mouseScroll(const InputValue& val) {
	if (m_IsPaused || !m_MainCamera) return;

	float scrollOffset = val.getFloat();
	if (scrollOffset != 0.0f) {
		m_MainCamera->processMouseScroll(scrollOffset);
	}
}

void App::quit(const InputValue& val) {
	glfwSetWindowShouldClose(m_Window.getNativeWindow(), true);
}
