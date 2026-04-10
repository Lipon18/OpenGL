#include "pch.h"
#include "SpotLight.h"
#include "renderer/shaders/Shader.h"

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 dir, float inner, float outer, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) : Light(amb, diff, spec), 
m_Position(pos), m_Direction(dir), m_CutOff(inner), m_OuterCutOff(outer), m_Constant(1.0f), m_Linear(0.09f), m_Quadratic(0.032f) {}

void SpotLight::apply(Shader& shader, const std::string& name) const {
	/*if (!shader) {
		std::cout << "Shader is NULL! please check." << std::endl;
		return;
	}*/
    shader.setVec3(name + ".position", m_Position);
    shader.setVec3(name + ".direction", m_Direction);
    shader.setVec3(name + ".ambient", ambient);
    shader.setVec3(name + ".diffuse", diffuse);
    shader.setVec3(name + ".specular", specular);
    shader.setFloat(name + ".constant", m_Constant);
    shader.setFloat(name + ".linear", m_Linear);
    shader.setFloat(name + ".quadratic", m_Quadratic);
    shader.setFloat(name + ".cutOff", glm::cos(glm::radians(m_CutOff)));
    shader.setFloat(name + ".outerCutOff", glm::cos(glm::radians(m_OuterCutOff)));
}