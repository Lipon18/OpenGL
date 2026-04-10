#ifndef MATERIAL_H
#define MATERIAL_H

class Shader;

extern glm::mat4 gl_View;
extern glm::mat4 gl_Projection;

class Material {
public:
	Material(std::shared_ptr<Shader> shader);

	void setTexture(unsigned int textureID);
	void setColor(const glm::vec3& color);

	void apply();

	std::shared_ptr<Shader> getShader() const { return m_Shader; }

private:
	std::shared_ptr<Shader> m_Shader;
	unsigned int m_TextureID = 0;
	glm::vec3 m_Color = glm::vec3(1.0f);
};

#endif // !MATERIAL_H