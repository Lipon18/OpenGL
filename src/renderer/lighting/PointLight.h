#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

class PointLight : public Light {
public:
	PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float c, float l, float q);
	void apply(Shader& shader, const std::string& name) const override;
private:
	float m_Constant, m_Linear, m_Quadratic;
	glm::vec3 m_Position;
};

#endif

