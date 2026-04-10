#ifndef DIRLIGHT_H
#define DIRLIGHT_H

#include "Light.h"

class DirLight : public Light {
public:
	DirLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
	void apply(Shader& shader, const std::string& name) const override;
private:
	glm::vec3 m_direction;
};

#endif // !DIRLIGHT_H
