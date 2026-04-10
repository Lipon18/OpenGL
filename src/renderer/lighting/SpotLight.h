#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"

class SpotLight : public Light {
public:
	SpotLight(glm::vec3 pos, glm::vec3 dir, float inner, float outer, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
	void apply(Shader& shader, const std::string& name) const override;

	void setPosition(const glm::vec3& pos) { m_Position = pos; }
	void setDirection(const glm::vec3& dir) { m_Direction = dir; }
private:
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	float m_CutOff, m_OuterCutOff;
	float m_Constant, m_Linear, m_Quadratic;
};

#endif

