#include "pch.h"
#include "Material.h"
#include "renderer/shaders/Shader.h"
#include "renderer/Renderer.h"

Material::Material(std::shared_ptr<Shader> shader) : m_Shader(shader) {}

void Material::setTexture(unsigned int textureID) {
	m_TextureID = textureID;
}

void Material::setColor(const glm::vec3& color) {
	m_Color = color;
}

void Material::apply() {
	if (!m_Shader) return;

	m_Shader->activate();
	m_Shader->setVec3("materialColor", m_Color);
	m_Shader->setMat4("view", gl_View);
	m_Shader->setMat4("projection", gl_Projection);
	m_Shader->setBool("showDepth", Renderer::getShowDepth());

	if (m_TextureID != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		m_Shader->setInt("texture_diffuse1", 0);
	}
}