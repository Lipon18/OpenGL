#include "pch.h"
#include "Actor.h"
#include "renderer/shaders/Shader.h"
#include "core/components/BaseComponent.h"
#include "core/components/MeshRendererComponent.h"

Actor::Actor() {}

Actor::~Actor() {
	m_Components.clear();
}
void Actor::updateComponents(float deltaTime) {
	for (auto& comp : m_Components) {
		comp->update(deltaTime);
	}
}

void Actor::renderComponents() {
	/*for (auto& comp : m_Components) {
		if (auto renderer = dynamic_cast<MeshRendererComponent*>(comp.get())) {
			renderer->render();
		}
	}*/
	for (auto& comp : m_Components) {
		comp->render();
	}
}