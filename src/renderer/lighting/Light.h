#ifndef LIGHT_H
#define LIGHT_H

class Shader;

class Light {
public:
	Light(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
	virtual ~Light() = default;
	virtual void apply(Shader& shader, const std::string& name) const = 0;
protected:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

#endif // !LIGHT_H