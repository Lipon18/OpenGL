#ifndef APP_H
#define APP_H

#include "platform/window/Window.h"
#include "renderer/lighting/PointLight.h"

class Camera;
class Model;
class Mesh;
class Keyboard;
class Mouse;
class DirLight;
class SpotLight;
class Actor;
class InputComponent;
class CameraComponent;
class Scene;
struct InputValue;
class Pawn;

#define BIND_ACTION(fn) std::bind(&fn, this, std::placeholders::_1)

class App {
public:
	App();
	~App();

	void run();

private:
	void init();
	void initLighting();
	void setupInput();

	void update(float deltaTime);
	void render();

	Window m_Window;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	bool m_IsPaused = false;
	bool m_ShowDepth = false;
	bool m_IsPawnPossessed = true;

	CameraComponent* m_MainCamera;
	Pawn* m_Pawn;

	std::unique_ptr<Shader> m_ObjectShader;
	std::unique_ptr<Actor> m_CameraActor;
	std::unique_ptr<Scene> m_ActiveScene;

	std::unique_ptr<Keyboard> m_Keyboard;
	std::unique_ptr<Mouse> m_Mouse;
	std::unique_ptr<InputComponent> m_InputComponent;

	std::unique_ptr<DirLight> m_DirLight;
	std::unique_ptr<SpotLight> m_SpotLight;
	std::vector<PointLight> m_PointLights;

	void setMappingContext();
	void moveForward(const InputValue& val);
	void moveRight(const  InputValue& val);
	void TogglePause(const InputValue& val);
	void ToggleDepthTest(const InputValue& val);
	void ToggleCollisionDebug(const InputValue& val);
	void TogglePossession();
	void mouseLook(const InputValue& val);
	void mouseScroll(const InputValue& val);
	void quit(const InputValue& val);
};

#endif // !APP_H