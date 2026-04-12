#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "BaseComponent.h"
#include "core/camera/Camera.h"

class TransformComponent;

class CameraComponent : public BaseComponent {
public:
	CameraComponent() : m_Transform(nullptr) {};

	void init() override;
	void update(float deltaTime) override;

	glm::mat4 getViewMatrix() const { return m_Camera ? m_Camera->GetViewMatrix() : glm::mat4(1.0f); }
	void processKeyboard(E_CameraMovement direction, float deltaTime) { m_Camera->ProcessKeyboard(direction, deltaTime); }
	void processMouseMovement(float x, float y) { m_Camera->ProcessMouseMovement(x, y); }
	void processMouseScroll(float offset) { m_Camera->ProcessMouseScroll(offset); }
	void setYaw(float yaw) { m_Camera->setCamera_YAW(yaw); }
	void setPitch(float pitch) { m_Camera->setCamera_PITCH(pitch); }

	float getZoom() const { return m_Camera->getCameraZoom(); }
	glm::vec3 getPosition() const { return m_Camera->getCameraPosition(); }
	void setPosition(const glm::vec3& position) { m_Camera->setCameraPosition(position); }
	glm::vec3 getFront() const { return m_Camera->getCameraFront(); }

	TransformComponent* getTransform() const { return m_Transform; }

	void setRelativeOffset(const glm::vec3& offset) { m_RelativeOffset = offset; }

private:
	std::unique_ptr<Camera> m_Camera;
	TransformComponent* m_Transform;
	glm::vec3 m_RelativeOffset = glm::vec3(0.0f);
};

#endif // !CAMERACOMPONENT_H