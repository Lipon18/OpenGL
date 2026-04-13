#include "pch.h"
#include "SpringArmComponent.h"
#include "core/components/TransformComponent.h"

void SpringArmComponent::update(float deltaTime) {
	if (!owner) return;
	auto transformComponent = owner->getComponent<TransformComponent>();
	if (!transformComponent) return;

	glm::vec3 forward = glm::vec3(cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw)), sin(glm::radians(m_Pitch)), cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw)));
	m_Position = transformComponent->getPosition() - (forward * m_TargetArmLength) + m_SocketOffset;
}
