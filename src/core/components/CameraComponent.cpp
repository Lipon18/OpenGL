#include "pch.h"
#include "CameraComponent.h"
#include "core/components/TransformComponent.h"

void CameraComponent::init() {
	m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 3.0f, 7.0f));

	m_Camera->setCamera_YAW(-90.0f);
	m_Camera->setCamera_PITCH(-20.0f);

	if (owner) {
		m_Transform = owner->getComponent<TransformComponent>();
	}
}

void CameraComponent::update(float deltaTime) {
	if (!m_Transform || !m_Camera) return;

	//m_Camera->setCameraPosition(m_Transform->getPosition());
	m_Transform->setPosition(m_Camera->getCameraPosition());
	m_Transform->setRotation(glm::vec3(m_Camera->getCamera_PITCH(), m_Camera->getCamera_YAW(), 0.0f));
}