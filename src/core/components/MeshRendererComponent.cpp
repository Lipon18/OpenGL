#include "pch.h"
#include "MeshRendererComponent.h"
#include "core/mesh/Model.h"
#include "core/mesh/Mesh.h"
#include "renderer/shaders/Shader.h"
#include "renderer/Renderer.h"
#include "renderer/material/Material.h"
#include "core/components/TransformComponent.h"

extern glm::mat4 gl_View;
extern glm::mat4 gl_Projection;

MeshRendererComponent::MeshRendererComponent(std::shared_ptr<Model> model, std::shared_ptr<Material> material) :
	m_Model(model), m_Material(material) {
	name = "MeshRendererComponent";
}

void MeshRendererComponent::render() {
	if (!m_Model || !m_Material || !owner) return;

	auto transform = owner->getComponent<TransformComponent>();
	if (!transform) return;

	m_Material->apply();

	auto shader = m_Material->getShader();
	shader->setMat4("model", transform->getModelMatrix());
	shader->setMat4("view", gl_View);
	shader->setMat4("projection", gl_Projection);
	shader->setVec2("uvScale", transform->getUVScale());
	shader->setVec2("uvOffset", transform->getUVOffset());
	shader->setBool("showDepth", Renderer::getShowDepth());

	m_Model->draw(*shader);
}
