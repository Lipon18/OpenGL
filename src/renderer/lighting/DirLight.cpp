#include "pch.h"
#include "DirLight.h"
#include "renderer/shaders/Shader.h"

DirLight::DirLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) : Light(amb, diff, spec), m_direction(dir) {}

void DirLight::apply(Shader& shader, const std::string& name) const {
	/*if (!shader) {
		std::cout << "Shader is NULL! please check." << std::endl;
		return;
	}*/
	shader.setVec3(name + ".direction", m_direction);
	shader.setVec3(name + ".ambient", ambient);
	shader.setVec3(name + ".diffuse", diffuse);
	shader.setVec3(name + ".specular", specular);
}
