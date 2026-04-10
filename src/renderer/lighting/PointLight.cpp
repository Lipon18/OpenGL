#include "pch.h"
#include "PointLight.h"
#include "renderer/shaders/Shader.h"

PointLight::PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float constant, float linear, float quadratic) : Light(amb, diff, spec),
m_Position(pos), m_Constant(constant), m_Linear(linear), m_Quadratic(quadratic) {}

void PointLight::apply(Shader& shader, const std::string& name) const {
	/*if (!shader) {
		std::cout << "Shader is NULL! please check." << std::endl;
		return;
	}*/
    shader.setVec3(name + ".position", m_Position);
    shader.setVec3(name + ".ambient", ambient);
    shader.setVec3(name + ".diffuse", diffuse);
    shader.setVec3(name + ".specular", specular);
    shader.setFloat(name + ".constant", m_Constant);
    shader.setFloat(name + ".linear", m_Linear);
    shader.setFloat(name + ".quadratic", m_Quadratic);
};
