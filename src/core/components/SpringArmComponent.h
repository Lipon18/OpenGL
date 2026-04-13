#ifndef SPRINGARMCOMPONENT_H
#define SPRINGARMCOMPONENT_H

#include "BaseComponent.h"

class SpringArmComponent : public BaseComponent {
public:
	void update(float deltaTime) override;

	glm::vec3 getPosition() const { return m_Position; }
	void setRotation(float pitch, float yaw) { m_Pitch = pitch; m_Yaw = yaw; }


private:
	float m_TargetArmLength = 5.0f;
	glm::vec3 m_SocketOffset = glm::vec3(0.0f, 2.0f, 0.0f);
	glm::vec3 m_Position;
	float m_Pitch = 0.0f;
	float m_Yaw = -90.0f;
};

#endif // !SPRINGARMCOMPONENT_H