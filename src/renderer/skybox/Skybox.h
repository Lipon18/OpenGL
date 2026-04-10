#ifndef SKYBOX_H
#define SKYBOX_H

class Shader;
class Cubemap;

class Skybox {
public:
	Skybox(const std::vector<std::string>& faces); 
	~Skybox();

	void render(const glm::mat4& view, const glm::mat4& projection);

private:
	unsigned int m_VAO, m_VBO;
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Cubemap> m_Cubemap;
};
#endif // !SKYBOX_H